import signal
from time import sleep
import concurrent.futures as futures
from typing import Optional, Union, List
from datetime import datetime, timedelta, UTC
from pyqttier import Mqtt5Connection, MqttTransportType, MqttTransport
from proponlyipc.client import PropOnlyClient, PropOnlyClientBuilder, PropOnlyClientDiscoverer
from proponlyipc.interface_types import *
import threading

client_builder = PropOnlyClientBuilder()


class SuperAwesomeDoerOfThings:

    def __init__(self, label: str, connection: Mqtt5Connection):
        self.counter = 0
        self.label = label
        discovery = PropOnlyClientDiscoverer(connection, client_builder, build_binding=self)  # The build binding will bind all @client_builder decorated methods to this instance.
        self.client = discovery.get_singleton_client().result()

        threading.Thread(target=self.request_loop, daemon=True).start()

    @client_builder.home_address_updated
    def print_new_home_address_value(self, value: Address):
        print(f"{self.label}-{self.counter} printing signal 'home_address' : value={value}")

    @client_builder.favorite_country_updated
    def print_new_favorite_country_value(self, value: Country):
        print(f"{self.label}-{self.counter} printing signal 'favorite_country' : value={value}")

    def request_loop(self):
        """Example request loop that runs in a separate thread."""
        sleep(30)
        while True:

            self.client.home_address = Address(street="apples", city="apples", state="apples", postal_code="apples", country=Country.USA)

            self.client.favorite_country = Country.USA

            sleep(10)


if __name__ == "__main__":

    transport = MqttTransport(MqttTransportType.TCP, "localhost", 1883)
    conn = Mqtt5Connection(transport)

    doer1 = SuperAwesomeDoerOfThings("Doer1", conn)

    print("Ctrl-C will stop the program.")
    signal.pause()
