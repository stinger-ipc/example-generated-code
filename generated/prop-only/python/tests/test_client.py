"""
Tests for prop-only client.
"""

import pytest
import sys
from pathlib import Path
from datetime import datetime, timedelta, UTC

from proponlyipc.client import PropOnlyClient, DiscoveredInstance
from proponlyipc.property import PropOnlyInitialPropertyValues
from proponlyipc.interface_types import *
from pyqttier.mock import MockConnection
import json
from typing import Dict, Any


def to_jsonified_dict(model: BaseModel) -> Dict[str, Any]:
    """Convert a Pydantic model to a JSON-serializable dict."""
    json_str = model.model_dump_json(by_alias=True)
    return json.loads(json_str)


@pytest.fixture
def initial_property_values():
    initial_property_values = PropOnlyInitialPropertyValues(
        home_address=Address(street="apples", city="apples", state="apples", postal_code="apples", country=Country.USA),
        favorite_country=Country.USA,
    )
    return initial_property_values


@pytest.fixture
def mock_connection():
    """Fixture providing a mock MQTT connection."""
    conn = MockConnection()
    return conn


@pytest.fixture
def client(mock_connection, initial_property_values):
    """Fixture providing a prop-only client with mocked connection."""
    mock_discovered_instance = DiscoveredInstance(
        instance_id="x",
        initial_property_values=initial_property_values,
        info=PropOnlyInterfaceInfo(
            instance="x",
            connection_topic="x/prop-only/x/interface",
            timestamp="2024-01-01T00:00:00Z",
            prefix="x",
        ),
    )
    client = PropOnlyClient(
        connection=mock_connection,
        instance_info=mock_discovered_instance,
    )
    return client


class TestClient:
    """Tests for client initialization."""

    def test_client_initializes(self, client):
        """Test that client initializes successfully."""
        assert client is not None, "Client failed to initialize"
        assert client.service_id == "x", "Client service_id does not match expected value"


class TestClientProperties:

    def test_client_properties_initialization(self, client, initial_property_values):
        """Test that client properties are initialized correctly."""

        assert hasattr(client, "home_address"), "Client missing property 'home_address'"
        assert client.home_address is not None, "Property 'home_address' not initialized properly"
        assert client.home_address == initial_property_values.home_address, "Property 'home_address' value does not match expected value"

        assert hasattr(client, "favorite_country"), "Client missing property 'favorite_country'"
        assert client.favorite_country is not None, "Property 'favorite_country' not initialized properly"
        assert client.favorite_country == initial_property_values.favorite_country, "Property 'favorite_country' value does not match expected value"

    def test_home_address_setter(self, client):
        new_home_address_value = Address(street="apples", city="apples", state="apples", postal_code="apples", country=Country.USA)

        client.home_address = new_home_address_value

    def test_favorite_country_setter(self, client):
        new_favorite_country_value = Country.USA

        client.favorite_country = new_favorite_country_value
