"""Client screen for interacting with TestableIPC server."""

import concurrent.futures as futures
from datetime import datetime, timedelta
import isodate
from typing import List, Optional, Any, Dict
from textual.app import ComposeResult # typing: ignore
from textual.screen import Screen, ModalScreen # typing: ignore
from textual.widgets import Header, Footer, Static, RichLog, Button, Input, Label, Select, TextArea, Markdown # typing: ignore
from textual.containers import Horizontal, VerticalScroll, Vertical # typing: ignore
from testableipc.interface_types import *
from testableipc.client import TestableClient
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
    
    def __init__(self, property_name: str, current_value: Any, client: TestableClient):
        super().__init__()
        self.property_name = property_name
        self.current_value = current_value
        self.client = client
    
    def compose(self) -> ComposeResult:
        """Compose the modal screen."""
        with Vertical(id="property_modal_container"):
            yield Static(f"Edit: {self.property_name}", id="property_modal_title")
            yield Label(f"Current value: {self.current_value}", classes="property_input_label")
            if self.property_name == 'read_write_integer':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_only_integer':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_optional_integer':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_two_integers':
                    yield Label(f"first", classes="property_input_value_label")
                    yield Input(placeholder=f"first value", value=str(self.current_value.first), classes="property_input_value", id="property_input_first")
                
                    yield Label(f"second", classes="property_input_value_label")
                    yield Input(placeholder=f"second value", value=str(self.current_value.second), classes="property_input_value", id="property_input_second")
                
            if self.property_name == 'read_only_string':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_string':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_optional_string':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_two_strings':
                    yield Label(f"first", classes="property_input_value_label")
                    yield Input(placeholder=f"first value", value=str(self.current_value.first), classes="property_input_value", id="property_input_first")
                
                    yield Label(f"second", classes="property_input_value_label")
                    yield Input(placeholder=f"second value", value=str(self.current_value.second), classes="property_input_value", id="property_input_second")
                
            if self.property_name == 'read_write_struct':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_optional_struct':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_two_structs':
                    yield Label(f"first (JSON)", classes="property_input_value_label")
                    yield Input(placeholder=f"first value", value=self.current_value.first.model_dump_json(), classes="property_input_value", id="property_input_first")
                
                    yield Label(f"second (JSON)", classes="property_input_value_label")
                    yield Input(placeholder=f"second value", value=self.current_value.second.model_dump_json(), classes="property_input_value", id="property_input_second")
                
            if self.property_name == 'read_only_enum':
                
                enum_options = [
                    (f"{member.name} ({member.value})", member.value)
                    for member in Numbers
                ]
                yield Select(options=enum_options, value=self.current_value.value if self.current_value is not None else None, id="property_input")
                
                
            if self.property_name == 'read_write_enum':
                
                enum_options = [
                    (f"{member.name} ({member.value})", member.value)
                    for member in Numbers
                ]
                yield Select(options=enum_options, value=self.current_value.value if self.current_value is not None else None, id="property_input")
                
                
            if self.property_name == 'read_write_optional_enum':
                
                enum_options = [
                    (f"{member.name} ({member.value})", member.value)
                    for member in Numbers
                ]
                yield Select(options=enum_options, value=self.current_value.value if self.current_value is not None else None, id="property_input")
                
                
            if self.property_name == 'read_write_two_enums':
                    yield Label(f"first", classes="property_input_value_label")
                    yield Input(placeholder=f"first value", value=str(self.current_value.first), classes="property_input_value", id="property_input_first")
                
                    yield Label(f"second", classes="property_input_value_label")
                    yield Input(placeholder=f"second value", value=str(self.current_value.second), classes="property_input_value", id="property_input_second")
                
            if self.property_name == 'read_write_datetime':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_optional_datetime':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_two_datetimes':
                    yield Label(f"first", classes="property_input_value_label")
                    yield Input(placeholder=f"first value", value=str(self.current_value.first), classes="property_input_value", id="property_input_first")
                
                    yield Label(f"second", classes="property_input_value_label")
                    yield Input(placeholder=f"second value", value=str(self.current_value.second), classes="property_input_value", id="property_input_second")
                
            if self.property_name == 'read_write_duration':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_optional_duration':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_two_durations':
                    yield Label(f"first", classes="property_input_value_label")
                    yield Input(placeholder=f"first value", value=str(self.current_value.first), classes="property_input_value", id="property_input_first")
                
                    yield Label(f"second", classes="property_input_value_label")
                    yield Input(placeholder=f"second value", value=str(self.current_value.second), classes="property_input_value", id="property_input_second")
                
            if self.property_name == 'read_write_binary':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_optional_binary':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_two_binaries':
                    yield Label(f"first", classes="property_input_value_label")
                    yield Input(placeholder=f"first value", value=str(self.current_value.first), classes="property_input_value", id="property_input_first")
                
                    yield Label(f"second", classes="property_input_value_label")
                    yield Input(placeholder=f"second value", value=str(self.current_value.second), classes="property_input_value", id="property_input_second")
                
            if self.property_name == 'read_write_list_of_strings':
                
                yield Input(placeholder=f"Enter new value", value=str(self.current_value) if self.current_value is not None else "", id="property_input")
                
                
            if self.property_name == 'read_write_lists':
                    yield Label(f"the_list", classes="property_input_value_label")
                    yield Input(placeholder=f"the_list value", value=str(self.current_value.the_list), classes="property_input_value", id="property_input_the_list")
                
                    yield Label(f"optionalList", classes="property_input_value_label")
                    yield Input(placeholder=f"optionalList value", value=str(self.current_value.optional_list), classes="property_input_value", id="property_input_optional_list")
                
            
            
            with Horizontal(id="property_button_container"):
                yield Button("Update", variant="primary", id="update_button")
                yield Button("Cancel", variant="error", id="cancel_button")
                yield Button("Help", variant="default", id="help_button")
    


    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Handle button presses."""
        if event.button.id == "update_button":
            try:
                if self.property_name == 'read_write_integer':
                    input_widget = self.query_one("#property_input")
                    new_read_write_integer_value = int(input_widget.value)
                    
                    self.client.read_write_integer = new_read_write_integer_value
                elif self.property_name == 'read_write_optional_integer':
                    input_widget = self.query_one("#property_input")
                    new_read_write_optional_integer_value = int(input_widget.value) if input_widget.value else None
                    
                    self.client.read_write_optional_integer = new_read_write_optional_integer_value
                elif self.property_name == 'read_write_two_integers':
                    input_widget_first = self.query_one("#property_input_first", Input)
                    # first should be ArgPrimitiveType.INTEGER
                    new_read_write_two_integers_value_first = int(input_widget_first.value)
                    
                    input_widget_second = self.query_one("#property_input_second", Input)
                    # second should be ArgPrimitiveType.INTEGER
                    new_read_write_two_integers_value_second = int(input_widget_second.value) if input_widget_second.value else None
                    
                    new_read_write_two_integers_value = ReadWriteTwoIntegersProperty(
                        first=new_read_write_two_integers_value_first,
                        second=new_read_write_two_integers_value_second,
                    )
                    
                    self.client.read_write_two_integers = new_read_write_two_integers_value
                elif self.property_name == 'read_write_string':
                    input_widget = self.query_one("#property_input")
                    new_read_write_string_value = str(input_widget.value)
                    
                    self.client.read_write_string = new_read_write_string_value
                elif self.property_name == 'read_write_optional_string':
                    input_widget = self.query_one("#property_input")
                    new_read_write_optional_string_value = str(input_widget.value) if input_widget.value else None
                    
                    self.client.read_write_optional_string = new_read_write_optional_string_value
                elif self.property_name == 'read_write_two_strings':
                    input_widget_first = self.query_one("#property_input_first", Input)
                    # first should be ArgPrimitiveType.STRING
                    new_read_write_two_strings_value_first = str(input_widget_first.value)
                    
                    input_widget_second = self.query_one("#property_input_second", Input)
                    # second should be ArgPrimitiveType.STRING
                    new_read_write_two_strings_value_second = str(input_widget_second.value) if input_widget_second.value else None
                    
                    new_read_write_two_strings_value = ReadWriteTwoStringsProperty(
                        first=new_read_write_two_strings_value_first,
                        second=new_read_write_two_strings_value_second,
                    )
                    
                    self.client.read_write_two_strings = new_read_write_two_strings_value
                elif self.property_name == 'read_write_struct':
                    input_widget = self.query_one("#property_input")
                    new_read_write_struct_value = AllTypes.model_validate_json(input_widget.value)
                    
                    self.client.read_write_struct = new_read_write_struct_value
                elif self.property_name == 'read_write_optional_struct':
                    input_widget = self.query_one("#property_input")
                    new_read_write_optional_struct_value = AllTypes.model_validate_json(input_widget.value) if input_widget.value else None
                    
                    self.client.read_write_optional_struct = new_read_write_optional_struct_value
                elif self.property_name == 'read_write_two_structs':
                    input_widget_first = self.query_one("#property_input_first", Input)
                    # first should be 
                    new_read_write_two_structs_value_first = AllTypes.model_validate_json(input_widget_first.value)
                    
                    input_widget_second = self.query_one("#property_input_second", Input)
                    # second should be 
                    new_read_write_two_structs_value_second = AllTypes.model_validate_json(input_widget_second.value) if input_widget_second.value else None
                    
                    new_read_write_two_structs_value = ReadWriteTwoStructsProperty(
                        first=new_read_write_two_structs_value_first,
                        second=new_read_write_two_structs_value_second,
                    )
                    
                    self.client.read_write_two_structs = new_read_write_two_structs_value
                elif self.property_name == 'read_write_enum':
                    input_widget = self.query_one("#property_input")
                    new_read_write_enum_value = Numbers(input_widget.value)
                    
                    self.client.read_write_enum = new_read_write_enum_value
                elif self.property_name == 'read_write_optional_enum':
                    input_widget = self.query_one("#property_input")
                    new_read_write_optional_enum_value = Numbers(input_widget.value) if input_widget.value else None
                    
                    self.client.read_write_optional_enum = new_read_write_optional_enum_value
                elif self.property_name == 'read_write_two_enums':
                    input_widget_first = self.query_one("#property_input_first", Input)
                    # first should be 
                    new_read_write_two_enums_value_first = Numbers(input_widget_first.value)
                    
                    input_widget_second = self.query_one("#property_input_second", Input)
                    # second should be 
                    new_read_write_two_enums_value_second = Numbers(input_widget_second.value) if input_widget_second.value else None
                    
                    new_read_write_two_enums_value = ReadWriteTwoEnumsProperty(
                        first=new_read_write_two_enums_value_first,
                        second=new_read_write_two_enums_value_second,
                    )
                    
                    self.client.read_write_two_enums = new_read_write_two_enums_value
                elif self.property_name == 'read_write_datetime':
                    input_widget = self.query_one("#property_input")
                    new_read_write_datetime_value = datetime.fromisoformat(input_widget.value)
                    
                    self.client.read_write_datetime = new_read_write_datetime_value
                elif self.property_name == 'read_write_optional_datetime':
                    input_widget = self.query_one("#property_input")
                    new_read_write_optional_datetime_value = datetime.fromisoformat(input_widget.value) if input_widget.value else None
                    
                    self.client.read_write_optional_datetime = new_read_write_optional_datetime_value
                elif self.property_name == 'read_write_two_datetimes':
                    input_widget_first = self.query_one("#property_input_first", Input)
                    # first should be 
                    new_read_write_two_datetimes_value_first = datetime.fromisoformat(input_widget_first.value)
                    
                    input_widget_second = self.query_one("#property_input_second", Input)
                    # second should be 
                    new_read_write_two_datetimes_value_second = datetime.fromisoformat(input_widget_second.value) if input_widget_second.value else None
                    
                    new_read_write_two_datetimes_value = ReadWriteTwoDatetimesProperty(
                        first=new_read_write_two_datetimes_value_first,
                        second=new_read_write_two_datetimes_value_second,
                    )
                    
                    self.client.read_write_two_datetimes = new_read_write_two_datetimes_value
                elif self.property_name == 'read_write_duration':
                    input_widget = self.query_one("#property_input")
                    new_read_write_duration_value = isodate.parse_duration(input_widget.value)
                    
                    self.client.read_write_duration = new_read_write_duration_value
                elif self.property_name == 'read_write_optional_duration':
                    input_widget = self.query_one("#property_input")
                    new_read_write_optional_duration_value = isodate.parse_duration(input_widget.value) if input_widget.value else None
                    
                    self.client.read_write_optional_duration = new_read_write_optional_duration_value
                elif self.property_name == 'read_write_two_durations':
                    input_widget_first = self.query_one("#property_input_first", Input)
                    # first should be 
                    new_read_write_two_durations_value_first = isodate.parse_duration(input_widget_first.value)
                    
                    input_widget_second = self.query_one("#property_input_second", Input)
                    # second should be 
                    new_read_write_two_durations_value_second = isodate.parse_duration(input_widget_second.value) if input_widget_second.value else None
                    
                    new_read_write_two_durations_value = ReadWriteTwoDurationsProperty(
                        first=new_read_write_two_durations_value_first,
                        second=new_read_write_two_durations_value_second,
                    )
                    
                    self.client.read_write_two_durations = new_read_write_two_durations_value
                elif self.property_name == 'read_write_binary':
                    input_widget = self.query_one("#property_input")
                    new_read_write_binary_value = input_widget.value.encode('utf-8')
                    
                    self.client.read_write_binary = new_read_write_binary_value
                elif self.property_name == 'read_write_optional_binary':
                    input_widget = self.query_one("#property_input")
                    new_read_write_optional_binary_value = input_widget.value.encode('utf-8') if input_widget.value else None
                    
                    self.client.read_write_optional_binary = new_read_write_optional_binary_value
                elif self.property_name == 'read_write_two_binaries':
                    input_widget_first = self.query_one("#property_input_first", Input)
                    # first should be 
                    new_read_write_two_binaries_value_first = input_widget_first.value.encode('utf-8')
                    
                    input_widget_second = self.query_one("#property_input_second", Input)
                    # second should be 
                    new_read_write_two_binaries_value_second = input_widget_second.value.encode('utf-8') if input_widget_second.value else None
                    
                    new_read_write_two_binaries_value = ReadWriteTwoBinariesProperty(
                        first=new_read_write_two_binaries_value_first,
                        second=new_read_write_two_binaries_value_second,
                    )
                    
                    self.client.read_write_two_binaries = new_read_write_two_binaries_value
                elif self.property_name == 'read_write_list_of_strings':
                    input_widget = self.query_one("#property_input")
                    new_read_write_list_of_strings_value = [str(v) for v in input_widget.value.split(',')]
                    
                    self.client.read_write_list_of_strings = new_read_write_list_of_strings_value
                elif self.property_name == 'read_write_lists':
                    input_widget_the_list = self.query_one("#property_input_the_list", Input)
                    # the_list should be 
                    new_read_write_lists_value_the_list = [Numbers(v) for v in input_widget_the_list.value.split(',')]
                    
                    input_widget_optional_list = self.query_one("#property_input_optional_list", Input)
                    # optionalList should be 
                    new_read_write_lists_value_optional_list = [datetime.fromisoformat(v) for v in input_widget_optional_list.value.split(',')] if input_widget_optional_list.value else None
                    
                    new_read_write_lists_value = ReadWriteListsProperty(
                        the_list=new_read_write_lists_value_the_list,
                        optional_list=new_read_write_lists_value_optional_list,
                    )
                    
                    self.client.read_write_lists = new_read_write_lists_value
                
            
                self.dismiss(True)
            except Exception as e:
                self.app.notify(f"Error updating property: {e}", severity="error")
        elif event.button.id == "help_button":
            if self.property_name == 'read_write_integer':
                help_text = """A read-write integer property."""
            if self.property_name == 'read_only_integer':
                help_text = """A read-only integer property."""
            if self.property_name == 'read_write_optional_integer':
                help_text = """A read-write optional integer property."""
            if self.property_name == 'read_write_two_integers':
                help_text = """A read-write property with two integer values. The second is optional."""
            if self.property_name == 'read_only_string':
                help_text = """A read-only string property."""
            if self.property_name == 'read_write_string':
                help_text = """A read-write string property."""
            if self.property_name == 'read_write_optional_string':
                help_text = """A read-write optional string property."""
            if self.property_name == 'read_write_two_strings':
                help_text = """A read-write property with two string values. The second is optional."""
            if self.property_name == 'read_write_struct':
                help_text = """A read-write struct property."""
            if self.property_name == 'read_write_optional_struct':
                help_text = """A read-write optional struct property."""
            if self.property_name == 'read_write_two_structs':
                help_text = """A read-write property with two struct values. The second is optional."""
            if self.property_name == 'read_only_enum':
                help_text = """A read-only enum property."""
            if self.property_name == 'read_write_enum':
                help_text = """A read-write enum property."""
            if self.property_name == 'read_write_optional_enum':
                help_text = """A read-write optional enum property."""
            if self.property_name == 'read_write_two_enums':
                help_text = """A read-write property with two enum values. The second is optional."""
            if self.property_name == 'read_write_datetime':
                help_text = """A read-write datetime property."""
            if self.property_name == 'read_write_optional_datetime':
                help_text = """A read-write optional datetime property."""
            if self.property_name == 'read_write_two_datetimes':
                help_text = """A read-write property with two datetime values. The second is optional."""
            if self.property_name == 'read_write_duration':
                help_text = """A read-write duration property."""
            if self.property_name == 'read_write_optional_duration':
                help_text = """A read-write optional duration property."""
            if self.property_name == 'read_write_two_durations':
                help_text = """A read-write property with two duration values. The second is optional."""
            if self.property_name == 'read_write_binary':
                help_text = """A read-write binary property."""
            if self.property_name == 'read_write_optional_binary':
                help_text = """A read-write optional binary property."""
            if self.property_name == 'read_write_two_binaries':
                help_text = """A read-write property with two binary values.  The second is optional."""
            if self.property_name == 'read_write_list_of_strings':
                help_text = """A read-write property that is a list of strings."""
            if self.property_name == 'read_write_lists':
                help_text = """A read-write property containing two lists.  The second list is optional."""
            
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
        height: 3;
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
                """ Vertical Scroll container for method inputs """
                
                 
                
                
                if self.method_name == "call_one_integer":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (PRIMITIVE)", classes="input_label") 
                    yield Input(type="integer",
                        id=f"input_input1",
                        placeholder="None",
                    )
                    
                    
                    
                 
                
                
                if self.method_name == "call_optional_integer":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (PRIMITIVE) [Optional]", classes="input_label") 
                    yield Input(type="integer",
                        id=f"input_input1",
                        placeholder="None",
                    )
                    
                    
                    
                 
                
                
                if self.method_name == "call_three_integers":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (PRIMITIVE)", classes="input_label") 
                    yield Input(type="integer",
                        id=f"input_input1",
                        placeholder="The first integer input.  The other two don't have descriptions.",
                    )
                    
                    
                    
                    
                    yield Label(f"input2 (PRIMITIVE)", classes="input_label") 
                    yield Input(type="integer",
                        id=f"input_input2",
                        placeholder="None",
                    )
                    
                    
                    
                    
                    yield Label(f"input3 (PRIMITIVE) [Optional]", classes="input_label") 
                    yield Input(type="integer",
                        id=f"input_input3",
                        placeholder="None",
                    )
                    
                    
                    
                 
                
                
                if self.method_name == "call_one_string":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (PRIMITIVE)", classes="input_label") 
                    yield Input(type="text",
                        id=f"input_input1",
                        placeholder="None",
                    )
                    
                    
                    
                 
                
                
                if self.method_name == "call_optional_string":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (PRIMITIVE) [Optional]", classes="input_label") 
                    yield Input(type="text",
                        id=f"input_input1",
                        placeholder="None",
                    )
                    
                    
                    
                 
                
                
                if self.method_name == "call_three_strings":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (PRIMITIVE)", classes="input_label") 
                    yield Input(type="text",
                        id=f"input_input1",
                        placeholder="The first string input.  The other two don't have descriptions.",
                    )
                    
                    
                    
                    
                    yield Label(f"input2 (PRIMITIVE) [Optional]", classes="input_label") 
                    yield Input(type="text",
                        id=f"input_input2",
                        placeholder="None",
                    )
                    
                    
                    
                    
                    yield Label(f"input3 (PRIMITIVE)", classes="input_label") 
                    yield Input(type="text",
                        id=f"input_input3",
                        placeholder="None",
                    )
                    
                    
                    
                 
                
                
                if self.method_name == "call_one_enum":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (ENUM)", classes="input_label")
                    options = [
                        ("one", 1),
                    
                        ("two", 2),
                    
                        ("three", 3),
                    ]
                    yield Select(options=options, id=f"input_input1", tooltip="None", allow_blank=False)
                    
                    
                 
                
                
                if self.method_name == "call_optional_enum":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (ENUM) [Optional]", classes="input_label")
                    options = [
                        ("one", 1),
                    
                        ("two", 2),
                    
                        ("three", 3),
                    ]
                    yield Select(options=options, id=f"input_input1", tooltip="None", allow_blank=True)
                    
                    
                 
                
                
                if self.method_name == "call_three_enums":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (ENUM)", classes="input_label")
                    options = [
                        ("one", 1),
                    
                        ("two", 2),
                    
                        ("three", 3),
                    ]
                    yield Select(options=options, id=f"input_input1", tooltip="The first enum input.  The other two don't have descriptions.", allow_blank=False)
                    
                    
                    
                    yield Label(f"input2 (ENUM)", classes="input_label")
                    options = [
                        ("one", 1),
                    
                        ("two", 2),
                    
                        ("three", 3),
                    ]
                    yield Select(options=options, id=f"input_input2", tooltip="None", allow_blank=False)
                    
                    
                    
                    yield Label(f"input3 (ENUM) [Optional]", classes="input_label")
                    options = [
                        ("one", 1),
                    
                        ("two", 2),
                    
                        ("three", 3),
                    ]
                    yield Select(options=options, id=f"input_input3", tooltip="None", allow_blank=True)
                    
                    
                 
                
                
                if self.method_name == "call_one_struct":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (JSON Object)", classes="input_label")
                    yield TextArea.code_editor(language="json", placeholder="None", id=f"input_input1")
                    
                    
                 
                
                
                if self.method_name == "call_optional_struct":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (JSON Object) [Optional]", classes="input_label")
                    yield TextArea.code_editor(language="json", placeholder="None", id=f"input_input1")
                    
                    
                 
                
                
                if self.method_name == "call_three_structs":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (JSON Object) [Optional]", classes="input_label")
                    yield TextArea.code_editor(language="json", placeholder="The first struct input.  The other two don't have descriptions.", id=f"input_input1")
                    
                    
                    
                    yield Label(f"input2 (JSON Object)", classes="input_label")
                    yield TextArea.code_editor(language="json", placeholder="None", id=f"input_input2")
                    
                    
                    
                    yield Label(f"input3 (JSON Object)", classes="input_label")
                    yield TextArea.code_editor(language="json", placeholder="None", id=f"input_input3")
                    
                    
                 
                
                
                if self.method_name == "call_one_date_time":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (DATETIME)", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input1")
                    
                    
                 
                
                
                if self.method_name == "call_optional_date_time":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (DATETIME) [Optional]", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input1")
                    
                    
                 
                
                
                if self.method_name == "call_three_date_times":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (DATETIME)", classes="input_label")
                    yield Input(placeholder="The first date and time input.  The other two don't have descriptions.", id=f"input_input1")
                    
                    
                    
                    yield Label(f"input2 (DATETIME)", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input2")
                    
                    
                    
                    yield Label(f"input3 (DATETIME) [Optional]", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input3")
                    
                    
                 
                
                
                if self.method_name == "call_one_duration":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (DURATION)", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input1")
                    
                    
                 
                
                
                if self.method_name == "call_optional_duration":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (DURATION) [Optional]", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input1")
                    
                    
                 
                
                
                if self.method_name == "call_three_durations":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (DURATION)", classes="input_label")
                    yield Input(placeholder="The first duration input.  The other two don't have descriptions.", id=f"input_input1")
                    
                    
                    
                    yield Label(f"input2 (DURATION)", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input2")
                    
                    
                    
                    yield Label(f"input3 (DURATION) [Optional]", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input3")
                    
                    
                 
                
                
                if self.method_name == "call_one_binary":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (BINARY)", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input1")
                    
                    
                 
                
                
                if self.method_name == "call_optional_binary":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (BINARY) [Optional]", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input1")
                    
                    
                 
                
                
                if self.method_name == "call_three_binaries":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (BINARY)", classes="input_label")
                    yield Input(placeholder="The first binary input.  The other two don't have descriptions.", id=f"input_input1")
                    
                    
                    
                    yield Label(f"input2 (BINARY)", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input2")
                    
                    
                    
                    yield Label(f"input3 (BINARY) [Optional]", classes="input_label")
                    yield Input(placeholder="None", id=f"input_input3")
                    
                    
                 
                
                
                if self.method_name == "call_one_list_of_integers":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (JSON Array)", classes="input_label")
                    yield TextArea.code_editor(language="json", placeholder="None", id=f"input_input1")
                    
                    
                 
                
                
                if self.method_name == "call_optional_list_of_floats":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (JSON Array) [Optional]", classes="input_label")
                    yield TextArea.code_editor(language="json", placeholder="None", id=f"input_input1")
                    
                    
                 
                
                
                if self.method_name == "call_two_lists":
                    """ Generate input fields for each argument of the method """
                    
                    yield Label(f"input1 (JSON Array)", classes="input_label")
                    yield TextArea.code_editor(language="json", placeholder="The first list of enums.", id=f"input_input1")
                    
                    
                    
                    yield Label(f"input2 (JSON Array) [Optional]", classes="input_label")
                    yield TextArea.code_editor(language="json", placeholder="None", id=f"input_input2")
                    
                    
                 
                 
             
            
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
            
            if self.method_name == "call_with_nothing":
                help_text = """Method that takes no arguments and returns nothing."""
            
            if self.method_name == "call_one_integer":
                help_text = """Method that takes one integer argument and returns one integer value."""
            
            if self.method_name == "call_optional_integer":
                help_text = """Method that takes one optional integer argument and returns one optional integer value."""
            
            if self.method_name == "call_three_integers":
                help_text = """Method that takes three integer arguments, the third is optional, and returns three integer values, the third is optional."""
            
            if self.method_name == "call_one_string":
                help_text = """Method that takes one string argument and returns one string value."""
            
            if self.method_name == "call_optional_string":
                help_text = """Method that takes one optional string argument and returns one optional string value."""
            
            if self.method_name == "call_three_strings":
                help_text = """Method that takes three string arguments, the 2nd is optional, and returns three string values, the 2nd is optional."""
            
            if self.method_name == "call_one_enum":
                help_text = """Method that takes one enum argument and returns one enum value."""
            
            if self.method_name == "call_optional_enum":
                help_text = """Method that takes one optional enum argument and returns one optional enum value."""
            
            if self.method_name == "call_three_enums":
                help_text = """Method that takes three enum arguments, the third is optional, and returns three enum values, the third is optional."""
            
            if self.method_name == "call_one_struct":
                help_text = """Method that takes one struct argument and returns one struct value."""
            
            if self.method_name == "call_optional_struct":
                help_text = """Method that takes one optional struct argument and returns one optional struct value."""
            
            if self.method_name == "call_three_structs":
                help_text = """Method that takes three struct arguments, the first is optional, and returns three struct values, the first is optional."""
            
            if self.method_name == "call_one_date_time":
                help_text = """Method that takes one date and time argument and returns one date and time value."""
            
            if self.method_name == "call_optional_date_time":
                help_text = """Method that takes one optional date and time argument and returns one optional date and time value."""
            
            if self.method_name == "call_three_date_times":
                help_text = """Method that takes three date and time arguments, the third is optional, and returns three date and time values, the third is optional."""
            
            if self.method_name == "call_one_duration":
                help_text = """Method that takes one duration argument and returns one duration value."""
            
            if self.method_name == "call_optional_duration":
                help_text = """Method that takes one optional duration argument and returns one optional duration value."""
            
            if self.method_name == "call_three_durations":
                help_text = """Method that takes three duration arguments, the third is optional, and returns three duration values, the third is optional."""
            
            if self.method_name == "call_one_binary":
                help_text = """Method that takes one binary argument and returns one binary value."""
            
            if self.method_name == "call_optional_binary":
                help_text = """Method that takes one optional binary argument and returns one optional binary value."""
            
            if self.method_name == "call_three_binaries":
                help_text = """Method that takes three binary arguments, the third is optional, and returns three binary values, the third is optional."""
            
            if self.method_name == "call_one_list_of_integers":
                help_text = """Method that takes one list of integers argument and returns one list of integers value."""
            
            if self.method_name == "call_optional_list_of_floats":
                help_text = """Method that takes one optional list of floats argument and returns one optional list of floats value."""
            
            if self.method_name == "call_two_lists":
                help_text = """Method that takes two list arguments, the second is optional, and returns two list values, the second is optional."""
            
            self.app.push_screen(HelpModal(help_text, title=f"Help: {self.method_name}"))

    def _call_method(self) -> None:
        """Call the method with collected inputs."""
        assert self.result_widget is not None, "result_widget must be initialized"
        logger.debug("Calling method '%s' with params: %s", self.method_name, self.params)
        try:
            # Collect inputs
            kwargs = {} # type: Dict[str, Any]
            
            
             
            
            
            if self.method_name == "call_one_integer":
                
                call_one_integer_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_one_integer_input1_value = int(call_one_integer_input1_input_widget.value)
                kwargs["input1"] = call_one_integer_input1_value
                
                
                
             
            
            
            if self.method_name == "call_optional_integer":
                
                call_optional_integer_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_optional_integer_input1_value = int(call_optional_integer_input1_input_widget.value)
                kwargs["input1"] = call_optional_integer_input1_value
                
                
                
             
            
            
            if self.method_name == "call_three_integers":
                
                call_three_integers_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_three_integers_input1_value = int(call_three_integers_input1_input_widget.value)
                kwargs["input1"] = call_three_integers_input1_value
                
                
                
                
                call_three_integers_input2_input_widget = self.query_one(f"#input_input2", Input)
                call_three_integers_input2_value = int(call_three_integers_input2_input_widget.value)
                kwargs["input2"] = call_three_integers_input2_value
                
                
                
                
                call_three_integers_input3_input_widget = self.query_one(f"#input_input3", Input)
                call_three_integers_input3_value = int(call_three_integers_input3_input_widget.value)
                kwargs["input3"] = call_three_integers_input3_value
                
                
                
             
            
            
            if self.method_name == "call_one_string":
                
                call_one_string_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_one_string_input1_value = str(call_one_string_input1_input_widget.value)
                kwargs["input1"] = call_one_string_input1_value
                
                
                
             
            
            
            if self.method_name == "call_optional_string":
                
                call_optional_string_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_optional_string_input1_value = str(call_optional_string_input1_input_widget.value)
                kwargs["input1"] = call_optional_string_input1_value
                
                
                
             
            
            
            if self.method_name == "call_three_strings":
                
                call_three_strings_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_three_strings_input1_value = str(call_three_strings_input1_input_widget.value)
                kwargs["input1"] = call_three_strings_input1_value
                
                
                
                
                call_three_strings_input2_input_widget = self.query_one(f"#input_input2", Input)
                call_three_strings_input2_value = str(call_three_strings_input2_input_widget.value)
                kwargs["input2"] = call_three_strings_input2_value
                
                
                
                
                call_three_strings_input3_input_widget = self.query_one(f"#input_input3", Input)
                call_three_strings_input3_value = str(call_three_strings_input3_input_widget.value)
                kwargs["input3"] = call_three_strings_input3_value
                
                
                
             
            
            
            if self.method_name == "call_one_enum":
                
                call_one_enum_input1_input_widget = self.query_one(f"#input_input1", Select)
                call_one_enum_input1_value = Numbers(int(call_one_enum_input1_input_widget.value))
                kwargs["input1"] = call_one_enum_input1_value
                
                
             
            
            
            if self.method_name == "call_optional_enum":
                
                call_optional_enum_input1_input_widget = self.query_one(f"#input_input1", Select)
                call_optional_enum_input1_value = Numbers(int(call_optional_enum_input1_input_widget.value))
                kwargs["input1"] = call_optional_enum_input1_value
                
                
             
            
            
            if self.method_name == "call_three_enums":
                
                call_three_enums_input1_input_widget = self.query_one(f"#input_input1", Select)
                call_three_enums_input1_value = Numbers(int(call_three_enums_input1_input_widget.value))
                kwargs["input1"] = call_three_enums_input1_value
                
                
                
                call_three_enums_input2_input_widget = self.query_one(f"#input_input2", Select)
                call_three_enums_input2_value = Numbers(int(call_three_enums_input2_input_widget.value))
                kwargs["input2"] = call_three_enums_input2_value
                
                
                
                call_three_enums_input3_input_widget = self.query_one(f"#input_input3", Select)
                call_three_enums_input3_value = Numbers(int(call_three_enums_input3_input_widget.value))
                kwargs["input3"] = call_three_enums_input3_value
                
                
             
            
            
            if self.method_name == "call_one_struct":
                
                call_one_struct_input1_input_widget = self.query_one(f"#input_input1", TextArea)
                call_one_struct_input1_widget_text_value = call_one_struct_input1_input_widget.value.strip()
                
                if not call_one_struct_input1_widget_text_value:
                    self.result_widget.update(f"[red]Error: Missing required parameter input1[/red]")
                    return
                 
                try:
                    call_one_struct_input1_value = AllTypes.model_validate_json(call_one_struct_input1_widget_text_value)
                except Exception as e:
                    self.result_widget.update(f"[red]Error parsing JSON for parameter input1: {e}[/red]")
                    return
                else:
                    kwargs["input1"] = call_one_struct_input1_value
                
                
             
            
            
            if self.method_name == "call_optional_struct":
                
                call_optional_struct_input1_input_widget = self.query_one(f"#input_input1", TextArea)
                call_optional_struct_input1_widget_text_value = call_optional_struct_input1_input_widget.value.strip()
                 
                try:
                    call_optional_struct_input1_value = AllTypes.model_validate_json(call_optional_struct_input1_widget_text_value)
                except Exception as e:
                    self.result_widget.update(f"[red]Error parsing JSON for parameter input1: {e}[/red]")
                    return
                else:
                    kwargs["input1"] = call_optional_struct_input1_value
                
                
             
            
            
            if self.method_name == "call_three_structs":
                
                call_three_structs_input1_input_widget = self.query_one(f"#input_input1", TextArea)
                call_three_structs_input1_widget_text_value = call_three_structs_input1_input_widget.value.strip()
                 
                try:
                    call_three_structs_input1_value = AllTypes.model_validate_json(call_three_structs_input1_widget_text_value)
                except Exception as e:
                    self.result_widget.update(f"[red]Error parsing JSON for parameter input1: {e}[/red]")
                    return
                else:
                    kwargs["input1"] = call_three_structs_input1_value
                
                
                
                call_three_structs_input2_input_widget = self.query_one(f"#input_input2", TextArea)
                call_three_structs_input2_widget_text_value = call_three_structs_input2_input_widget.value.strip()
                
                if not call_three_structs_input2_widget_text_value:
                    self.result_widget.update(f"[red]Error: Missing required parameter input2[/red]")
                    return
                 
                try:
                    call_three_structs_input2_value = AllTypes.model_validate_json(call_three_structs_input2_widget_text_value)
                except Exception as e:
                    self.result_widget.update(f"[red]Error parsing JSON for parameter input2: {e}[/red]")
                    return
                else:
                    kwargs["input2"] = call_three_structs_input2_value
                
                
                
                call_three_structs_input3_input_widget = self.query_one(f"#input_input3", TextArea)
                call_three_structs_input3_widget_text_value = call_three_structs_input3_input_widget.value.strip()
                
                if not call_three_structs_input3_widget_text_value:
                    self.result_widget.update(f"[red]Error: Missing required parameter input3[/red]")
                    return
                 
                try:
                    call_three_structs_input3_value = AllTypes.model_validate_json(call_three_structs_input3_widget_text_value)
                except Exception as e:
                    self.result_widget.update(f"[red]Error parsing JSON for parameter input3: {e}[/red]")
                    return
                else:
                    kwargs["input3"] = call_three_structs_input3_value
                
                
             
            
            
            if self.method_name == "call_one_date_time":
                
                call_one_date_time_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_one_date_time_input1_value = datetime.fromisoformat(call_one_date_time_input1_input_widget.value)
                kwargs["input1"] = call_one_date_time_input1_value
                
                
             
            
            
            if self.method_name == "call_optional_date_time":
                
                call_optional_date_time_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_optional_date_time_input1_value = datetime.fromisoformat(call_optional_date_time_input1_input_widget.value)
                kwargs["input1"] = call_optional_date_time_input1_value
                
                
             
            
            
            if self.method_name == "call_three_date_times":
                
                call_three_date_times_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_three_date_times_input1_value = datetime.fromisoformat(call_three_date_times_input1_input_widget.value)
                kwargs["input1"] = call_three_date_times_input1_value
                
                
                
                call_three_date_times_input2_input_widget = self.query_one(f"#input_input2", Input)
                call_three_date_times_input2_value = datetime.fromisoformat(call_three_date_times_input2_input_widget.value)
                kwargs["input2"] = call_three_date_times_input2_value
                
                
                
                call_three_date_times_input3_input_widget = self.query_one(f"#input_input3", Input)
                call_three_date_times_input3_value = datetime.fromisoformat(call_three_date_times_input3_input_widget.value)
                kwargs["input3"] = call_three_date_times_input3_value
                
                
             
            
            
            if self.method_name == "call_one_duration":
                
                call_one_duration_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_one_duration_input1_value = isodate.parse_duration(call_one_duration_input1_input_widget.value)
                kwargs["input1"] = call_one_duration_input1_value
                
                
             
            
            
            if self.method_name == "call_optional_duration":
                
                call_optional_duration_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_optional_duration_input1_value = isodate.parse_duration(call_optional_duration_input1_input_widget.value)
                kwargs["input1"] = call_optional_duration_input1_value
                
                
             
            
            
            if self.method_name == "call_three_durations":
                
                call_three_durations_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_three_durations_input1_value = isodate.parse_duration(call_three_durations_input1_input_widget.value)
                kwargs["input1"] = call_three_durations_input1_value
                
                
                
                call_three_durations_input2_input_widget = self.query_one(f"#input_input2", Input)
                call_three_durations_input2_value = isodate.parse_duration(call_three_durations_input2_input_widget.value)
                kwargs["input2"] = call_three_durations_input2_value
                
                
                
                call_three_durations_input3_input_widget = self.query_one(f"#input_input3", Input)
                call_three_durations_input3_value = isodate.parse_duration(call_three_durations_input3_input_widget.value)
                kwargs["input3"] = call_three_durations_input3_value
                
                
             
            
            
            if self.method_name == "call_one_binary":
                
                call_one_binary_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_one_binary_input1_value = call_one_binary_input1_input_widget.value.encode('utf-8')
                kwargs["input1"] = call_one_binary_input1_value
                 
                
             
            
            
            if self.method_name == "call_optional_binary":
                
                call_optional_binary_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_optional_binary_input1_value = call_optional_binary_input1_input_widget.value.encode('utf-8')
                kwargs["input1"] = call_optional_binary_input1_value
                 
                
             
            
            
            if self.method_name == "call_three_binaries":
                
                call_three_binaries_input1_input_widget = self.query_one(f"#input_input1", Input)
                call_three_binaries_input1_value = call_three_binaries_input1_input_widget.value.encode('utf-8')
                kwargs["input1"] = call_three_binaries_input1_value
                 
                
                
                call_three_binaries_input2_input_widget = self.query_one(f"#input_input2", Input)
                call_three_binaries_input2_value = call_three_binaries_input2_input_widget.value.encode('utf-8')
                kwargs["input2"] = call_three_binaries_input2_value
                 
                
                
                call_three_binaries_input3_input_widget = self.query_one(f"#input_input3", Input)
                call_three_binaries_input3_value = call_three_binaries_input3_input_widget.value.encode('utf-8')
                kwargs["input3"] = call_three_binaries_input3_value
                 
                
             
            
            
            if self.method_name == "call_one_list_of_integers":
                
                type_adapter = TypeAdapter(list)

                try:
                    call_one_list_of_integers_input1_input_widget = self.query_one(f"#input_input1", TextArea)
                    call_one_list_of_integers_input1_widget_text_value = call_one_list_of_integers_input1_input_widget.value.strip()
                    call_one_list_of_integers_input1_values = type_adapter.validate_json(call_one_list_of_integers_input1_widget_text_value)
                except Exception as e:
                    self.result_widget.update(f"[red]Error parsing JSON for parameter input1: {e}[/red]")
                    return
                else:
                    kwargs["input1"] = call_one_list_of_integers_input1_values
                
                
             
            
            
            if self.method_name == "call_optional_list_of_floats":
                
                type_adapter = TypeAdapter(list)

                try:
                    call_optional_list_of_floats_input1_input_widget = self.query_one(f"#input_input1", TextArea)
                    call_optional_list_of_floats_input1_widget_text_value = call_optional_list_of_floats_input1_input_widget.value.strip()
                    call_optional_list_of_floats_input1_values = type_adapter.validate_json(call_optional_list_of_floats_input1_widget_text_value)
                except Exception as e:
                    self.result_widget.update(f"[red]Error parsing JSON for parameter input1: {e}[/red]")
                    return
                else:
                    kwargs["input1"] = call_optional_list_of_floats_input1_values
                
                
             
            
            
            if self.method_name == "call_two_lists":
                
                type_adapter = TypeAdapter(list)

                try:
                    call_two_lists_input1_input_widget = self.query_one(f"#input_input1", TextArea)
                    call_two_lists_input1_widget_text_value = call_two_lists_input1_input_widget.value.strip()
                    call_two_lists_input1_values = type_adapter.validate_json(call_two_lists_input1_widget_text_value)
                except Exception as e:
                    self.result_widget.update(f"[red]Error parsing JSON for parameter input1: {e}[/red]")
                    return
                else:
                    kwargs["input1"] = call_two_lists_input1_values
                
                
                
                type_adapter = TypeAdapter(list)

                try:
                    call_two_lists_input2_input_widget = self.query_one(f"#input_input2", TextArea)
                    call_two_lists_input2_widget_text_value = call_two_lists_input2_input_widget.value.strip()
                    call_two_lists_input2_values = type_adapter.validate_json(call_two_lists_input2_widget_text_value)
                except Exception as e:
                    self.result_widget.update(f"[red]Error parsing JSON for parameter input2: {e}[/red]")
                    return
                else:
                    kwargs["input2"] = call_two_lists_input2_values
                
                
             
            


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
    """Screen for interacting with a connected TestableIPC server."""
    
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
        self.client = self.app.testable_client

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
            "call_with_nothing": {  }, 
        
            "call_one_integer": { "input1": int,  }, 
        
            "call_optional_integer": { "input1": Optional[int],  }, 
        
            "call_three_integers": { "input1": int, "input2": int, "input3": Optional[int],  }, 
        
            "call_one_string": { "input1": str,  }, 
        
            "call_optional_string": { "input1": Optional[str],  }, 
        
            "call_three_strings": { "input1": str, "input2": Optional[str], "input3": str,  }, 
        
            "call_one_enum": { "input1": Numbers,  }, 
        
            "call_optional_enum": { "input1": Optional[Numbers],  }, 
        
            "call_three_enums": { "input1": Numbers, "input2": Numbers, "input3": Optional[Numbers],  }, 
        
            "call_one_struct": { "input1": AllTypes,  }, 
        
            "call_optional_struct": { "input1": Optional[AllTypes],  }, 
        
            "call_three_structs": { "input1": Optional[AllTypes], "input2": AllTypes, "input3": AllTypes,  }, 
        
            "call_one_date_time": { "input1": datetime,  }, 
        
            "call_optional_date_time": { "input1": Optional[datetime],  }, 
        
            "call_three_date_times": { "input1": datetime, "input2": datetime, "input3": Optional[datetime],  }, 
        
            "call_one_duration": { "input1": timedelta,  }, 
        
            "call_optional_duration": { "input1": Optional[timedelta],  }, 
        
            "call_three_durations": { "input1": timedelta, "input2": timedelta, "input3": Optional[timedelta],  }, 
        
            "call_one_binary": { "input1": bytes,  }, 
        
            "call_optional_binary": { "input1": Optional[bytes],  }, 
        
            "call_three_binaries": { "input1": bytes, "input2": bytes, "input3": Optional[bytes],  }, 
        
            "call_one_list_of_integers": { "input1": List[int],  }, 
        
            "call_optional_list_of_floats": { "input1": Optional[List[float]],  }, 
        
            "call_two_lists": { "input1": List[Numbers], "input2": Optional[List[str]],  }, 
        
        } # type: Dict[str, Dict[str, Any]]
        
        for method_name, params in methods.items():
            btn = Button(method_name, classes="method_button")
            btn.method_name = method_name  # Store for retrieval
            btn.method_params = params  
            pane.mount(btn)
    
    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Handle method button presses."""
        if hasattr(event.button, 'method_name'):
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
        logger.debug("Registering TUI handler for signal '%s'", "empty")
        self.client.receive_empty(make_handler("empty"))
        logger.debug("Registering TUI handler for signal '%s'", "single_int")
        self.client.receive_single_int(make_handler("single_int"))
        logger.debug("Registering TUI handler for signal '%s'", "single_optional_int")
        self.client.receive_single_optional_int(make_handler("single_optional_int"))
        logger.debug("Registering TUI handler for signal '%s'", "three_integers")
        self.client.receive_three_integers(make_handler("three_integers"))
        logger.debug("Registering TUI handler for signal '%s'", "single_string")
        self.client.receive_single_string(make_handler("single_string"))
        logger.debug("Registering TUI handler for signal '%s'", "single_optional_string")
        self.client.receive_single_optional_string(make_handler("single_optional_string"))
        logger.debug("Registering TUI handler for signal '%s'", "three_strings")
        self.client.receive_three_strings(make_handler("three_strings"))
        logger.debug("Registering TUI handler for signal '%s'", "single_enum")
        self.client.receive_single_enum(make_handler("single_enum"))
        logger.debug("Registering TUI handler for signal '%s'", "single_optional_enum")
        self.client.receive_single_optional_enum(make_handler("single_optional_enum"))
        logger.debug("Registering TUI handler for signal '%s'", "three_enums")
        self.client.receive_three_enums(make_handler("three_enums"))
        logger.debug("Registering TUI handler for signal '%s'", "single_struct")
        self.client.receive_single_struct(make_handler("single_struct"))
        logger.debug("Registering TUI handler for signal '%s'", "single_optional_struct")
        self.client.receive_single_optional_struct(make_handler("single_optional_struct"))
        logger.debug("Registering TUI handler for signal '%s'", "three_structs")
        self.client.receive_three_structs(make_handler("three_structs"))
        logger.debug("Registering TUI handler for signal '%s'", "single_date_time")
        self.client.receive_single_date_time(make_handler("single_date_time"))
        logger.debug("Registering TUI handler for signal '%s'", "single_optional_datetime")
        self.client.receive_single_optional_datetime(make_handler("single_optional_datetime"))
        logger.debug("Registering TUI handler for signal '%s'", "three_date_times")
        self.client.receive_three_date_times(make_handler("three_date_times"))
        logger.debug("Registering TUI handler for signal '%s'", "single_duration")
        self.client.receive_single_duration(make_handler("single_duration"))
        logger.debug("Registering TUI handler for signal '%s'", "single_optional_duration")
        self.client.receive_single_optional_duration(make_handler("single_optional_duration"))
        logger.debug("Registering TUI handler for signal '%s'", "three_durations")
        self.client.receive_three_durations(make_handler("three_durations"))
        logger.debug("Registering TUI handler for signal '%s'", "single_binary")
        self.client.receive_single_binary(make_handler("single_binary"))
        logger.debug("Registering TUI handler for signal '%s'", "single_optional_binary")
        self.client.receive_single_optional_binary(make_handler("single_optional_binary"))
        logger.debug("Registering TUI handler for signal '%s'", "three_binaries")
        self.client.receive_three_binaries(make_handler("three_binaries"))
        logger.debug("Registering TUI handler for signal '%s'", "single_array_of_integers")
        self.client.receive_single_array_of_integers(make_handler("single_array_of_integers"))
        logger.debug("Registering TUI handler for signal '%s'", "single_optional_array_of_strings")
        self.client.receive_single_optional_array_of_strings(make_handler("single_optional_array_of_strings"))
        logger.debug("Registering TUI handler for signal '%s'", "array_of_every_type")
        self.client.receive_array_of_every_type(make_handler("array_of_every_type"))
    
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
                prop_widget.property_name = prop_name# Store for click handling
            else:
                prop_widget.add_class("readonly")
            
            pane.mount(prop_widget)
            
            
            if prop_name == "read_write_integer":
                def on_read_write_integer_updated(value: int):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value}")  # other - PRIMITIVE
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_integer_changed(on_read_write_integer_updated, call_immediately=True)
            
            elif prop_name == "read_only_integer":
                def on_read_only_integer_updated(value: int):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value}")  # other - PRIMITIVE
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_only_integer_changed(on_read_only_integer_updated, call_immediately=True)
            
            elif prop_name == "read_write_optional_integer":
                def on_read_write_optional_integer_updated(value: Optional[int]):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value}")  # other - PRIMITIVE
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_optional_integer_changed(on_read_write_optional_integer_updated, call_immediately=True)
            
            elif prop_name == "read_write_two_integers":
                def on_read_write_two_integers_updated(value: ReadWriteTwoIntegersProperty):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Multiple values
                    
                    line = (f"[bold]first[/bold]: { value.first }") # Other - PRIMITIVE
                    values.append(line)
                    
                    
                    line = (f"[bold]second[/bold]: { value.second }") # Other - PRIMITIVE
                    values.append(line)
                    
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_two_integers_changed(on_read_write_two_integers_updated, call_immediately=True)
            
            elif prop_name == "read_only_string":
                def on_read_only_string_updated(value: str):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value}")  # other - PRIMITIVE
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_only_string_changed(on_read_only_string_updated, call_immediately=True)
            
            elif prop_name == "read_write_string":
                def on_read_write_string_updated(value: str):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value}")  # other - PRIMITIVE
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_string_changed(on_read_write_string_updated, call_immediately=True)
            
            elif prop_name == "read_write_optional_string":
                def on_read_write_optional_string_updated(value: Optional[str]):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value}")  # other - PRIMITIVE
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_optional_string_changed(on_read_write_optional_string_updated, call_immediately=True)
            
            elif prop_name == "read_write_two_strings":
                def on_read_write_two_strings_updated(value: ReadWriteTwoStringsProperty):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Multiple values
                    
                    line = (f"[bold]first[/bold]: { value.first }") # Other - PRIMITIVE
                    values.append(line)
                    
                    
                    line = (f"[bold]second[/bold]: { value.second }") # Other - PRIMITIVE
                    values.append(line)
                    
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_two_strings_changed(on_read_write_two_strings_updated, call_immediately=True)
            
            elif prop_name == "read_write_struct":
                def on_read_write_struct_updated(value: AllTypes):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        
                        values.append(f"[bold]value.the_bool[/bold]: { value.the_bool }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.the_int[/bold]: { value.the_int }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.the_number[/bold]: { value.the_number }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.the_str[/bold]: { value.the_str }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.the_enum[/bold]: { value.the_enum }") # struct member ENUM
                        
                        values.append(f"[bold]value.an_entry_object[/bold]: { value.an_entry_object }") # struct member STRUCT
                        
                        values.append(f"[bold]value.date_and_time[/bold]: { value.date_and_time }") # struct member DATETIME
                        
                        values.append(f"[bold]value.time_duration[/bold]: { value.time_duration }") # struct member DURATION
                        
                        values.append(f"[bold]value.data[/bold]: { value.data!r }") # struct member BINARY
                        
                        values.append(f"[bold]value.OptionalInteger[/bold]: { value.optional_integer }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.OptionalString[/bold]: { value.optional_string }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.OptionalEnum[/bold]: { value.optional_enum }") # struct member ENUM
                        
                        values.append(f"[bold]value.optionalEntryObject[/bold]: { value.optional_entry_object }") # struct member STRUCT
                        
                        values.append(f"[bold]value.OptionalDateTime[/bold]: { value.optional_date_time }") # struct member DATETIME
                        
                        values.append(f"[bold]value.OptionalDuration[/bold]: { value.optional_duration }") # struct member DURATION
                        
                        values.append(f"[bold]value.OptionalBinary[/bold]: { value.optional_binary!r }") # struct member BINARY
                        
                        values.append(f"[bold]value.array_of_integers[/bold]: { value.array_of_integers }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_integers[/bold]: { value.optional_array_of_integers }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_strings[/bold]: { value.array_of_strings }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_strings[/bold]: { value.optional_array_of_strings }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_enums[/bold]: { value.array_of_enums }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_enums[/bold]: { value.optional_array_of_enums }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_datetimes[/bold]: { value.array_of_datetimes }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_datetimes[/bold]: { value.optional_array_of_datetimes }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_durations[/bold]: { value.array_of_durations }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_durations[/bold]: { value.optional_array_of_durations }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_binaries[/bold]: { value.array_of_binaries }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_binaries[/bold]: { value.optional_array_of_binaries }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_entry_objects[/bold]: { value.array_of_entry_objects }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_entry_objects[/bold]: { value.optional_array_of_entry_objects }") # struct member ARRAY
                        
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_struct_changed(on_read_write_struct_updated, call_immediately=True)
            
            elif prop_name == "read_write_optional_struct":
                def on_read_write_optional_struct_updated(value: Optional[AllTypes]):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        
                        values.append(f"[bold]value.the_bool[/bold]: { value.the_bool }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.the_int[/bold]: { value.the_int }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.the_number[/bold]: { value.the_number }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.the_str[/bold]: { value.the_str }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.the_enum[/bold]: { value.the_enum }") # struct member ENUM
                        
                        values.append(f"[bold]value.an_entry_object[/bold]: { value.an_entry_object }") # struct member STRUCT
                        
                        values.append(f"[bold]value.date_and_time[/bold]: { value.date_and_time }") # struct member DATETIME
                        
                        values.append(f"[bold]value.time_duration[/bold]: { value.time_duration }") # struct member DURATION
                        
                        values.append(f"[bold]value.data[/bold]: { value.data!r }") # struct member BINARY
                        
                        values.append(f"[bold]value.OptionalInteger[/bold]: { value.optional_integer }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.OptionalString[/bold]: { value.optional_string }") # struct member PRIMITIVE
                        
                        values.append(f"[bold]value.OptionalEnum[/bold]: { value.optional_enum }") # struct member ENUM
                        
                        values.append(f"[bold]value.optionalEntryObject[/bold]: { value.optional_entry_object }") # struct member STRUCT
                        
                        values.append(f"[bold]value.OptionalDateTime[/bold]: { value.optional_date_time }") # struct member DATETIME
                        
                        values.append(f"[bold]value.OptionalDuration[/bold]: { value.optional_duration }") # struct member DURATION
                        
                        values.append(f"[bold]value.OptionalBinary[/bold]: { value.optional_binary!r }") # struct member BINARY
                        
                        values.append(f"[bold]value.array_of_integers[/bold]: { value.array_of_integers }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_integers[/bold]: { value.optional_array_of_integers }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_strings[/bold]: { value.array_of_strings }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_strings[/bold]: { value.optional_array_of_strings }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_enums[/bold]: { value.array_of_enums }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_enums[/bold]: { value.optional_array_of_enums }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_datetimes[/bold]: { value.array_of_datetimes }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_datetimes[/bold]: { value.optional_array_of_datetimes }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_durations[/bold]: { value.array_of_durations }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_durations[/bold]: { value.optional_array_of_durations }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_binaries[/bold]: { value.array_of_binaries }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_binaries[/bold]: { value.optional_array_of_binaries }") # struct member ARRAY
                        
                        values.append(f"[bold]value.array_of_entry_objects[/bold]: { value.array_of_entry_objects }") # struct member ARRAY
                        
                        values.append(f"[bold]value.optional_array_of_entry_objects[/bold]: { value.optional_array_of_entry_objects }") # struct member ARRAY
                        
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_optional_struct_changed(on_read_write_optional_struct_updated, call_immediately=True)
            
            elif prop_name == "read_write_two_structs":
                def on_read_write_two_structs_updated(value: ReadWriteTwoStructsProperty):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Multiple values
                    
                    if value.first is None:
                        values.append(f"[bold]first[/bold]: None")
                    else:
                        values.append(f"[bold]first.the_bool[/bold]: { value.first.the_bool }") # Struct Member
                        values.append(f"[bold]first.the_int[/bold]: { value.first.the_int }") # Struct Member
                        values.append(f"[bold]first.the_number[/bold]: { value.first.the_number }") # Struct Member
                        values.append(f"[bold]first.the_str[/bold]: { value.first.the_str }") # Struct Member
                        values.append(f"[bold]first.the_enum[/bold]: { value.first.the_enum }") # Struct Member
                        values.append(f"[bold]first.an_entry_object[/bold]: { value.first.an_entry_object }") # Struct Member
                        values.append(f"[bold]first.date_and_time[/bold]: { value.first.date_and_time }") # Struct Member
                        values.append(f"[bold]first.time_duration[/bold]: { value.first.time_duration }") # Struct Member
                        values.append(f"[bold]first.data[/bold]: { value.first.data!r }") # Struct Member
                        values.append(f"[bold]first.OptionalInteger[/bold]: { value.first.optional_integer }") # Struct Member
                        values.append(f"[bold]first.OptionalString[/bold]: { value.first.optional_string }") # Struct Member
                        values.append(f"[bold]first.OptionalEnum[/bold]: { value.first.optional_enum }") # Struct Member
                        values.append(f"[bold]first.optionalEntryObject[/bold]: { value.first.optional_entry_object }") # Struct Member
                        values.append(f"[bold]first.OptionalDateTime[/bold]: { value.first.optional_date_time }") # Struct Member
                        values.append(f"[bold]first.OptionalDuration[/bold]: { value.first.optional_duration }") # Struct Member
                        values.append(f"[bold]first.OptionalBinary[/bold]: { value.first.optional_binary!r }") # Struct Member
                        values.append(f"[bold]first.array_of_integers[/bold]: { value.first.array_of_integers }") # Struct Member
                        values.append(f"[bold]first.optional_array_of_integers[/bold]: { value.first.optional_array_of_integers }") # Struct Member
                        values.append(f"[bold]first.array_of_strings[/bold]: { value.first.array_of_strings }") # Struct Member
                        values.append(f"[bold]first.optional_array_of_strings[/bold]: { value.first.optional_array_of_strings }") # Struct Member
                        values.append(f"[bold]first.array_of_enums[/bold]: { value.first.array_of_enums }") # Struct Member
                        values.append(f"[bold]first.optional_array_of_enums[/bold]: { value.first.optional_array_of_enums }") # Struct Member
                        values.append(f"[bold]first.array_of_datetimes[/bold]: { value.first.array_of_datetimes }") # Struct Member
                        values.append(f"[bold]first.optional_array_of_datetimes[/bold]: { value.first.optional_array_of_datetimes }") # Struct Member
                        values.append(f"[bold]first.array_of_durations[/bold]: { value.first.array_of_durations }") # Struct Member
                        values.append(f"[bold]first.optional_array_of_durations[/bold]: { value.first.optional_array_of_durations }") # Struct Member
                        values.append(f"[bold]first.array_of_binaries[/bold]: { value.first.array_of_binaries }") # Struct Member
                        values.append(f"[bold]first.optional_array_of_binaries[/bold]: { value.first.optional_array_of_binaries }") # Struct Member
                        values.append(f"[bold]first.array_of_entry_objects[/bold]: { value.first.array_of_entry_objects }") # Struct Member
                        values.append(f"[bold]first.optional_array_of_entry_objects[/bold]: { value.first.optional_array_of_entry_objects }") # Struct Member
                        
                    
                    
                    if value.second is None:
                        values.append(f"[bold]second[/bold]: None")
                    else:
                        values.append(f"[bold]second.the_bool[/bold]: { value.second.the_bool }") # Struct Member
                        values.append(f"[bold]second.the_int[/bold]: { value.second.the_int }") # Struct Member
                        values.append(f"[bold]second.the_number[/bold]: { value.second.the_number }") # Struct Member
                        values.append(f"[bold]second.the_str[/bold]: { value.second.the_str }") # Struct Member
                        values.append(f"[bold]second.the_enum[/bold]: { value.second.the_enum }") # Struct Member
                        values.append(f"[bold]second.an_entry_object[/bold]: { value.second.an_entry_object }") # Struct Member
                        values.append(f"[bold]second.date_and_time[/bold]: { value.second.date_and_time }") # Struct Member
                        values.append(f"[bold]second.time_duration[/bold]: { value.second.time_duration }") # Struct Member
                        values.append(f"[bold]second.data[/bold]: { value.second.data!r }") # Struct Member
                        values.append(f"[bold]second.OptionalInteger[/bold]: { value.second.optional_integer }") # Struct Member
                        values.append(f"[bold]second.OptionalString[/bold]: { value.second.optional_string }") # Struct Member
                        values.append(f"[bold]second.OptionalEnum[/bold]: { value.second.optional_enum }") # Struct Member
                        values.append(f"[bold]second.optionalEntryObject[/bold]: { value.second.optional_entry_object }") # Struct Member
                        values.append(f"[bold]second.OptionalDateTime[/bold]: { value.second.optional_date_time }") # Struct Member
                        values.append(f"[bold]second.OptionalDuration[/bold]: { value.second.optional_duration }") # Struct Member
                        values.append(f"[bold]second.OptionalBinary[/bold]: { value.second.optional_binary!r }") # Struct Member
                        values.append(f"[bold]second.array_of_integers[/bold]: { value.second.array_of_integers }") # Struct Member
                        values.append(f"[bold]second.optional_array_of_integers[/bold]: { value.second.optional_array_of_integers }") # Struct Member
                        values.append(f"[bold]second.array_of_strings[/bold]: { value.second.array_of_strings }") # Struct Member
                        values.append(f"[bold]second.optional_array_of_strings[/bold]: { value.second.optional_array_of_strings }") # Struct Member
                        values.append(f"[bold]second.array_of_enums[/bold]: { value.second.array_of_enums }") # Struct Member
                        values.append(f"[bold]second.optional_array_of_enums[/bold]: { value.second.optional_array_of_enums }") # Struct Member
                        values.append(f"[bold]second.array_of_datetimes[/bold]: { value.second.array_of_datetimes }") # Struct Member
                        values.append(f"[bold]second.optional_array_of_datetimes[/bold]: { value.second.optional_array_of_datetimes }") # Struct Member
                        values.append(f"[bold]second.array_of_durations[/bold]: { value.second.array_of_durations }") # Struct Member
                        values.append(f"[bold]second.optional_array_of_durations[/bold]: { value.second.optional_array_of_durations }") # Struct Member
                        values.append(f"[bold]second.array_of_binaries[/bold]: { value.second.array_of_binaries }") # Struct Member
                        values.append(f"[bold]second.optional_array_of_binaries[/bold]: { value.second.optional_array_of_binaries }") # Struct Member
                        values.append(f"[bold]second.array_of_entry_objects[/bold]: { value.second.array_of_entry_objects }") # Struct Member
                        values.append(f"[bold]second.optional_array_of_entry_objects[/bold]: { value.second.optional_array_of_entry_objects }") # Struct Member
                        
                    
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_two_structs_changed(on_read_write_two_structs_updated, call_immediately=True)
            
            elif prop_name == "read_only_enum":
                def on_read_only_enum_updated(value: Numbers):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value.name if value else 'None'} ({value.value if value else 'None'})") # enum
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_only_enum_changed(on_read_only_enum_updated, call_immediately=True)
            
            elif prop_name == "read_write_enum":
                def on_read_write_enum_updated(value: Numbers):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value.name if value else 'None'} ({value.value if value else 'None'})") # enum
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_enum_changed(on_read_write_enum_updated, call_immediately=True)
            
            elif prop_name == "read_write_optional_enum":
                def on_read_write_optional_enum_updated(value: Optional[Numbers]):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value.name if value else 'None'} ({value.value if value else 'None'})") # enum
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_optional_enum_changed(on_read_write_optional_enum_updated, call_immediately=True)
            
            elif prop_name == "read_write_two_enums":
                def on_read_write_two_enums_updated(value: ReadWriteTwoEnumsProperty):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Multiple values
                    
                    line = (f"[bold]first[/bold]: { value.first.name if value.first else 'None' } ({ value.first.value if value.first else 'None' })") # Enum
                    values.append(line)
                    
                    
                    line = (f"[bold]second[/bold]: { value.second.name if value.second else 'None' } ({ value.second.value if value.second else 'None' })") # Enum
                    values.append(line)
                    
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_two_enums_changed(on_read_write_two_enums_updated, call_immediately=True)
            
            elif prop_name == "read_write_datetime":
                def on_read_write_datetime_updated(value: datetime):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value.isoformat() if value else 'None'}") # datetime
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_datetime_changed(on_read_write_datetime_updated, call_immediately=True)
            
            elif prop_name == "read_write_optional_datetime":
                def on_read_write_optional_datetime_updated(value: Optional[datetime]):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value.isoformat() if value else 'None'}") # datetime
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_optional_datetime_changed(on_read_write_optional_datetime_updated, call_immediately=True)
            
            elif prop_name == "read_write_two_datetimes":
                def on_read_write_two_datetimes_updated(value: ReadWriteTwoDatetimesProperty):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Multiple values
                    
                    line = (f"[bold]first[/bold]: { value.first.isoformat() if value.first else 'None' }") # Datetime
                    values.append(line)
                    
                    
                    line = (f"[bold]second[/bold]: { value.second.isoformat() if value.second else 'None' }") # Datetime
                    values.append(line)
                    
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_two_datetimes_changed(on_read_write_two_datetimes_updated, call_immediately=True)
            
            elif prop_name == "read_write_duration":
                def on_read_write_duration_updated(value: timedelta):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value}")  # other - DURATION
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_duration_changed(on_read_write_duration_updated, call_immediately=True)
            
            elif prop_name == "read_write_optional_duration":
                def on_read_write_optional_duration_updated(value: Optional[timedelta]):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value}")  # other - DURATION
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_optional_duration_changed(on_read_write_optional_duration_updated, call_immediately=True)
            
            elif prop_name == "read_write_two_durations":
                def on_read_write_two_durations_updated(value: ReadWriteTwoDurationsProperty):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Multiple values
                    
                    line = (f"[bold]first[/bold]: { value.first }") # Other - DURATION
                    values.append(line)
                    
                    
                    line = (f"[bold]second[/bold]: { value.second }") # Other - DURATION
                    values.append(line)
                    
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_two_durations_changed(on_read_write_two_durations_updated, call_immediately=True)
            
            elif prop_name == "read_write_binary":
                def on_read_write_binary_updated(value: bytes):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value!r}")  # binary
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_binary_changed(on_read_write_binary_updated, call_immediately=True)
            
            elif prop_name == "read_write_optional_binary":
                def on_read_write_optional_binary_updated(value: Optional[bytes]):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        values.append(f"{value!r}")  # binary
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_optional_binary_changed(on_read_write_optional_binary_updated, call_immediately=True)
            
            elif prop_name == "read_write_two_binaries":
                def on_read_write_two_binaries_updated(value: ReadWriteTwoBinariesProperty):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Multiple values
                    
                    values.append(f"[bold]first[/bold]: { value.first!r }") # Binary
                    
                    
                    values.append(f"[bold]second[/bold]: { value.second!r }") # Binary
                    
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_two_binaries_changed(on_read_write_two_binaries_updated, call_immediately=True)
            
            elif prop_name == "read_write_list_of_strings":
                def on_read_write_list_of_strings_updated(value: List[str]):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Single value
                    if value is None:
                        values.append("None")
                    else:
                        
                        for i, item in enumerate(value or []):
                            values.append(f"[bold]value[{i}][/bold]: { item }") # array item
                        
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_list_of_strings_changed(on_read_write_list_of_strings_updated, call_immediately=True)
            
            elif prop_name == "read_write_lists":
                def on_read_write_lists_updated(value: ReadWriteListsProperty):
                    prop_widget.current_value = value
                    
                    values = []
                    
                    # Multiple values
                    
                    for i, the_list_item in enumerate(value.the_list or []):
                        values.append(f"[bold]the_list[{i}][/bold]: { the_list_item  }") # Array Item
                    
                    
                    for i, optional_list_item in enumerate(value.optional_list or []):
                        values.append(f"[bold]optionalList[{i}][/bold]: { optional_list_item  }") # Array Item
                    
                    
                    value_str = "\n".join(values)
                    
                    # Update the widget
                    prop_widget.update(
                        f"[bold cyan]{prop_name}[/bold cyan]\n{value_str}"
                    )

                # Register the handler with call_immediately=True
                assert self.client is not None, "Client must be initialized"
                self.client.read_write_lists_changed(on_read_write_lists_updated, call_immediately=True)
            

        # Register all properties
        register_property("read_write_integer", "read_write_integer_changed", is_writable=True)
        register_property("read_only_integer", "read_only_integer_changed", is_writable=False)
        register_property("read_write_optional_integer", "read_write_optional_integer_changed", is_writable=True)
        register_property("read_write_two_integers", "read_write_two_integers_changed", is_writable=True)
        register_property("read_only_string", "read_only_string_changed", is_writable=False)
        register_property("read_write_string", "read_write_string_changed", is_writable=True)
        register_property("read_write_optional_string", "read_write_optional_string_changed", is_writable=True)
        register_property("read_write_two_strings", "read_write_two_strings_changed", is_writable=True)
        register_property("read_write_struct", "read_write_struct_changed", is_writable=True)
        register_property("read_write_optional_struct", "read_write_optional_struct_changed", is_writable=True)
        register_property("read_write_two_structs", "read_write_two_structs_changed", is_writable=True)
        register_property("read_only_enum", "read_only_enum_changed", is_writable=False)
        register_property("read_write_enum", "read_write_enum_changed", is_writable=True)
        register_property("read_write_optional_enum", "read_write_optional_enum_changed", is_writable=True)
        register_property("read_write_two_enums", "read_write_two_enums_changed", is_writable=True)
        register_property("read_write_datetime", "read_write_datetime_changed", is_writable=True)
        register_property("read_write_optional_datetime", "read_write_optional_datetime_changed", is_writable=True)
        register_property("read_write_two_datetimes", "read_write_two_datetimes_changed", is_writable=True)
        register_property("read_write_duration", "read_write_duration_changed", is_writable=True)
        register_property("read_write_optional_duration", "read_write_optional_duration_changed", is_writable=True)
        register_property("read_write_two_durations", "read_write_two_durations_changed", is_writable=True)
        register_property("read_write_binary", "read_write_binary_changed", is_writable=True)
        register_property("read_write_optional_binary", "read_write_optional_binary_changed", is_writable=True)
        register_property("read_write_two_binaries", "read_write_two_binaries_changed", is_writable=True)
        register_property("read_write_list_of_strings", "read_write_list_of_strings_changed", is_writable=True)
        register_property("read_write_lists", "read_write_lists_changed", is_writable=True)
    
    
    def on_click(self, event) -> None:
        """Handle clicks on property widgets."""
        assert self.client is not None, "Client must be initialized"

        # Check if the clicked widget is a writable property
        widget = event.widget
        if hasattr(widget, 'property_name') and hasattr(widget, 'current_value'):
            property_name = widget.property_name
            current_value = widget.current_value
            
            # Open the edit modal
            modal = PropertyEditModal(property_name, current_value, self.client)
            self.app.push_screen(modal)
    
    def action_back_to_discovery(self) -> None:
        """Navigate back to the discovery screen."""
        self.app.pop_screen()