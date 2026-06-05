from pyqttier.lwt import OnlinePresence
from pyqttier.message import Message


def StingerPresence(
    client_id: str,
) -> OnlinePresence:
    """Helper function to create an OnlinePresence instance for StingerIPC clients."""

    def _create_message(is_online: bool) -> Message:
        online_value = "true" if is_online else "false"
        return Message(
            topic=f"client/{client_id}/online",
            payload=f'{"online": {online_value} }.encode("utf-8")',
            qos=1,
            retain=True,
            content_type="application/json",
        )

    return OnlinePresence(
        topic=f"client/{client_id}/online",
        online=_create_message(is_online=True),
        offline=_create_message(is_online=False),
    )
