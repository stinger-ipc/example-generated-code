"""
Tests for prop-only server.
"""

import pytest
from unittest.mock import Mock, patch, MagicMock
import sys
from pathlib import Path
from datetime import datetime, timedelta, timezone

UTC = timezone.utc

from proponlyipc.server import PropOnlyServer
from proponlyipc.property import PropOnlyInitialPropertyValues
from proponlyipc.interface_types import *
from stinger_python_utils.return_codes import MethodReturnCode
from pyqttier.mock import MockConnection
from pyqttier.message import Message
import json
from pydantic import BaseModel
from typing import Any, Dict


def to_jsonified_dict(model: BaseModel) -> Dict[str, Any]:
    """Convert a Pydantic model to a JSON-serializable dict."""
    json_str = model.model_dump_json(by_alias=True)
    return json.loads(json_str)


class PropOnlyServerSetup:

    def __init__(self):
        self.initial_property_values = self.get_initial_property_values()

    def get_initial_property_values(self) -> PropOnlyInitialPropertyValues:
        initial_property_values = PropOnlyInitialPropertyValues(
            home_address=Address(street="apples", city="apples", state="apples", postal_code="apples", country=Country.USA),
            favorite_country=Country.USA,
        )
        return initial_property_values

    def create_server(self, mock_connection) -> PropOnlyServer:
        server = PropOnlyServer(
            mock_connection,
            "x",
            self.initial_property_values,
            "x",
        )
        return server


@pytest.fixture
def server_setup():
    setup = PropOnlyServerSetup()
    return setup


@pytest.fixture
def initial_property_values(server_setup):
    return server_setup.initial_property_values


@pytest.fixture
def mock_connection():
    """Fixture providing a mock MQTT connection."""
    conn = MockConnection()
    return conn


@pytest.fixture
def server(server_setup, mock_connection):
    server = server_setup.create_server(mock_connection)
    yield server
    server.shutdown(timeout=0.01)


class TestPropOnlyServer:

    def test_server_initializes(self, server):
        """Test that client initializes successfully."""
        assert server is not None, "server failed to initialize"
        assert server.instance_id == "x", "Server instance_id does not match expected value"


class TestPropOnlyServerProperties:

    def test_server_home_address_property_initialization(self, server, initial_property_values):
        """Test that the home_address server property is initialized correctly."""
        assert hasattr(server, "home_address"), "Server missing property 'home_address'"
        assert server.home_address is not None, "Property 'home_address' not initialized properly"
        assert server.home_address == initial_property_values.home_address, "Property 'home_address' value does not match expected value"

    def test_home_address_property_publish(self, server, mock_connection, initial_property_values):
        """Test that setting the 'home_address' property publishes the correct message."""
        mock_connection.clear_published_messages()
        server.publish_home_address_value()

        published_list = mock_connection.find_published("+/prop-only/+/property/home_address/value")
        assert len(published_list) == 1, f"No message was published for property 'home_address'.  Messages: {mock_connection.published_messages}"

        msg = published_list[0]
        expected_topic = "x/prop-only/x/property/home_address/value"
        assert msg.topic == expected_topic, f"Published topic '{msg.topic}' does not match expected '{expected_topic}'"

        # Verify payload
        expected_obj = HomeAddressProperty(address=initial_property_values.home_address)
        expected_dict = to_jsonified_dict(expected_obj)
        payload_dict = json.loads(msg.payload.decode("utf-8"))
        assert payload_dict == expected_dict, f"Published payload '{payload_dict}' does not match expected '{expected_dict}'"

    def test_home_address_property_receive(self, server, mock_connection):
        """Test that receiving a property update for 'home_address' updates the server property and calls callbacks."""
        received_data = None

        def callback(address):
            nonlocal received_data
            received_data = {
                "address": address,
            }

        server.on_home_address_updated(callback)

        # Create and simulate receiving a property update message
        prop_data = {
            "address": Address(street="example", city="example", state="example", postal_code="example", country=Country.USA),
        }
        prop_obj = HomeAddressProperty(**prop_data)  # type: ignore[arg-type]
        response_topic = "client/test/response"
        correlation_data = b"123-41"
        incoming_msg = Message(
            topic="x/prop-only/x/property/home_address/update",
            payload=prop_obj.model_dump_json(by_alias=True).encode("utf-8"),
            qos=1,
            retain=False,
            response_topic=response_topic,
            correlation_data=correlation_data,
            content_type="application/json",
            user_properties={"PropertyVersion": str(server._property_home_address.version)},
        )
        mock_connection.simulate_message(incoming_msg)

        # Verify that server property was updated
        assert received_data is not None, "Callback for property 'home_address' was not called"

        # Expect a reply sent back acknowledging the update
        published_list = mock_connection.find_published(response_topic)
        assert len(published_list) == 1, f"No response message was published for property 'home_address'."
        resp = published_list[0]
        assert resp.user_properties.get("ReturnCode") == str(MethodReturnCode.SUCCESS.value), f"Expected SUCCESS return code, got '{resp.user_properties.get('ReturnCode')}'"
        assert resp.correlation_data == correlation_data, "Correlation data in response does not match expected value"

    def test_home_address_property_receive_out_of_sync(self, server, mock_connection):
        """Test that receiving a property update for 'home_address' updates the server property and calls callbacks."""
        received_data = None

        def callback(address):
            nonlocal received_data
            received_data = {
                "address": address,
            }

        server.on_home_address_updated(callback)

        # Create and simulate receiving a property update message
        prop_data = {
            "address": Address(street="example", city="example", state="example", postal_code="example", country=Country.USA),
        }
        prop_obj = HomeAddressProperty(**prop_data)  # type: ignore[arg-type]
        response_topic = "client/test/response"
        correlation_data = b"12345-67"
        incoming_msg = Message(
            topic="x/prop-only/x/property/home_address/update",
            payload=prop_obj.model_dump_json(by_alias=True).encode("utf-8"),
            qos=1,
            retain=False,
            content_type="application/json",
            response_topic=response_topic,
            correlation_data=correlation_data,
            user_properties={"PropertyVersion": "67"},
        )
        mock_connection.simulate_message(incoming_msg)

        assert received_data is None, "Callback for property 'home_address' was called despite out-of-sync version"

        # Check for error message published
        published_list = mock_connection.find_published(response_topic)
        assert len(published_list) == 1, f"No response/error message was published for out-of-sync property 'home_address'."

        resp = published_list[0]
        assert resp.user_properties.get("ReturnCode") == str(MethodReturnCode.OUT_OF_SYNC.value), f"Expected OUT_OF_SYNC return code, got '{resp.user_properties.get('ReturnCode')}'"
        assert resp.correlation_data == correlation_data, "Correlation data in response does not match expected value"

    def test_home_address_property_receive_nonsense_payload(self, server, mock_connection):
        """Test that receiving a property update for 'home_address' updates the server property and calls callbacks."""
        received_data = None

        def callback(address):
            nonlocal received_data
            received_data = {
                "address": address,
            }

        server.on_home_address_updated(callback)

        # Create and simulate receiving a property update message that has nonsensical payload
        response_topic = "client/test/response"
        correlation_data = b"12345-67"
        incoming_msg = Message(
            topic="x/prop-only/x/property/home_address/update",
            payload=b"adsfaf{this is not json}12|false",
            qos=1,
            retain=False,
            content_type="application/json",
            response_topic=response_topic,
            correlation_data=correlation_data,
            user_properties={"PropertyVersion": str(server._property_home_address.version)},
        )
        mock_connection.simulate_message(incoming_msg)

        assert received_data is None, "Callback for property 'home_address' was called despite bad payload"

        # Check for error message published
        published_list = mock_connection.find_published(response_topic)
        assert len(published_list) == 1, f"No response/error message was published for bad payload request."

        resp = published_list[0]
        assert resp.user_properties.get("ReturnCode") == str(
            MethodReturnCode.SERVER_DESERIALIZATION_ERROR.value
        ), f"Expected SERVER_DESERIALIZATION_ERROR return code, got '{resp.user_properties.get('ReturnCode')}'"
        assert resp.correlation_data == correlation_data, "Correlation data in response does not match expected value"

    def test_home_address_property_receive_wrong_payload(self, server, mock_connection):
        """Test that receiving a property update for 'home_address' updates the server property and calls callbacks."""
        received_data = None

        def callback(address):
            nonlocal received_data
            received_data = {
                "address": address,
            }

        server.on_home_address_updated(callback)

        # Create and simulate receiving a property update message that has nonsensical payload
        response_topic = "client/test/response"
        correlation_data = b"12345-67"
        incoming_msg = Message(
            topic="x/prop-only/x/property/home_address/update",
            payload=b'{"wrong_field": 123, "another_wrong": false}',
            qos=1,
            retain=False,
            content_type="application/json",
            response_topic=response_topic,
            correlation_data=correlation_data,
            user_properties={"PropertyVersion": str(server._property_home_address.version)},
        )
        mock_connection.simulate_message(incoming_msg)

        assert received_data is None, "Callback for property 'home_address' was called despite wrong payload"

        # Check for error message published
        published_list = mock_connection.find_published(response_topic)
        assert len(published_list) == 1, f"No response/error message was published for wrong payload request."

        resp = published_list[0]
        assert resp.user_properties.get("ReturnCode") == str(
            MethodReturnCode.SERVER_DESERIALIZATION_ERROR.value
        ), f"Expected SERVER_DESERIALIZATION_ERROR return code, got '{resp.user_properties.get('ReturnCode')}'"
        assert resp.correlation_data == correlation_data, "Correlation data in response does not match expected value"

    def test_server_favorite_country_property_initialization(self, server, initial_property_values):
        """Test that the favorite_country server property is initialized correctly."""
        assert hasattr(server, "favorite_country"), "Server missing property 'favorite_country'"
        assert server.favorite_country is not None, "Property 'favorite_country' not initialized properly"
        assert server.favorite_country == initial_property_values.favorite_country, "Property 'favorite_country' value does not match expected value"

    def test_favorite_country_property_publish(self, server, mock_connection, initial_property_values):
        """Test that setting the 'favorite_country' property publishes the correct message."""
        mock_connection.clear_published_messages()
        server.publish_favorite_country_value()

        published_list = mock_connection.find_published("+/prop-only/+/property/favorite_country/value")
        assert len(published_list) == 1, f"No message was published for property 'favorite_country'.  Messages: {mock_connection.published_messages}"

        msg = published_list[0]
        expected_topic = "x/prop-only/x/property/favorite_country/value"
        assert msg.topic == expected_topic, f"Published topic '{msg.topic}' does not match expected '{expected_topic}'"

        # Verify payload
        expected_obj = FavoriteCountryProperty(country=initial_property_values.favorite_country)
        expected_dict = to_jsonified_dict(expected_obj)
        payload_dict = json.loads(msg.payload.decode("utf-8"))
        assert payload_dict == expected_dict, f"Published payload '{payload_dict}' does not match expected '{expected_dict}'"

    def test_favorite_country_property_receive(self, server, mock_connection):
        """Test that receiving a property update for 'favorite_country' updates the server property and calls callbacks."""
        received_data = None

        def callback(country):
            nonlocal received_data
            received_data = {
                "country": country,
            }

        server.on_favorite_country_updated(callback)

        # Create and simulate receiving a property update message
        prop_data = {
            "country": Country.USA,
        }
        prop_obj = FavoriteCountryProperty(**prop_data)  # type: ignore[arg-type]
        response_topic = "client/test/response"
        correlation_data = b"123-41"
        incoming_msg = Message(
            topic="x/prop-only/x/property/favorite_country/update",
            payload=prop_obj.model_dump_json(by_alias=True).encode("utf-8"),
            qos=1,
            retain=False,
            response_topic=response_topic,
            correlation_data=correlation_data,
            content_type="application/json",
            user_properties={"PropertyVersion": str(server._property_favorite_country.version)},
        )
        mock_connection.simulate_message(incoming_msg)

        # Verify that server property was updated
        assert received_data is not None, "Callback for property 'favorite_country' was not called"

        # Expect a reply sent back acknowledging the update
        published_list = mock_connection.find_published(response_topic)
        assert len(published_list) == 1, f"No response message was published for property 'favorite_country'."
        resp = published_list[0]
        assert resp.user_properties.get("ReturnCode") == str(MethodReturnCode.SUCCESS.value), f"Expected SUCCESS return code, got '{resp.user_properties.get('ReturnCode')}'"
        assert resp.correlation_data == correlation_data, "Correlation data in response does not match expected value"

    def test_favorite_country_property_receive_out_of_sync(self, server, mock_connection):
        """Test that receiving a property update for 'favorite_country' updates the server property and calls callbacks."""
        received_data = None

        def callback(country):
            nonlocal received_data
            received_data = {
                "country": country,
            }

        server.on_favorite_country_updated(callback)

        # Create and simulate receiving a property update message
        prop_data = {
            "country": Country.USA,
        }
        prop_obj = FavoriteCountryProperty(**prop_data)  # type: ignore[arg-type]
        response_topic = "client/test/response"
        correlation_data = b"12345-67"
        incoming_msg = Message(
            topic="x/prop-only/x/property/favorite_country/update",
            payload=prop_obj.model_dump_json(by_alias=True).encode("utf-8"),
            qos=1,
            retain=False,
            content_type="application/json",
            response_topic=response_topic,
            correlation_data=correlation_data,
            user_properties={"PropertyVersion": "67"},
        )
        mock_connection.simulate_message(incoming_msg)

        assert received_data is None, "Callback for property 'favorite_country' was called despite out-of-sync version"

        # Check for error message published
        published_list = mock_connection.find_published(response_topic)
        assert len(published_list) == 1, f"No response/error message was published for out-of-sync property 'favorite_country'."

        resp = published_list[0]
        assert resp.user_properties.get("ReturnCode") == str(MethodReturnCode.OUT_OF_SYNC.value), f"Expected OUT_OF_SYNC return code, got '{resp.user_properties.get('ReturnCode')}'"
        assert resp.correlation_data == correlation_data, "Correlation data in response does not match expected value"

    def test_favorite_country_property_receive_nonsense_payload(self, server, mock_connection):
        """Test that receiving a property update for 'favorite_country' updates the server property and calls callbacks."""
        received_data = None

        def callback(country):
            nonlocal received_data
            received_data = {
                "country": country,
            }

        server.on_favorite_country_updated(callback)

        # Create and simulate receiving a property update message that has nonsensical payload
        response_topic = "client/test/response"
        correlation_data = b"12345-67"
        incoming_msg = Message(
            topic="x/prop-only/x/property/favorite_country/update",
            payload=b"adsfaf{this is not json}12|false",
            qos=1,
            retain=False,
            content_type="application/json",
            response_topic=response_topic,
            correlation_data=correlation_data,
            user_properties={"PropertyVersion": str(server._property_favorite_country.version)},
        )
        mock_connection.simulate_message(incoming_msg)

        assert received_data is None, "Callback for property 'favorite_country' was called despite bad payload"

        # Check for error message published
        published_list = mock_connection.find_published(response_topic)
        assert len(published_list) == 1, f"No response/error message was published for bad payload request."

        resp = published_list[0]
        assert resp.user_properties.get("ReturnCode") == str(
            MethodReturnCode.SERVER_DESERIALIZATION_ERROR.value
        ), f"Expected SERVER_DESERIALIZATION_ERROR return code, got '{resp.user_properties.get('ReturnCode')}'"
        assert resp.correlation_data == correlation_data, "Correlation data in response does not match expected value"

    def test_favorite_country_property_receive_wrong_payload(self, server, mock_connection):
        """Test that receiving a property update for 'favorite_country' updates the server property and calls callbacks."""
        received_data = None

        def callback(country):
            nonlocal received_data
            received_data = {
                "country": country,
            }

        server.on_favorite_country_updated(callback)

        # Create and simulate receiving a property update message that has nonsensical payload
        response_topic = "client/test/response"
        correlation_data = b"12345-67"
        incoming_msg = Message(
            topic="x/prop-only/x/property/favorite_country/update",
            payload=b'{"wrong_field": 123, "another_wrong": false}',
            qos=1,
            retain=False,
            content_type="application/json",
            response_topic=response_topic,
            correlation_data=correlation_data,
            user_properties={"PropertyVersion": str(server._property_favorite_country.version)},
        )
        mock_connection.simulate_message(incoming_msg)

        assert received_data is None, "Callback for property 'favorite_country' was called despite wrong payload"

        # Check for error message published
        published_list = mock_connection.find_published(response_topic)
        assert len(published_list) == 1, f"No response/error message was published for wrong payload request."

        resp = published_list[0]
        assert resp.user_properties.get("ReturnCode") == str(
            MethodReturnCode.SERVER_DESERIALIZATION_ERROR.value
        ), f"Expected SERVER_DESERIALIZATION_ERROR return code, got '{resp.user_properties.get('ReturnCode')}'"
        assert resp.correlation_data == correlation_data, "Correlation data in response does not match expected value"
