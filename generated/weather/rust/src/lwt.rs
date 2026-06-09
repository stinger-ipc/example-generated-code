use bytes::Bytes;
use jsonpath_rust::JsonPath;
use std::str::FromStr;
use std::sync::Arc;
use std::time::Duration;
use stinger_mqtt_trait::availability_trait::AvailabilityHelper;
use stinger_mqtt_trait::message::{MqttMessage, QoS};

/// A Stinger implementation of [`AvailabilityHelper`] that configures the publishing of JSON online/offline
/// availability messages to `client/{client_id}/online`.
///
/// # Example
///
/// ```
/// use weather_ipc::lwt::StingerAvailability;
/// use stinger_mqtt_trait::availability_trait::AvailabilityHelper;
///
/// let helper = StingerAvailability::new("example", );
/// assert_eq!(helper.get_client_online_message().topic, "client/example/online");
/// assert_eq!(helper.get_republish_interval(), None);
/// ```
pub struct StingerAvailability {
    online_message: MqttMessage,
    offline_message: MqttMessage,
    republish_interval: Option<Duration>,
}

impl StingerAvailability {
    /// Create a new `StingerAvailability` for the given client ID.
    ///
    /// The topic will be `client/{client_id}/online`. Online and offline payloads are
    /// `{"online":true}` and `{"online":false}` respectively,
    /// published at QoS 1
    /// with retain enabled and no periodic republish interval.
    pub fn new(client_id: impl Into<String>) -> Arc<dyn AvailabilityHelper + Send + Sync> {
        let client_id = client_id.into();

        let topic = format!("client/{}/online", client_id,);
        let online_message = MqttMessage::simple(
            topic.clone(),
            QoS::AtLeastOnce,
            true,
            Bytes::from_static(b"{\"online\":true}"),
        );
        let offline_message = MqttMessage::simple(
            topic,
            QoS::AtLeastOnce,
            true,
            Bytes::from_static(b"{\"online\":false}"),
        );
        Arc::new(Self {
            online_message,
            offline_message,
            republish_interval: None,
        })
    }
}

impl AvailabilityHelper for StingerAvailability {
    fn get_client_online_message(&self) -> MqttMessage {
        self.online_message.clone()
    }

    fn get_client_offline_message(&self) -> MqttMessage {
        self.offline_message.clone()
    }

    fn get_online_json_path(&self) -> JsonPath {
        JsonPath::from_str("$.online").expect("hardcoded path is valid")
    }

    fn get_republish_interval(&self) -> Option<Duration> {
        self.republish_interval
    }
}

pub fn client_id_creator(client_id: impl Into<String>) -> String {
    let client_id = client_id.into();

    format!("{client_id}", client_id = client_id,)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_online_message() {
        let helper = StingerAvailability::new("example");
        let msg = helper.get_client_online_message();
        assert_eq!(msg.topic, "client/example/online");
        assert_eq!(msg.payload, Bytes::from_static(b"{\"online\":true}"));
        assert_eq!(msg.qos, QoS::AtLeastOnce);
        assert!(msg.retain);
    }

    #[test]
    fn test_offline_message() {
        let helper = StingerAvailability::new("example");
        let msg = helper.get_client_offline_message();
        assert_eq!(msg.topic, "client/example/online");
        assert_eq!(msg.payload, Bytes::from_static(b"{\"online\":false}"));
        assert_eq!(msg.qos, QoS::AtLeastOnce);
        assert!(msg.retain);
    }

    #[test]
    fn test_online_json_path_returns_expected_path() {
        let helper = StingerAvailability::new("example");
        let expected = JsonPath::from_str("$.online").unwrap();
        assert_eq!(helper.get_online_json_path(), expected);
    }

    #[test]
    fn test_online_json_path_matches_online_payload() {
        use jsonpath_rust::JsonPathValue;

        let helper = StingerAvailability::new("example");
        let path = helper.get_online_json_path();
        let payload: serde_json::Value =
            serde_json::from_slice(&helper.get_client_online_message().payload)
                .expect("valid JSON payload");
        let results = path.find_slice(&payload);
        assert_eq!(results.len(), 1);
        assert!(
            matches!(&results[0], JsonPathValue::Slice(v, _) if *v == &serde_json::Value::Bool(true))
        );
    }

    #[test]
    fn test_online_json_path_matches_offline_payload() {
        use jsonpath_rust::JsonPathValue;

        let helper = StingerAvailability::new("example");
        let path = helper.get_online_json_path();
        let payload: serde_json::Value =
            serde_json::from_slice(&helper.get_client_offline_message().payload)
                .expect("valid JSON payload");
        let results = path.find_slice(&payload);
        assert_eq!(results.len(), 1);
        assert!(
            matches!(&results[0], JsonPathValue::Slice(v, _) if *v == &serde_json::Value::Bool(false))
        );
    }
}
