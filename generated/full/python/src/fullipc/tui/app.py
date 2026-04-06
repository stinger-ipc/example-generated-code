"""Main TUI application for FullIPC."""

import typer
from typing import Optional, Dict, Any

cli = typer.Typer()
from textual.app import App # typing: ignore
from textual.screen import Screen # typing: ignore
from textual.command import Provider, Hit  # typing: ignore
from pyqttier.connection import Mqtt5Connection
from fullipc.client import FullClient


class LogsCommandProvider(Provider):
    """Command provider for showing logs."""

    async def search(self, query: str):
        """Search for log-related commands."""
        matcher = self.matcher(query)
        
        if matcher.match("logs") or matcher.match("show logs") or matcher.match("view logs"):
            yield Hit(
                score=matcher.match("show logs"),
                match_display="Show Logs",
                command=lambda: self.app.push_screen("logs"),
                help="View application logs for debugging"
            )


class FullIPCApp(App):
    """A Textual app for FullIPC client interface."""
    
    # Store the MQTT connection and client globally
    mqtt_connection: Optional[Mqtt5Connection] = None
    full_client: Optional[FullClient] = None
    
    # Store TLS configuration
    tls_config: Dict[str, Any] = dict()
    
    COMMANDS = {LogsCommandProvider}

    CSS = """
    Screen {
        align: center middle;
    }
    """
    
    def on_mount(self) -> None:
        """Start with the connection screen."""
        # Import screens here to avoid circular imports
        from fullipc.tui.connection import ConnectionScreen
        from fullipc.tui.discovery import DiscoveryScreen
        from fullipc.tui.logs import LogsScreen, install_log_handler
        
        # Install log capture handler
        install_log_handler()

        # Install screens
        self.install_screen(ConnectionScreen(), name="connection")
        self.install_screen(DiscoveryScreen(), name="discovery")
        self.install_screen(LogsScreen(), name="logs")
        
        # Start with connection screen
        self.push_screen("connection")


@cli.command(
    epilog="Environment variables: MQTT_HOSTNAME — MQTT broker hostname (default: 'localhost')"
)
def main(
    cafile: Optional[str] = typer.Option(None, help="Path to CA certificate file for TLS certificate validation"),
    capath: Optional[str] = typer.Option(None, help="Path to directory containing CA certificates for TLS"),
    cert: Optional[str] = typer.Option(None, help="Path to client certificate file for TLS client authentication"),
    insecure: bool = typer.Option(False, help="Disable TLS certificate verification (not recommended for production)"),
) -> None:
    """Run the TUI application."""
    app = FullIPCApp()

    # Store TLS configuration in the app
    app.tls_config = {
        "cafile": cafile,
        "capath": capath,
        "cert": cert,
        "insecure": insecure,
    }

    app.run()


if __name__ == "__main__":
    cli()