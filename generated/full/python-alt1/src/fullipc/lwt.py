from pyqttier.lwt import OnlinePresence
from pyqttier.message import Message
import json

def StingerPresence(client_id: str,) -> OnlinePresence:
    """Helper function to create an OnlinePresence instance for StingerIPC clients."""

    def _create_message(is_online: bool) -> Message:
        return Message(
            topic=f"client/{client_id}/online",
            payload=json.dumps({"online": is_online}).encode("utf-8"),
            qos=1,
            retain=True,
            content_type="application/json",
        )

    return OnlinePresence(
        topic=f"client/{client_id}/online",
        online=_create_message(is_online=True),
        offline=_create_message(is_online=False),
    )

def ClientIdCreator(client_id: str,) -> str:
    """Helper function to generate a client ID string for StingerIPC clients based on the provided topic template parameters."""
    return f"{client_id}".format(client_id=client_id, )