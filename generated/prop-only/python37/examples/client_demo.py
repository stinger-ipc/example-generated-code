import os
import signal
from time import sleep
import concurrent.futures as futures
from typing import Optional, Union, List
from datetime import datetime, timedelta, UTC
from pyqttier import Mqtt5Connection, MqttTransportType, MqttTransport
from proponlyipc.lwt import StingerPresence
from proponlyipc.client import PropOnlyClient, PropOnlyClientBuilder, PropOnlyClientDiscoverer
from proponlyipc.interface_types import *
import threading


def request_loop(client: PropOnlyClient):
    """Example request loop that runs in a separate thread."""
    sleep(30)
    while True:

        client.home_address = Address(street="apples", city="apples", state="apples", postal_code="apples", country=Country.USA)

        client.favorite_country = Country.USA

        sleep(10)


if __name__ == "__main__":
    import sys

    if len(sys.argv) > 1 and sys.argv[1] == "--help":
        print("Usage: client_demo.py [--help]")
        print("Connects to an MQTT broker and demonstrates prop-only client functionality.")
        print("Environment variables:")
        print("  MQTT_HOSTNAME - MQTT broker hostname (default: localhost)")
        print("  MQTT_PORT - MQTT broker port (default: 1883)")
        sys.exit(0)

    transport = MqttTransport(MqttTransportType.TCP, os.environ.get("MQTT_HOSTNAME", "localhost"), int(os.environ.get("MQTT_PORT", 1883)))
    lwt = StingerPresence(
        client_id="py-server-demo",
    )
    conn = Mqtt5Connection(transport, client_id=os.environ.get("CLIENT_ID", "py-server-demo"), lwt=lwt)

    client_builder = PropOnlyClientBuilder()

    @client_builder.home_address_updated
    def print_new_home_address_value(value: Address):
        """ """
        print(f"Property 'home_address' has been updated to: {value}")

    @client_builder.favorite_country_updated
    def print_new_favorite_country_value(value: Country):
        """ """
        print(f"Property 'favorite_country' has been updated to: {value}")

    discovery = PropOnlyClientDiscoverer(conn, client_builder)
    fut_client = discovery.get_singleton_client()
    try:
        client = fut_client.result(10)
    except futures.TimeoutError:
        print("Timed out waiting for a service to appear")
        exit(1)

    sleep(2)

    threading.Thread(target=request_loop, args=(client,), daemon=True).start()

    print("Ctrl-C will stop the program.")
    signal.pause()
