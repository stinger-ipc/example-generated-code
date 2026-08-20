use crate::payloads::MethodReturnCode;
use bytes::Bytes;
use serde::Serialize;
use stinger_mqtt_trait::message::{MqttMessage, MqttMessageBuilder, QoS};
#[cfg(feature = "client")]
use uuid::Uuid;

#[cfg(any(feature = "server", test))]
pub fn interface_online(
    topic: &str,
    payload: &crate::interface::InterfaceInfo,
    message_expiry_seconds: u32,
) -> Result<MqttMessage, MethodReturnCode> {
    let mut builder = MqttMessageBuilder::default();
    builder
        .topic(topic)
        .qos(QoS::AtLeastOnce)
        .retain(true)
        .message_expiry_interval(message_expiry_seconds);
    match builder.object_payload(payload) {
        Ok(_) => {}
        Err(e) => return Err(MethodReturnCode::ServerSerializationError(e.to_string())),
    }
    let msg = builder
        .build()
        .map_err(|e| MethodReturnCode::PayloadError(e.to_string()))?;
    Ok(msg)
}

#[cfg(feature = "server")]
pub fn signal<T: Serialize>(topic: &str, payload: &T) -> Result<MqttMessage, MethodReturnCode> {
    let msg = MqttMessageBuilder::default()
        .topic(topic)
        .object_payload(payload)
        .map_err(|e| MethodReturnCode::ServerSerializationError(e.to_string()))?
        .qos(QoS::ExactlyOnce)
        .retain(false)
        .build()
        .map_err(|e| MethodReturnCode::PayloadError(e.to_string()))?;
    Ok(msg)
}
