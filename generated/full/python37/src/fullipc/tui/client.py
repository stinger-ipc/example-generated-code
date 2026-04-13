"""Client screen for interacting with FullIPC server."""

import concurrent.futures as futures
from datetime import datetime, timedelta
import isodate
from typing import List, Optional, Any, Dict
from textual.app import ComposeResult  # typing: ignore
from textual.screen import Screen, ModalScreen  # typing: ignore
from textual.widgets import Header, Footer, Static, RichLog, Button, Input, Label, Select, TextArea, Markdown  # typing: ignore
from textual.containers import Horizontal, VerticalScroll, Vertical  # typing: ignore
from fullipc.interface_types import *
from fullipc.client import FullClient
from pydantic import TypeAdapter
import logging

# Configure logging
logger = logging.getLogger("TUI-Client")
logger.setLevel(logging.DEBUG)


class HelpModal(ModalScreen):
    """Modal screen for displaying markdown-formatted help text."""

    CSS = """
    HelpModal {
        align: center middle;
    }

    #help_modal_container {
        width: 70%;
        height: auto;
        max-height: 80%;
        background: $surface;
        border: thick $primary;
        padding: 1;
    }

    #help_modal_title {
        text-style: bold;
        text-align: center;
        background: $primary;
        padding: 1;
        margin-bottom: 1;
    }

    #help_content {
        height: auto;
        max-height: 60vh;
        overflow-y: auto;
        padding: 0 1;
    }

    #help_close_button {
        width: 100%;
        margin-top: 1;
    }
    """

    BINDINGS = [("escape", "dismiss", "Close")]

    def __init__(self, text: str, title: str = "Help"):
        super().__init__()
        self._text = text
        self._title = title

    def compose(self) -> ComposeResult:
        """Compose the help modal."""
        with Vertical(id="help_modal_container"):
            yield Static(self._title, id="help_modal_title")
            yield Markdown(self._text, id="help_content")
            yield Button("Close", variant="primary", id="help_close_button")

    def on_button_pressed(self, event: Button.Pressed) -> None:
        if event.button.id == "help_close_button":
            self.dismiss()


class PropertyEditModal(ModalScreen[bool]):
    """Modal screen for editing a property value."""

    CSS = """
    PropertyEditModal {
        align: center middle;
    }
    
    #property_modal_container {
        width: 60%;
        height: auto;
        background: $surface;
        border: thick $primary;
        padding: 1;
    }
    
    #property_modal_title {
        text-style: bold;
        text-align: center;
        background: $primary;
        padding: 1;
        margin-bottom: 1;
    }
    
    .property_input_label {
        margin-top: 1;
        margin-bottom: 1;
    }

    .property_input_value_label {
        margin-top: 1;
        margin-bottom: 1;
        color: $primary;
    }
    
    #property_input {
        margin-bottom: 1;
    }

    .property_input_value {
        margin-bottom: 1;
    }
    
    #property_button_container {
        layout: horizontal;
        height: auto;
        margin-top: 1;
    }
    
    #property_button_container Button {
        width: 1fr;
        margin: 0 1;
    }
    """

    def __init__(self, property_name: str, current_value: Any, client: FullClient):
        super().__init__()
        self.property_name = property_name
        self.current_value = current_value
        self.client = client

    def compose(self) -> ComposeResult:
        """Compose the modal screen."""
        with Vertical(id="property_modal_container"):
            yield Static(f"Edit: {self.property_name}", id="property_modal_title")
            yield Label(f"Current value: {self.current_value}", classes="property_input_label")
            if self.property_name == "favorite_number":

                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")

            if self.property_name == "favorite_foods":
                yield Label(f"drink", classes="property_input_value_label")
                yield Input(placeholder=f"drink value", value=str(self.current_value.drink), classes="property_input_value", id="property_input_drink")

                yield Label(f"slices_of_pizza", classes="property_input_value_label")
                yield Input(placeholder=f"slices_of_pizza value", value=str(self.current_value.slices_of_pizza), classes="property_input_value", id="property_input_slices_of_pizza")

                yield Label(f"breakfast", classes="property_input_value_label")
                yield Input(placeholder=f"breakfast value", value=str(self.current_value.breakfast), classes="property_input_value", id="property_input_breakfast")

            if self.property_name == "lunch_menu":
                yield Label(f"monday (JSON)", classes="property_input_value_label")
                yield Input(placeholder=f"monday value", value=self.current_value.monday.model_dump_json(), classes="property_input_value", id="property_input_monday")

                yield Label(f"tuesday (JSON)", classes="property_input_value_label")
                yield Input(placeholder=f"tuesday value", value=self.current_value.tuesday.model_dump_json(), classes="property_input_value", id="property_input_tuesday")

            if self.property_name == "family_name":

                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")

            if self.property_name == "last_breakfast_time":

                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")

            if self.property_name == "last_birthdays":
                yield Label(f"mom", classes="property_input_value_label")
                yield Input(placeholder=f"mom value", value=str(self.current_value.mom), classes="property_input_value", id="property_input_mom")

                yield Label(f"dad", classes="property_input_value_label")
                yield Input(placeholder=f"dad value", value=str(self.current_value.dad), classes="property_input_value", id="property_input_dad")

                yield Label(f"sister", classes="property_input_value_label")
                yield Input(placeholder=f"sister value", value=str(self.current_value.sister), classes="property_input_value", id="property_input_sister")

                yield Label(f"brothers_age", classes="property_input_value_label")
                yield Input(placeholder=f"brothers_age value", value=str(self.current_value.brothers_age), classes="property_input_value", id="property_input_brothers_age")

            with Horizontal(id="property_button_container"):
                yield Button("Update", variant="primary", id="update_button")
                yield Button("Cancel", variant="error", id="cancel_button")
                yield Button("Help", variant="default", id="help_button")

    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Handle button presses."""
        if event.button.id == "update_button":
            try:
                if self.property_name == "favorite_number":
                    input_widget = self.query_one("#property_input")
                    new_favorite_number_value = int(input_widget.value)

                    self.client.favorite_number = new_favorite_number_value
                elif self.property_name == "favorite_foods":
                    input_widget_drink = self.query_one("#property_input_drink", Input)
                    # drink should be ArgPrimitiveType.STRING
                    new_favorite_foods_value_drink = str(input_widget_drink.value)

                    input_widget_slices_of_pizza = self.query_one("#property_input_slices_of_pizza", Input)
                    # slices_of_pizza should be ArgPrimitiveType.INTEGER
                    new_favorite_foods_value_slices_of_pizza = int(input_widget_slices_of_pizza.value)

                    input_widget_breakfast = self.query_one("#property_input_breakfast", Input)
                    # breakfast should be ArgPrimitiveType.STRING
                    new_favorite_foods_value_breakfast = str(input_widget_breakfast.value) if input_widget_breakfast.value else None

                    new_favorite_foods_value = FavoriteFoodsProperty(
                        drink=new_favorite_foods_value_drink,
                        slices_of_pizza=new_favorite_foods_value_slices_of_pizza,
                        breakfast=new_favorite_foods_value_breakfast,
                    )

                    self.client.favorite_foods = new_favorite_foods_value
                elif self.property_name == "family_name":
                    input_widget = self.query_one("#property_input")
                    new_family_name_value = str(input_widget.value)

                    self.client.family_name = new_family_name_value
                elif self.property_name == "last_breakfast_time":
                    input_widget = self.query_one("#property_input")
                    new_last_breakfast_time_value = datetime.fromisoformat(input_widget.value)

                    self.client.last_breakfast_time = new_last_breakfast_time_value
                elif self.property_name == "last_birthdays":
                    input_widget_mom = self.query_one("#property_input_mom", Input)
                    # mom should be
                    new_last_birthdays_value_mom = datetime.fromisoformat(input_widget_mom.value)

                    input_widget_dad = self.query_one("#property_input_dad", Input)
                    # dad should be
                    new_last_birthdays_value_dad = datetime.fromisoformat(input_widget_dad.value)

                    input_widget_sister = self.query_one("#property_input_sister", Input)
                    # sister should be
                    new_last_birthdays_value_sister = datetime.fromisoformat(input_widget_sister.value) if input_widget_sister.value else None

                    input_widget_brothers_age = self.query_one("#property_input_brothers_age", Input)
                    # brothers_age should be ArgPrimitiveType.INTEGER
                    new_last_birthdays_value_brothers_age = int(input_widget_brothers_age.value) if input_widget_brothers_age.value else None

                    new_last_birthdays_value = LastBirthdaysProperty(
                        mom=new_last_birthdays_value_mom,
                        dad=new_last_birthdays_value_dad,
                        sister=new_last_birthdays_value_sister,
                        brothers_age=new_last_birthdays_value_brothers_age,
                    )

                    self.client.last_birthdays = new_last_birthdays_value

                self.dismiss(True)
            except Exception as e:
                self.app.notify(f"Error updating property: {e}", severity="error")
        elif event.button.id == "help_button":
            help_text = "No documentation available."
            if self.property_name == "favorite_number":
                help_text = """My favorite number
"""
            elif self.property_name == "favorite_foods":
                help_text = """No documentation provided for this property."""
            elif self.property_name == "lunch_menu":
                help_text = """No documentation provided for this property."""
            elif self.property_name == "family_name":
                help_text = """This is to test a property with a single string value."""
            elif self.property_name == "last_breakfast_time":
                help_text = """This is to test a property with a single datetime value."""
            elif self.property_name == "last_birthdays":
                help_text = """This is to test a property with multiple datetime values."""

            self.app.push_screen(HelpModal(help_text, title=f"Help: {self.property_name}"))
        else:
            self.dismiss(False)


class MethodCallModal(ModalScreen[Optional[str]]):
    """Modal screen for calling a method with inputs."""

    CSS = """
    MethodCallModal {
        align: center middle;
    }
    
    #modal_container {
        width: 80;
        height: auto;
        max-height: 90%;
        background: $surface;
        border: thick $primary;
        padding: 1;
    }
    
    #modal_title {
        text-style: bold;
        text-align: center;
        background: $primary;
        padding: 1;
        margin-bottom: 1;
    }
    
    #inputs_container {
        height: auto;
        max-height: 60%;
        overflow-y: auto;
    }
    
    .input_label {
        margin-top: 1;
        margin-bottom: 1;
    }
    
    Input {
        margin-bottom: 1;
    }
    
    TextArea {
        height: 40vw;
        width: 100%;
    }

    #button_container {
        layout: horizontal;
        height: auto;
        margin-top: 1;
    }
    
    #button_container Button {
        width: 1fr;
        margin: 0 1;
    }
    
    #result_text {
        margin-top: 1;
        padding: 1;
        background: $panel;
        border: solid $accent;
        max-height: 20;
        overflow-y: auto;
    }
    """

    def __init__(self, method_name: str, params: Dict[str, type], client: Any):
        super().__init__()
        self.method_name = method_name
        self.params = params
        self.client = client
        self.result_widget: Optional[Static] = None

    def compose(self) -> ComposeResult:
        """Compose the modal screen."""
        with Vertical(id="modal_container"):
            yield Static(f"Call: {self.method_name}", id="modal_title")

            with VerticalScroll(id="inputs_container"):
                """Vertical Scroll container for method inputs"""

                if self.method_name == "add_numbers":
                    """Generate input fields for each argument of the method"""

                    yield Label(f"first (PRIMITIVE)", classes="input_label")
                    yield Input(
                        type="integer",
                        id=f"input_first",
                        placeholder="None",
                    )

                    yield Label(f"second (PRIMITIVE)", classes="input_label")
                    yield Input(
                        type="integer",
                        id=f"input_second",
                        placeholder="None",
                    )

                    yield Label(f"third (PRIMITIVE) [Optional]", classes="input_label")
                    yield Input(
                        type="integer",
                        id=f"input_third",
                        placeholder="None",
                    )

                if self.method_name == "do_something":
                    """Generate input fields for each argument of the method"""

                    yield Label(f"task_to_do (PRIMITIVE)", classes="input_label")
                    yield Input(
                        type="text",
                        id=f"input_task_to_do",
                        placeholder="None",
                    )

                if self.method_name == "hold_temperature":
                    """Generate input fields for each argument of the method"""

                    yield Label(f"temperature_celsius (PRIMITIVE)", classes="input_label")
                    yield Input(
                        type="number",
                        id=f"input_temperature_celsius",
                        placeholder="The temperature to hold in degrees Celsius.",
                    )

            with Horizontal(id="button_container"):
                yield Button("Call Method", variant="primary", id="call_button")
                yield Button("Close", variant="error", id="cancel_button")
                yield Button("Help", variant="default", id="help_button")

            self.result_widget = Static("", id="result_text")
            yield self.result_widget

    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Handle button presses."""
        if event.button.id == "cancel_button":
            self.dismiss(None)
        elif event.button.id == "call_button":
            self._call_method()
        elif event.button.id == "help_button":
            help_text = "No documentation available."

            if self.method_name == "add_numbers":
                help_text = """No documentation provided for this method."""

            elif self.method_name == "do_something":
                help_text = """No documentation provided for this method."""

            elif self.method_name == "what_time_is_it":
                help_text = """Get the current date and time."""

            elif self.method_name == "hold_temperature":
                help_text = """Hold a temperature for a specified duration."""

            self.app.push_screen(HelpModal(help_text, title=f"Help: {self.method_name}"))

    def _call_method(self) -> None:
        """Call the method with collected inputs."""
        assert self.result_widget is not None, "result_widget must be initialized"
        logger.debug("Calling method '%s' with params: %s", self.method_name, self.params)
        try:
            # Collect inputs
            kwargs = {}  # type: Dict[str, Any]

            if self.method_name == "add_numbers":

                add_numbers_first_input_widget = self.query_one(f"#input_first", Input)
                add_numbers_first_value = int(add_numbers_first_input_widget.value)
                kwargs["first"] = add_numbers_first_value

                add_numbers_second_input_widget = self.query_one(f"#input_second", Input)
                add_numbers_second_value = int(add_numbers_second_input_widget.value)
                kwargs["second"] = add_numbers_second_value

                add_numbers_third_input_widget = self.query_one(f"#input_third", Input)
                add_numbers_third_value = int(add_numbers_third_input_widget.value)
                kwargs["third"] = add_numbers_third_value

            if self.method_name == "do_something":

                do_something_task_to_do_input_widget = self.query_one(f"#input_task_to_do", Input)
                do_something_task_to_do_value = str(do_something_task_to_do_input_widget.value)
                kwargs["task_to_do"] = do_something_task_to_do_value

            if self.method_name == "hold_temperature":

                hold_temperature_temperature_celsius_input_widget = self.query_one(f"#input_temperature_celsius", Input)
                hold_temperature_temperature_celsius_value = float(hold_temperature_temperature_celsius_input_widget.value)
                kwargs["temperature_celsius"] = hold_temperature_temperature_celsius_value

            # Call the method
            self.result_widget.update("[yellow]Calling method...[/yellow]")
            method = getattr(self.client, self.method_name)
            future_result = method(**kwargs)

            # Wait for result (with timeout)
            try:
                result = future_result.result(timeout=5.0)
                self.result_widget.update(f"[green]Success![/green]\n\nResult: {result}")
            except futures.TimeoutError:
                self.result_widget.update("[red]Timeout waiting for response[/red]")
            except Exception as e:
                self.result_widget.update(f"[red]Error: {e}[/red]")

        except Exception as e:
            self.result_widget.update(f"[red]Error preparing call: {e}[/red]")
            logger.exception(f"Error preparing method call {self.method_name}", exc_info=e)


class ClientScreen(Screen):
    """Screen for interacting with a connected FullIPC server."""

    CSS = """
    ClientScreen {
        layout: vertical;
    }
    
    #main_container {
        width: 100%;
        height: 1fr;
    }
    
    #left_pane {
        width: 20%;
        border-right: solid $primary;
        padding: 1;
    }
    
    #middle_pane {
        width: 30%;
        border-right: solid $primary;
        padding: 1;
    }
    
    #right_pane {
        width: 50%;
        padding: 1;
    }
    
    .pane_title {
        text-style: bold;
        background: $primary;
        padding: 1;
        text-align: center;
        margin-bottom: 1;
    }
    
    RichLog {
        height: 1fr;
        border: solid $accent;
    }
    
    .property_item {
        margin: 0;
        padding: 1;
        background: $surface;
        border: solid $accent;
    }
    
    .property_item.writable {
        background: $surface;
    }
    
    .property_item.writable:hover {
        background: $accent 20%;
    }
    
    .property_item.readonly {
        background: $surface-darken-1;
        color: $text-muted;
    }
    
    .property_name {
        text-style: bold;
        color: $accent;
    }
    
    .property_value {
        margin-top: 1;
        color: $text;
    }
    """

    BINDINGS = [
        ("escape", "back_to_discovery", "Back to Discovery"),
    ]

    def __init__(self):
        """Initialize the client screen."""
        super().__init__()
        self.client = None

    def compose(self) -> ComposeResult:
        """Compose the client screen widgets."""
        yield Header()
        with Horizontal(id="main_container"):
            with VerticalScroll(id="left_pane"):
                yield Static("Methods", classes="pane_title")
                # Method buttons will be added dynamically
            with VerticalScroll(id="middle_pane"):
                yield Static("Properties", classes="pane_title")
                # Properties will be added dynamically

            with VerticalScroll(id="right_pane"):
                yield Static("Signals", classes="pane_title")
                yield RichLog(id="signals_log", highlight=True, markup=True)
        yield Footer()

    def on_mount(self) -> None:
        """Set up signal handlers when screen mounts."""
        logger.debug("Mounting client screen")

        # Get the client from the app
        self.client = self.app.full_client

        if self.client is None:
            self.notify("No client available!", severity="error")
            return

        # Add method buttons
        self._add_method_buttons()
        # Register all signal handlers
        self._register_signal_handlers()

        # Register all property handlers
        self._register_property_handlers()

    def on_show(self):
        logger.debug("Showing client screen")

    def _add_method_buttons(self) -> None:
        """Add buttons for all call_* methods."""
        pane = self.query_one("#left_pane", VerticalScroll)

        # Define all methods with their parameters
        methods = {
            "add_numbers": {
                "first": int,
                "second": int,
                "third": Optional[int],
            },
            "do_something": {
                "task_to_do": str,
            },
            "what_time_is_it": {},
            "hold_temperature": {
                "temperature_celsius": float,
            },
        }  # type: Dict[str, Dict[str, Any]]

        for method_name, params in methods.items():
            btn = Button(method_name, classes="method_button")
            btn.method_name = method_name  # Store for retrieval
            btn.method_params = params
            pane.mount(btn)

    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Handle method button presses."""
        if hasattr(event.button, "method_name"):
            method_name = event.button.method_name
            method_params = event.button.method_params

            # Show modal for method call
            modal = MethodCallModal(method_name, method_params, self.client)
            self.app.push_screen(modal)

    def _register_signal_handlers(self) -> None:
        """Register callbacks for all receive_* methods."""
        log = self.query_one("#signals_log", RichLog)

        # Define a generic handler factory
        def make_handler(signal_name: str):
            def handler(*args, **kwargs):
                # Format the received data
                if args:
                    data = ", ".join([f"{arg}" for arg in args])
                elif kwargs:
                    data = ", ".join([f"{k}={v}" for k, v in kwargs.items()])
                else:
                    data = "(no data)"

                # Log to the RichLog widget
                timestamp = datetime.now().strftime("%H:%M:%S")
                log.write(f"[gray]{timestamp}[/gray] [bold cyan]{signal_name}[/bold cyan]: {data}")

            return handler

        # Register all signal handlers
        assert self.client is not None, "Client must be initialized"
        logger.debug("Registering TUI handler for signal '%s'", "today_is")
        self.client.receive_today_is(make_handler("today_is"))
        logger.debug("Registering TUI handler for signal '%s'", "random_word")
        self.client.receive_random_word(make_handler("random_word"))

    def _register_property_handlers(self) -> None:
        """Register callbacks for all *_changed methods and create property displays."""
        pane = self.query_one("#middle_pane", VerticalScroll)

        # Define property registration helper
        def register_property(prop_name: str, changed_method_name: str, is_writable: bool = False):
            # Create a Static widget for this property
            prop_widget = Static(id=f"prop_{prop_name}")
            prop_widget.add_class("property_item")

            # Add writable or readonly class
            if is_writable:
                prop_widget.add_class("writable")
                prop_widget.can_focus = True
                prop_widget.property_name = prop_name  # Store for click handling
            else:
                prop_widget.add_class("readonly")

            pane.mount(prop_widget)

            if prop_name == "favorite_number":

                def on_favorite_number_updated(value: int):
                    prop_widget.current_value = value

                    values = []

                    # Single value
                    if value is None:
                        values.append("None")
                    else:

                        values.append(f"{value}")  # other - PRIMITIVE

                    value_str = "\n".join(values)

                    # Update the widget
                    prop_widget.update(f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}")

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.favorite_number_changed(on_favorite_number_updated, call_immediately=True)

            elif prop_name == "favorite_foods":

                def on_favorite_foods_updated(value: FavoriteFoodsProperty):
                    prop_widget.current_value = value

                    values = []

                    # Multiple values

                    line = f"[bold]drink[/bold]: { value.drink }"  # Other - PRIMITIVE
                    values.append(line)

                    line = f"[bold]slices_of_pizza[/bold]: { value.slices_of_pizza }"  # Other - PRIMITIVE
                    values.append(line)

                    line = f"[bold]breakfast[/bold]: { value.breakfast }"  # Other - PRIMITIVE
                    values.append(line)

                    value_str = "\n".join(values)

                    # Update the widget
                    prop_widget.update(f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}")

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.favorite_foods_changed(on_favorite_foods_updated, call_immediately=True)

            elif prop_name == "lunch_menu":

                def on_lunch_menu_updated(value: LunchMenuProperty):
                    prop_widget.current_value = value

                    values = []

                    # Multiple values

                    if value.monday is None:
                        values.append(f"[bold]monday[/bold]: None")
                    else:
                        values.append(f"[bold]monday.drink[/bold]: { value.monday.drink }")  # Struct Member
                        values.append(f"[bold]monday.sandwich[/bold]: { value.monday.sandwich }")  # Struct Member
                        values.append(f"[bold]monday.crackers[/bold]: { value.monday.crackers }")  # Struct Member
                        values.append(f"[bold]monday.day[/bold]: { value.monday.day }")  # Struct Member
                        values.append(f"[bold]monday.order_number[/bold]: { value.monday.order_number }")  # Struct Member
                        values.append(f"[bold]monday.time_of_lunch[/bold]: { value.monday.time_of_lunch }")  # Struct Member
                        values.append(f"[bold]monday.duration_of_lunch[/bold]: { value.monday.duration_of_lunch }")  # Struct Member

                    if value.tuesday is None:
                        values.append(f"[bold]tuesday[/bold]: None")
                    else:
                        values.append(f"[bold]tuesday.drink[/bold]: { value.tuesday.drink }")  # Struct Member
                        values.append(f"[bold]tuesday.sandwich[/bold]: { value.tuesday.sandwich }")  # Struct Member
                        values.append(f"[bold]tuesday.crackers[/bold]: { value.tuesday.crackers }")  # Struct Member
                        values.append(f"[bold]tuesday.day[/bold]: { value.tuesday.day }")  # Struct Member
                        values.append(f"[bold]tuesday.order_number[/bold]: { value.tuesday.order_number }")  # Struct Member
                        values.append(f"[bold]tuesday.time_of_lunch[/bold]: { value.tuesday.time_of_lunch }")  # Struct Member
                        values.append(f"[bold]tuesday.duration_of_lunch[/bold]: { value.tuesday.duration_of_lunch }")  # Struct Member

                    value_str = "\n".join(values)

                    # Update the widget
                    prop_widget.update(f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}")

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.lunch_menu_changed(on_lunch_menu_updated, call_immediately=True)

            elif prop_name == "family_name":

                def on_family_name_updated(value: str):
                    prop_widget.current_value = value

                    values = []

                    # Single value
                    if value is None:
                        values.append("None")
                    else:

                        values.append(f"{value}")  # other - PRIMITIVE

                    value_str = "\n".join(values)

                    # Update the widget
                    prop_widget.update(f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}")

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.family_name_changed(on_family_name_updated, call_immediately=True)

            elif prop_name == "last_breakfast_time":

                def on_last_breakfast_time_updated(value: datetime):
                    prop_widget.current_value = value

                    values = []

                    # Single value
                    if value is None:
                        values.append("None")
                    else:

                        values.append(f"{value.isoformat() if value else 'None'}")  # datetime

                    value_str = "\n".join(values)

                    # Update the widget
                    prop_widget.update(f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}")

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.last_breakfast_time_changed(on_last_breakfast_time_updated, call_immediately=True)

            elif prop_name == "last_birthdays":

                def on_last_birthdays_updated(value: LastBirthdaysProperty):
                    prop_widget.current_value = value

                    values = []

                    # Multiple values

                    line = f"[bold]mom[/bold]: { value.mom.isoformat() if value.mom else 'None' }"  # Datetime
                    values.append(line)

                    line = f"[bold]dad[/bold]: { value.dad.isoformat() if value.dad else 'None' }"  # Datetime
                    values.append(line)

                    line = f"[bold]sister[/bold]: { value.sister.isoformat() if value.sister else 'None' }"  # Datetime
                    values.append(line)

                    line = f"[bold]brothers_age[/bold]: { value.brothers_age }"  # Other - PRIMITIVE
                    values.append(line)

                    value_str = "\n".join(values)

                    # Update the widget
                    prop_widget.update(f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}")

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.last_birthdays_changed(on_last_birthdays_updated, call_immediately=True)

        # Register all properties
        register_property("favorite_number", "favorite_number_changed", is_writable=True)
        register_property("favorite_foods", "favorite_foods_changed", is_writable=True)
        register_property("lunch_menu", "lunch_menu_changed", is_writable=False)
        register_property("family_name", "family_name_changed", is_writable=True)
        register_property("last_breakfast_time", "last_breakfast_time_changed", is_writable=True)
        register_property("last_birthdays", "last_birthdays_changed", is_writable=True)

    def on_click(self, event) -> None:
        """Handle clicks on property widgets."""
        assert self.client is not None, "Client must be initialized"

        # Check if the clicked widget is a writable property
        widget = event.widget
        if hasattr(widget, "property_name") and hasattr(widget, "current_value"):
            property_name = widget.property_name
            current_value = widget.current_value

            # Open the edit modal
            modal = PropertyEditModal(property_name, current_value, self.client)
            self.app.push_screen(modal)

    def action_back_to_discovery(self) -> None:
        """Navigate back to the discovery screen."""
        self.app.pop_screen()
