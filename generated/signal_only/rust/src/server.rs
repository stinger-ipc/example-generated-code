//! Server module for SignalOnly IPC
//!
//! This module is only available when the "server" feature is enabled.

/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

This is the Server for the SignalOnly interface.

LICENSE: This generated code is not subject to any license restrictions from the generator itself.
TODO: Get license text from stinger file
*/

#[allow(unused_imports)]
use crate::payloads::{MethodReturnCode, *};
use bytes::Bytes;
use std::collections::HashMap;
use strfmt::strfmt;
use tokio::sync::oneshot;

use std::sync::{Arc, Mutex};
use tokio::sync::Mutex as AsyncMutex;

use std::future::Future;
use std::pin::Pin;
use stinger_mqtt_trait::message::{MqttMessage, QoS};
use stinger_mqtt_trait::{Mqtt5PubSub, Mqtt5PubSubError, MqttPublishSuccess};
#[allow(unused_imports)]
use stinger_rwlock_watch::{CommitResult, WriteRequestLockWatch};
use tokio::task::JoinError;
type SentMessageFuture = Pin<Box<dyn Future<Output = Result<(), MethodReturnCode>> + Send>>;
use crate::message;
#[cfg(feature = "metrics")]
use serde::Serialize;
#[cfg(feature = "server")]
#[allow(unused_imports)]
use tracing::{debug, error, info, warn};

#[cfg(feature = "metrics")]
#[derive(Debug, Serialize)]
pub struct SignalOnlyServerMetrics {}

#[cfg(feature = "metrics")]
impl Default for SignalOnlyServerMetrics {
    fn default() -> Self {
        SignalOnlyServerMetrics {}
    }
}

#[derive(Clone)]
pub struct SignalOnlyServer<C: Mqtt5PubSub> {
    mqtt_client: C,

    /// Copy of MQTT Client ID
    #[allow(dead_code)]
    pub client_id: String,

    pub instance_id: String,
    topic_param_prefix: String,
    #[cfg(feature = "metrics")]
    metrics: Arc<AsyncMutex<SignalOnlyServerMetrics>>,
}

impl<C: Mqtt5PubSub + Clone + Send> SignalOnlyServer<C> {
    pub async fn new(mut connection: C, instance_id: String, prefix: String) -> Self {
        #[cfg(feature = "metrics")]
        let mut metrics = SignalOnlyServerMetrics::default();

        SignalOnlyServer {
            mqtt_client: connection.clone(),

            client_id: connection.get_client_id(),
            instance_id,
            topic_param_prefix: prefix,
            #[cfg(feature = "metrics")]
            metrics: Arc::new(AsyncMutex::new(metrics)),
        }
    }

    #[cfg(feature = "metrics")]
    pub fn get_metrics(&self) -> Arc<AsyncMutex<SignalOnlyServerMetrics>> {
        self.metrics.clone()
    }

    /// Converts a oneshot channel receiver into a future.
    async fn oneshot_to_future(
        ch: oneshot::Receiver<Result<MqttPublishSuccess, Mqtt5PubSubError>>,
    ) -> SentMessageFuture {
        Box::pin(async move {
            let chan_result = ch.await;
            match chan_result {
                Ok(transferred_result) => match transferred_result {
                    Ok(MqttPublishSuccess::Acknowledged) => Ok(()),
                    Ok(MqttPublishSuccess::Completed) => Ok(()),
                    Ok(MqttPublishSuccess::Sent) => Ok(()),
                    Ok(MqttPublishSuccess::Queued) => Ok(()),
                    Err(e) => Err(MethodReturnCode::TransportError(format!(
                        "MQTT publish error: {:?}",
                        e
                    ))),
                },
                Err(e) => Err(MethodReturnCode::TransportError(format!(
                    "MQTT publish oneshot receive error: {:?}",
                    e
                ))),
            }
        })
    }

    async fn wrap_return_code_in_future(rc: MethodReturnCode) -> SentMessageFuture {
        Box::pin(async move {
            match rc {
                MethodReturnCode::Success(_) => Ok(()),
                _ => Err(rc),
            }
        })
    }
    /// Emits the anotherSignal signal with the given arguments.
    pub async fn emit_another_signal(
        &mut self,
        one: f32,
        two: bool,
        three: String,
    ) -> SentMessageFuture {
        let data = AnotherSignalSignalPayload { one, two, three };
        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("signal_name".to_string(), "anotherSignal".to_string()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);
        let topic = strfmt(
            "{prefix}/SignalOnly/{service_id}/signal/anotherSignal",
            &topic_param_map,
        )
        .unwrap();
        let msg = message::signal(&topic, &data).unwrap();
        let mut publisher = self.mqtt_client.clone();
        let ch = publisher.publish_noblock(msg).await;
        Self::oneshot_to_future(ch).await
    }

    /// Emits the anotherSignal signal with the given arguments, but this is a fire-and-forget version.
    pub fn emit_another_signal_nowait(
        &mut self,
        one: f32,
        two: bool,
        three: String,
    ) -> std::result::Result<MqttPublishSuccess, Mqtt5PubSubError> {
        let data = AnotherSignalSignalPayload { one, two, three };
        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("signal_name".to_string(), "anotherSignal".to_string()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);
        let topic = strfmt(
            "{prefix}/SignalOnly/{service_id}/signal/anotherSignal",
            &topic_param_map,
        )
        .unwrap();
        let msg = message::signal(&topic, &data).unwrap();
        let mut publisher = self.mqtt_client.clone();
        publisher.publish_nowait(msg)
    }
    /// Emits the bark signal with the given arguments.
    pub async fn emit_bark(&mut self, word: String) -> SentMessageFuture {
        let data = BarkSignalPayload { word };
        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("signal_name".to_string(), "bark".to_string()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);
        let topic = strfmt(
            "{prefix}/SignalOnly/{service_id}/signal/bark",
            &topic_param_map,
        )
        .unwrap();
        let msg = message::signal(&topic, &data).unwrap();
        let mut publisher = self.mqtt_client.clone();
        let ch = publisher.publish_noblock(msg).await;
        Self::oneshot_to_future(ch).await
    }

    /// Emits the bark signal with the given arguments, but this is a fire-and-forget version.
    pub fn emit_bark_nowait(
        &mut self,
        word: String,
    ) -> std::result::Result<MqttPublishSuccess, Mqtt5PubSubError> {
        let data = BarkSignalPayload { word };
        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("signal_name".to_string(), "bark".to_string()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);
        let topic = strfmt(
            "{prefix}/SignalOnly/{service_id}/signal/bark",
            &topic_param_map,
        )
        .unwrap();
        let msg = message::signal(&topic, &data).unwrap();
        let mut publisher = self.mqtt_client.clone();
        publisher.publish_nowait(msg)
    }
    /// Emits the maybe_number signal with the given arguments.
    pub async fn emit_maybe_number(&mut self, number: Option<i32>) -> SentMessageFuture {
        let data = MaybeNumberSignalPayload { number };
        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("signal_name".to_string(), "maybe_number".to_string()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);
        let topic = strfmt(
            "{prefix}/SignalOnly/{service_id}/signal/maybe_number",
            &topic_param_map,
        )
        .unwrap();
        let msg = message::signal(&topic, &data).unwrap();
        let mut publisher = self.mqtt_client.clone();
        let ch = publisher.publish_noblock(msg).await;
        Self::oneshot_to_future(ch).await
    }

    /// Emits the maybe_number signal with the given arguments, but this is a fire-and-forget version.
    pub fn emit_maybe_number_nowait(
        &mut self,
        number: Option<i32>,
    ) -> std::result::Result<MqttPublishSuccess, Mqtt5PubSubError> {
        let data = MaybeNumberSignalPayload { number };
        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("signal_name".to_string(), "maybe_number".to_string()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);
        let topic = strfmt(
            "{prefix}/SignalOnly/{service_id}/signal/maybe_number",
            &topic_param_map,
        )
        .unwrap();
        let msg = message::signal(&topic, &data).unwrap();
        let mut publisher = self.mqtt_client.clone();
        publisher.publish_nowait(msg)
    }
    /// Emits the maybe_name signal with the given arguments.
    pub async fn emit_maybe_name(&mut self, name: Option<String>) -> SentMessageFuture {
        let data = MaybeNameSignalPayload { name };
        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("signal_name".to_string(), "maybe_name".to_string()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);
        let topic = strfmt(
            "{prefix}/SignalOnly/{service_id}/signal/maybe_name",
            &topic_param_map,
        )
        .unwrap();
        let msg = message::signal(&topic, &data).unwrap();
        let mut publisher = self.mqtt_client.clone();
        let ch = publisher.publish_noblock(msg).await;
        Self::oneshot_to_future(ch).await
    }

    /// Emits the maybe_name signal with the given arguments, but this is a fire-and-forget version.
    pub fn emit_maybe_name_nowait(
        &mut self,
        name: Option<String>,
    ) -> std::result::Result<MqttPublishSuccess, Mqtt5PubSubError> {
        let data = MaybeNameSignalPayload { name };
        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("signal_name".to_string(), "maybe_name".to_string()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);
        let topic = strfmt(
            "{prefix}/SignalOnly/{service_id}/signal/maybe_name",
            &topic_param_map,
        )
        .unwrap();
        let msg = message::signal(&topic, &data).unwrap();
        let mut publisher = self.mqtt_client.clone();
        publisher.publish_nowait(msg)
    }
    /// Emits the now signal with the given arguments.
    pub async fn emit_now(
        &mut self,
        timestamp: chrono::DateTime<chrono::Utc>,
    ) -> SentMessageFuture {
        let data = NowSignalPayload { timestamp };
        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("signal_name".to_string(), "now".to_string()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);
        let topic = strfmt(
            "{prefix}/SignalOnly/{service_id}/signal/now",
            &topic_param_map,
        )
        .unwrap();
        let msg = message::signal(&topic, &data).unwrap();
        let mut publisher = self.mqtt_client.clone();
        let ch = publisher.publish_noblock(msg).await;
        Self::oneshot_to_future(ch).await
    }

    /// Emits the now signal with the given arguments, but this is a fire-and-forget version.
    pub fn emit_now_nowait(
        &mut self,
        timestamp: chrono::DateTime<chrono::Utc>,
    ) -> std::result::Result<MqttPublishSuccess, Mqtt5PubSubError> {
        let data = NowSignalPayload { timestamp };
        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("signal_name".to_string(), "now".to_string()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);
        let topic = strfmt(
            "{prefix}/SignalOnly/{service_id}/signal/now",
            &topic_param_map,
        )
        .unwrap();
        let msg = message::signal(&topic, &data).unwrap();
        let mut publisher = self.mqtt_client.clone();
        publisher.publish_nowait(msg)
    }

    /// Starts the tasks that process messages received.
    /// In the task, it loops over messages received from the rx side of the message_receiver channel.
    /// Based on the subscription id of the received message, it will call a function to handle the
    /// received message.
    pub async fn run_loop(&mut self) -> Result<(), JoinError>
    where
        C: 'static,
    {
        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);

        // Spawn a task to periodically publish interface info.
        let mut interface_publisher = self.mqtt_client.clone();
        let instance_id = self.instance_id.clone();
        let topic_param_map_for_info = topic_param_map.clone();
        tokio::spawn(async move {
            let mut interval = tokio::time::interval(std::time::Duration::from_secs(120));
            loop {
                interval.tick().await;
                let topic = strfmt(
                    "{prefix}/SignalOnly/{service_id}/interface",
                    &topic_param_map_for_info,
                )
                .unwrap();
                let info = crate::interface::InterfaceInfoBuilder::default()
                    .interface_name("SignalOnly".to_string())
                    .title("SignalOnly".to_string())
                    .version("0.0.1".to_string())
                    .instance(instance_id.clone())
                    .connection_topic(topic.clone())
                    .prefix(topic_param_map_for_info.get("prefix").unwrap().to_string())
                    .build()
                    .unwrap();
                let msg = message::interface_online(&topic, &info, 144 /*seconds*/).unwrap();
                let _ = interface_publisher.publish(msg).await;
            }
        });

        warn!("Server receive loop completed. Exiting run_loop.");
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use stinger_mqtt_trait::mock::MockClient;

    use tracing_subscriber::EnvFilter;

    #[tokio::test]
    async fn mock_server() {
        let _ = tracing_subscriber::fmt()
            .with_test_writer()
            .with_env_filter(EnvFilter::new("signal_only_ipc=debug"))
            .try_init();

        let service_id = "N".to_string();
        let client_id = "mock_client".to_string();

        let mut mock_mqtt = MockClient::new(client_id.clone());

        let server =
            SignalOnlyServer::new(mock_mqtt.clone(), service_id.clone(), "prefix".to_string())
                .await;

        // Start the server connection loop in a separate task.
        let mut looping_server = server.clone();
        let _loop_join_handle = tokio::spawn(async move {
            let _conn_loop = looping_server.run_loop().await;
        });

        let mut topic_param_map = HashMap::from([
            ("interface_name".to_string(), "SignalOnly".to_string()),
            ("service_id".to_string(), service_id.clone()),
            ("client_id".to_string(), client_id.clone()),
            ("property_name".to_string(), "prop_xyz".to_string()),
            ("prefix".to_string(), "prefix".to_string()),
        ]);

        tokio::time::sleep(tokio::time::Duration::from_millis(100)).await;
        let received_messages = mock_mqtt.published_messages();
        for (i, msg) in received_messages.iter().enumerate() {
            println!("Initial message {}: {:?}", i, msg);
        }
        assert_eq!(received_messages.len(), 1 + 0); // 1 for interface online, plus 1 for each property initial publish

        // Publish a property update message for each property
    }
}
