//! Server module for prop-only IPC
//!
//! This module is only available when the "server" feature is enabled.

/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

This is the Server for the prop-only interface.

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

use serde_json;
use tokio::sync::{broadcast, watch};

use crate::property::PropOnlyInitialPropertyValues;
use std::sync::atomic::{AtomicU32, Ordering};
use stinger_rwlock_watch::RwLockWatch;

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

/// This struct is used to store all the MQTTv5 subscription ids
/// for the subscriptions the client will make.
#[derive(Clone, Debug)]
struct PropOnlyServerSubscriptionIds {
    home_address_property_update: u32,

    favorite_country_property_update: u32,
}

#[derive(Clone)]
struct PropOnlyProperties {
    pub home_address: Arc<RwLockWatch<Address>>,
    home_address_version: Arc<AtomicU32>,
    pub favorite_country: Arc<RwLockWatch<Country>>,
    favorite_country_version: Arc<AtomicU32>,
}

#[cfg(feature = "metrics")]
#[derive(Debug, Serialize)]
pub struct PropOnlyServerMetrics {
    pub initial_property_publish_time: std::time::Duration,
}

#[cfg(feature = "metrics")]
impl Default for PropOnlyServerMetrics {
    fn default() -> Self {
        PropOnlyServerMetrics {
            initial_property_publish_time: std::time::Duration::from_secs(0),
        }
    }
}

#[derive(Clone)]
pub struct PropOnlyServer<C: Mqtt5PubSub> {
    mqtt_client: C,

    /// Temporarily holds the receiver for the broadcast channel.  The Receiver will be moved
    /// to a process loop when it is needed.  MQTT messages will be received with this.
    msg_streamer_rx: Arc<Mutex<Option<broadcast::Receiver<MqttMessage>>>>,

    /// The Sender side of MQTT messages that are received from the broker.  This tx
    /// side is cloned for each subscription made.
    #[allow(dead_code)]
    msg_streamer_tx: broadcast::Sender<MqttMessage>,

    /// Struct contains all the properties.
    properties: PropOnlyProperties,

    /// Subscription IDs for all the subscriptions this makes.
    subscription_ids: PropOnlyServerSubscriptionIds,

    /// Copy of MQTT Client ID
    #[allow(dead_code)]
    pub client_id: String,

    pub instance_id: String,
    topic_param_prefix: String,
    #[cfg(feature = "metrics")]
    metrics: Arc<AsyncMutex<PropOnlyServerMetrics>>,
}

impl<C: Mqtt5PubSub + Clone + Send> PropOnlyServer<C> {
    pub async fn new(
        mut connection: C,

        instance_id: String,
        initial_property_values: PropOnlyInitialPropertyValues,
        prefix: String,
    ) -> Self {
        #[cfg(feature = "metrics")]
        let mut metrics = PropOnlyServerMetrics::default();

        // Create a channel for messages to get from the Mqtt5PubSub object to this PropOnlyServer object.
        // The Connection object uses a clone of the tx side of the channel.
        let (message_received_tx, message_received_rx) = broadcast::channel::<MqttMessage>(64);

        let topic_param_map = HashMap::from([
            ("interface_name".to_string(), "prop-only".to_string()),
            ("service_id".to_string(), instance_id.to_string()),
            (
                "client_id".to_string(),
                connection.get_client_id().to_string(),
            ),
            ("prefix".to_string(), prefix.clone()),
        ]);

        // Create method handler struct

        let subscription_id_home_address_property_update = connection
            .subscribe(
                strfmt(
                    "{prefix}/prop-only/{service_id}/property/home_address/update",
                    &topic_param_map,
                )
                .unwrap(),
                QoS::AtLeastOnce,
                message_received_tx.clone(),
            )
            .await;
        let subscription_id_home_address_property_update =
            subscription_id_home_address_property_update.unwrap_or(u32::MAX);

        let subscription_id_favorite_country_property_update = connection
            .subscribe(
                strfmt(
                    "{prefix}/prop-only/{service_id}/property/favorite_country/update",
                    &topic_param_map,
                )
                .unwrap(),
                QoS::AtLeastOnce,
                message_received_tx.clone(),
            )
            .await;
        let subscription_id_favorite_country_property_update =
            subscription_id_favorite_country_property_update.unwrap_or(u32::MAX);

        // Create structure for subscription ids.
        let sub_ids = PropOnlyServerSubscriptionIds {
            home_address_property_update: subscription_id_home_address_property_update,
            favorite_country_property_update: subscription_id_favorite_country_property_update,
        };

        let property_values = PropOnlyProperties {
            home_address: Arc::new(RwLockWatch::new(
                initial_property_values.home_address.clone(),
            )),
            home_address_version: Arc::new(AtomicU32::new(
                initial_property_values.home_address_version,
            )),

            favorite_country: Arc::new(RwLockWatch::new(
                initial_property_values.favorite_country.clone(),
            )),
            favorite_country_version: Arc::new(AtomicU32::new(
                initial_property_values.favorite_country_version,
            )),
        };

        // Publish the initial property values for all the properties.
        #[cfg(feature = "metrics")]
        let start_prop_publish_time = std::time::Instant::now();
        {
            let topic = strfmt(
                "{prefix}/prop-only/{service_id}/property/home_address/value",
                &topic_param_map,
            )
            .unwrap();

            let payload_obj = HomeAddressProperty {
                address: initial_property_values.home_address,
            };
            let msg = message::property_value(
                &topic,
                &payload_obj,
                initial_property_values.home_address_version,
            )
            .unwrap();

            let _ = connection.publish_nowait(msg);
        }

        {
            let topic = strfmt(
                "{prefix}/prop-only/{service_id}/property/favorite_country/value",
                &topic_param_map,
            )
            .unwrap();

            let payload_obj = FavoriteCountryProperty {
                country: initial_property_values.favorite_country,
            };
            let msg = message::property_value(
                &topic,
                &payload_obj,
                initial_property_values.favorite_country_version,
            )
            .unwrap();

            let _ = connection.publish_nowait(msg);
        }

        #[cfg(feature = "metrics")]
        {
            metrics.initial_property_publish_time = start_prop_publish_time.elapsed();
            debug!(
                "Published 2 initial property values in {:?}",
                metrics.initial_property_publish_time
            );
        }

        PropOnlyServer {
            mqtt_client: connection.clone(),

            msg_streamer_rx: Arc::new(Mutex::new(Some(message_received_rx))),
            msg_streamer_tx: message_received_tx,
            properties: property_values,
            subscription_ids: sub_ids,

            client_id: connection.get_client_id(),
            instance_id,
            topic_param_prefix: prefix,
            #[cfg(feature = "metrics")]
            metrics: Arc::new(AsyncMutex::new(metrics)),
        }
    }

    #[cfg(feature = "metrics")]
    pub fn get_metrics(&self) -> Arc<AsyncMutex<PropOnlyServerMetrics>> {
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

    /// Publishes an error response to the given response topic with the given correlation data.
    async fn publish_error_response(
        mut publisher: C,
        response_topic: Option<String>,
        correlation_data: Option<Bytes>,
        err: MethodReturnCode,
    ) {
        if let Some(resp_topic) = response_topic {
            let msg = message::error_response(&resp_topic, correlation_data, err).unwrap();
            let _ = publisher.publish(msg).await;
        } else {
            info!("No response topic found in message properties; cannot send error response.");
        }
    }

    /// This is called because of an MQTT request to update the property value.
    /// It updates the local value, which notifies any watchers, and publishes the new value.
    /// If there is an error, it can publish back if a response topic was provided.
    async fn update_home_address_value(
        mut publisher: C,
        property_pointer: Arc<RwLockWatch<Address>>, // Arc to the property value
        version_pointer: Arc<AtomicU32>,
        msg: MqttMessage,
    ) {
        // This is JSON encoding of an object with 1 field.
        let payload_str = String::from_utf8_lossy(&msg.payload).to_string();

        let mut return_code = MethodReturnCode::Success(None);

        match msg.content_type.as_deref() {
            Some("application/json") => { /* OK */ }
            Some(ct) => {
                error!("Unexpected content-type for property update: {}", ct);
                return_code = MethodReturnCode::PayloadError(format!(
                    "Invalid Content-Type '{}', expected 'application/json'",
                    ct
                ));
            }
            None => {
                error!("Missing content-type for property update");
                return_code = MethodReturnCode::PayloadError(
                    "Missing Content-Type; expected 'application/json'".to_string(),
                );
            }
        }

        match return_code {
            MethodReturnCode::Success(_) => {
                let mut incoming_version: Option<u32> = None;
                if let Some(version_str) = msg.user_properties.get("PropertyVersion") {
                    match version_str.parse::<u32>() {
                        Ok(v) => incoming_version = Some(v),
                        Err(e) => {
                            error!(
                                "Failed to parse 'PropertyVersion' user property ('{}'): {:?}",
                                version_str, e
                            );
                            return_code = MethodReturnCode::PayloadError(
                                "Invalid 'PropertyVersion' user property".to_string(),
                            );
                        }
                    }
                }

                if let Some(v) = incoming_version {
                    let current = version_pointer.load(Ordering::SeqCst);
                    if v != current {
                        return_code = MethodReturnCode::OutOfSync(format!(
                            "PropertyVersion mismatch: incoming {}, current {}",
                            v, current
                        ));
                    }
                }
            }
            _ => { /* Do nothing, error already set. */ }
        }

        let opt_new_value = match return_code {
            MethodReturnCode::Success(_) => {
                match serde_json::from_str::<HomeAddressProperty>(&payload_str) {
                    Ok(new_property_structure) => {
                        let request_lock = property_pointer.write_request();
                        let mut write_request = request_lock.write().await;

                        // Single value property.  Use the address field of the struct.
                        *write_request = new_property_structure.address.clone();
                        debug!(
                            "Updating 'home_address' property to new value: {:?}",
                            *write_request
                        );

                        // Committing the write request blocks until the message has been published to MQTT.
                        match write_request
                            .commit(std::time::Duration::from_secs(2))
                            .await
                        {
                            CommitResult::Applied(_) => Some((*write_request).clone()),
                            CommitResult::TimedOut => {
                                error!("Timeout committing 'home_address' property change");
                                return_code = MethodReturnCode::ServerError(
                                    "Timeout committing 'home_address' property change".to_string(),
                                );
                                None
                            }
                        }
                    }
                    Err(e) => {
                        error!("Failed to parse JSON received over MQTT to update 'home_address' property: {:?}", e);
                        return_code = MethodReturnCode::ServerDeserializationError(
                            "Failed to deserialize property 'home_address' payload".to_string(),
                        );
                        None
                    }
                }
            }
            _ => None,
        };

        if let Some(resp_topic) = msg.response_topic {
            let corr_data = msg.correlation_data.unwrap_or_default();
            let payload_obj = {
                if let Some(new_value) = opt_new_value {
                    HomeAddressProperty { address: new_value }
                } else {
                    let prop_lock = property_pointer.read().await;

                    HomeAddressProperty {
                        address: (*prop_lock).clone(),
                    }
                }
            };
            match message::property_update_response(
                &resp_topic,
                &payload_obj,
                corr_data,
                return_code,
            ) {
                Ok(msg) => {
                    debug!("Publishing response to 'home_address' property update request to topic '{}', payload: {:?}", resp_topic, payload_obj);
                    let _fut_publish_result = publisher.publish(msg).await;
                }
                Err(err) => {
                    error!(
                        "Error occurred while handling property update for 'home_address': {:?}",
                        &err
                    );
                }
            }
        } else {
            debug!("No response topic provided, so no publishing response to property update for 'home_address'.");
        }
    }

    /// Watch for changes to the `home_address` property.
    /// This returns a watch::Receiver that can be awaited on for changes to the property value.
    /// Use `.borrow_and_update()` on the Receiver to get the current value and reset the watcher.
    pub fn watch_home_address(&self) -> watch::Receiver<Address> {
        self.properties.home_address.subscribe()
    }

    pub fn get_home_address_handle(&self) -> WriteRequestLockWatch<Address> {
        self.properties.home_address.write_request()
    }

    /// Sets the value of the home_address property.
    pub async fn set_home_address(&mut self, value: Address) -> SentMessageFuture {
        let write_request_lock = self.get_home_address_handle();
        Box::pin(async move {
            let mut write_request = write_request_lock.write().await;
            *write_request = value;
            match write_request
                .commit(std::time::Duration::from_secs(2))
                .await
            {
                CommitResult::Applied(_) => Ok(()),
                CommitResult::TimedOut => Err(MethodReturnCode::Timeout(
                    "Timeout committing property change".to_string(),
                )),
            }
        })
    }

    /// This is called because of an MQTT request to update the property value.
    /// It updates the local value, which notifies any watchers, and publishes the new value.
    /// If there is an error, it can publish back if a response topic was provided.
    async fn update_favorite_country_value(
        mut publisher: C,
        property_pointer: Arc<RwLockWatch<Country>>, // Arc to the property value
        version_pointer: Arc<AtomicU32>,
        msg: MqttMessage,
    ) {
        // This is JSON encoding of an object with 1 field.
        let payload_str = String::from_utf8_lossy(&msg.payload).to_string();

        let mut return_code = MethodReturnCode::Success(None);

        match msg.content_type.as_deref() {
            Some("application/json") => { /* OK */ }
            Some(ct) => {
                error!("Unexpected content-type for property update: {}", ct);
                return_code = MethodReturnCode::PayloadError(format!(
                    "Invalid Content-Type '{}', expected 'application/json'",
                    ct
                ));
            }
            None => {
                error!("Missing content-type for property update");
                return_code = MethodReturnCode::PayloadError(
                    "Missing Content-Type; expected 'application/json'".to_string(),
                );
            }
        }

        match return_code {
            MethodReturnCode::Success(_) => {
                let mut incoming_version: Option<u32> = None;
                if let Some(version_str) = msg.user_properties.get("PropertyVersion") {
                    match version_str.parse::<u32>() {
                        Ok(v) => incoming_version = Some(v),
                        Err(e) => {
                            error!(
                                "Failed to parse 'PropertyVersion' user property ('{}'): {:?}",
                                version_str, e
                            );
                            return_code = MethodReturnCode::PayloadError(
                                "Invalid 'PropertyVersion' user property".to_string(),
                            );
                        }
                    }
                }

                if let Some(v) = incoming_version {
                    let current = version_pointer.load(Ordering::SeqCst);
                    if v != current {
                        return_code = MethodReturnCode::OutOfSync(format!(
                            "PropertyVersion mismatch: incoming {}, current {}",
                            v, current
                        ));
                    }
                }
            }
            _ => { /* Do nothing, error already set. */ }
        }

        let opt_new_value = match return_code {
            MethodReturnCode::Success(_) => {
                match serde_json::from_str::<FavoriteCountryProperty>(&payload_str) {
                    Ok(new_property_structure) => {
                        let request_lock = property_pointer.write_request();
                        let mut write_request = request_lock.write().await;

                        // Single value property.  Use the country field of the struct.
                        *write_request = new_property_structure.country.clone();
                        debug!(
                            "Updating 'favorite_country' property to new value: {:?}",
                            *write_request
                        );

                        // Committing the write request blocks until the message has been published to MQTT.
                        match write_request
                            .commit(std::time::Duration::from_secs(2))
                            .await
                        {
                            CommitResult::Applied(_) => Some((*write_request).clone()),
                            CommitResult::TimedOut => {
                                error!("Timeout committing 'favorite_country' property change");
                                return_code = MethodReturnCode::ServerError(
                                    "Timeout committing 'favorite_country' property change"
                                        .to_string(),
                                );
                                None
                            }
                        }
                    }
                    Err(e) => {
                        error!("Failed to parse JSON received over MQTT to update 'favorite_country' property: {:?}", e);
                        return_code = MethodReturnCode::ServerDeserializationError(
                            "Failed to deserialize property 'favorite_country' payload".to_string(),
                        );
                        None
                    }
                }
            }
            _ => None,
        };

        if let Some(resp_topic) = msg.response_topic {
            let corr_data = msg.correlation_data.unwrap_or_default();
            let payload_obj = {
                if let Some(new_value) = opt_new_value {
                    FavoriteCountryProperty { country: new_value }
                } else {
                    let prop_lock = property_pointer.read().await;

                    FavoriteCountryProperty {
                        country: (*prop_lock).clone(),
                    }
                }
            };
            match message::property_update_response(
                &resp_topic,
                &payload_obj,
                corr_data,
                return_code,
            ) {
                Ok(msg) => {
                    debug!("Publishing response to 'favorite_country' property update request to topic '{}', payload: {:?}", resp_topic, payload_obj);
                    let _fut_publish_result = publisher.publish(msg).await;
                }
                Err(err) => {
                    error!("Error occurred while handling property update for 'favorite_country': {:?}", &err);
                }
            }
        } else {
            debug!("No response topic provided, so no publishing response to property update for 'favorite_country'.");
        }
    }

    /// Watch for changes to the `favorite_country` property.
    /// This returns a watch::Receiver that can be awaited on for changes to the property value.
    /// Use `.borrow_and_update()` on the Receiver to get the current value and reset the watcher.
    pub fn watch_favorite_country(&self) -> watch::Receiver<Country> {
        self.properties.favorite_country.subscribe()
    }

    pub fn get_favorite_country_handle(&self) -> WriteRequestLockWatch<Country> {
        self.properties.favorite_country.write_request()
    }

    /// Sets the value of the favorite_country property.
    pub async fn set_favorite_country(&mut self, value: Country) -> SentMessageFuture {
        let write_request_lock = self.get_favorite_country_handle();
        Box::pin(async move {
            let mut write_request = write_request_lock.write().await;
            *write_request = value;
            match write_request
                .commit(std::time::Duration::from_secs(2))
                .await
            {
                CommitResult::Applied(_) => Ok(()),
                CommitResult::TimedOut => Err(MethodReturnCode::Timeout(
                    "Timeout committing property change".to_string(),
                )),
            }
        })
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
            ("interface_name".to_string(), "prop-only".to_string()),
            ("service_id".to_string(), self.instance_id.clone()),
            ("client_id".to_string(), self.client_id.clone()),
            ("prefix".to_string(), self.topic_param_prefix.clone()),
        ]);

        // Take ownership of the RX channel that receives MQTT messages.  This will be moved into the loop_task.
        let mut message_receiver = {
            self.msg_streamer_rx
                .lock()
                .unwrap()
                .take()
                .expect("msg_streamer_rx should be Some")
        };

        let sub_ids = self.subscription_ids.clone();
        let publisher = self.mqtt_client.clone();

        let props = self.properties.clone();
        {
            // Set up property change request handling task
            let mut publisher_for_home_address_prop = self.mqtt_client.clone();
            let home_address_prop_version = props.home_address_version.clone();
            let topic_param_map_for_home_address = topic_param_map.clone();
            if let Some(mut rx_for_home_address_prop) = props.home_address.take_request_receiver() {
                tokio::spawn(async move {
                    while let Some((request, opt_responder)) = rx_for_home_address_prop.recv().await
                    {
                        debug!("Received request to update 'home_address' property value through local watch channel. Current version is {}, request is: {:?}", home_address_prop_version.load(Ordering::SeqCst), request);

                        let payload_obj = HomeAddressProperty {
                            address: request.clone(),
                        };

                        let version_value = home_address_prop_version
                            .fetch_add(1, std::sync::atomic::Ordering::Relaxed)
                            + 1; // fetch_add returns the previous value, so add 1 to get the new version after the update.
                        let topic: String = strfmt(
                            "{prefix}/prop-only/{service_id}/property/home_address/value",
                            &topic_param_map_for_home_address,
                        )
                        .unwrap();
                        match message::property_value(&topic, &payload_obj, version_value) {
                            Ok(msg) => {
                                let publish_result =
                                    publisher_for_home_address_prop.publish(msg).await;
                                if let Some(responder) = opt_responder {
                                    match publish_result {
                                        Ok(_) => {
                                            let _ = responder.send(Some(request));
                                        }
                                        Err(_) => {
                                            error!("Error publishing updated value for 'home_address' property");
                                            let _ = responder.send(None);
                                        }
                                    };
                                }
                            }
                            Err(e) => {
                                error!("Error creating property value message for 'home_address' property: {:?}", e);
                                if let Some(responder) = opt_responder {
                                    let _ = responder.send(None);
                                }
                            }
                        }
                    }
                });
            }
        }

        {
            // Set up property change request handling task
            let mut publisher_for_favorite_country_prop = self.mqtt_client.clone();
            let favorite_country_prop_version = props.favorite_country_version.clone();
            let topic_param_map_for_favorite_country = topic_param_map.clone();
            if let Some(mut rx_for_favorite_country_prop) =
                props.favorite_country.take_request_receiver()
            {
                tokio::spawn(async move {
                    while let Some((request, opt_responder)) =
                        rx_for_favorite_country_prop.recv().await
                    {
                        debug!("Received request to update 'favorite_country' property value through local watch channel. Current version is {}, request is: {:?}", favorite_country_prop_version.load(Ordering::SeqCst), request);

                        let payload_obj = FavoriteCountryProperty {
                            country: request.clone(),
                        };

                        let version_value = favorite_country_prop_version
                            .fetch_add(1, std::sync::atomic::Ordering::Relaxed)
                            + 1; // fetch_add returns the previous value, so add 1 to get the new version after the update.
                        let topic: String = strfmt(
                            "{prefix}/prop-only/{service_id}/property/favorite_country/value",
                            &topic_param_map_for_favorite_country,
                        )
                        .unwrap();
                        match message::property_value(&topic, &payload_obj, version_value) {
                            Ok(msg) => {
                                let publish_result =
                                    publisher_for_favorite_country_prop.publish(msg).await;
                                if let Some(responder) = opt_responder {
                                    match publish_result {
                                        Ok(_) => {
                                            let _ = responder.send(Some(request));
                                        }
                                        Err(_) => {
                                            error!("Error publishing updated value for 'favorite_country' property");
                                            let _ = responder.send(None);
                                        }
                                    };
                                }
                            }
                            Err(e) => {
                                error!("Error creating property value message for 'favorite_country' property: {:?}", e);
                                if let Some(responder) = opt_responder {
                                    let _ = responder.send(None);
                                }
                            }
                        }
                    }
                });
            }
        }

        // Spawn a task to periodically publish interface info.
        let mut interface_publisher = self.mqtt_client.clone();
        let instance_id = self.instance_id.clone();
        let topic_param_map_for_info = topic_param_map.clone();
        tokio::spawn(async move {
            let mut interval = tokio::time::interval(std::time::Duration::from_secs(120));
            loop {
                interval.tick().await;
                let topic = strfmt(
                    "{prefix}/prop-only/{service_id}/interface",
                    &topic_param_map_for_info,
                )
                .unwrap();
                let info = crate::interface::InterfaceInfoBuilder::default()
                    .interface_name("prop-only".to_string())
                    .title("Property Only Interface".to_string())
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

        let properties = self.properties.clone();

        let loop_task = tokio::spawn(async move {
            loop {
                match message_receiver.recv().await {
                    Ok(msg) => {
                        if let Some(subscription_id) = msg.subscription_id {
                            match subscription_id {
                                _i if _i == sub_ids.home_address_property_update => {
                                    debug!(
                                        "Received home_address property update request message."
                                    );
                                    PropOnlyServer::<C>::update_home_address_value(
                                        publisher.clone(),
                                        properties.home_address.clone(),
                                        properties.home_address_version.clone(),
                                        msg,
                                    )
                                    .await;
                                }

                                _i if _i == sub_ids.favorite_country_property_update => {
                                    debug!("Received favorite_country property update request message.");
                                    PropOnlyServer::<C>::update_favorite_country_value(
                                        publisher.clone(),
                                        properties.favorite_country.clone(),
                                        properties.favorite_country_version.clone(),
                                        msg,
                                    )
                                    .await;
                                }

                                _ => {
                                    error!(
                                        "Received MQTT message with unknown subscription id: {}",
                                        subscription_id
                                    );
                                }
                            }
                        } else {
                            warn!("Received MQTT message without subscription id; cannot process.");
                        }
                    }
                    Err(e) => {
                        warn!("Error receiving MQTT message in server loop: {:?}", e);
                    }
                }
            }
        });

        let _ = tokio::join!(loop_task);

        warn!("Server receive loop completed. Exiting run_loop.");
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::message::property_update_request;
    use stinger_mqtt_trait::mock::MockClient;
    use tracing_subscriber::EnvFilter;

    #[tokio::test]
    async fn mock_server() {
        let _ = tracing_subscriber::fmt()
            .with_test_writer()
            .with_env_filter(EnvFilter::new("prop_only_ipc=debug"))
            .try_init();

        let service_id = "N".to_string();
        let client_id = "mock_client".to_string();

        let mut mock_mqtt = MockClient::new(client_id.clone());

        let initial_property_values = PropOnlyInitialPropertyValues {
            home_address: Address {
                street: "apples".to_string(),
                city: "apples".to_string(),
                state: "apples".to_string(),
                postal_code: "apples".to_string(),
                country: Country::Usa,
            },
            home_address_version: 1,

            favorite_country: Country::Usa,
            favorite_country_version: 1,
        };

        let server = PropOnlyServer::new(
            mock_mqtt.clone(),
            service_id.clone(),
            initial_property_values.clone(),
            "prefix".to_string(),
        )
        .await;

        // Start the server connection loop in a separate task.
        let mut looping_server = server.clone();
        let _loop_join_handle = tokio::spawn(async move {
            let _conn_loop = looping_server.run_loop().await;
        });

        let mut topic_param_map = HashMap::from([
            ("interface_name".to_string(), "prop-only".to_string()),
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
        assert_eq!(received_messages.len(), 1 + 2); // 1 for interface online, plus 1 for each property initial publish

        // Publish a property update message for each property

        {
            mock_mqtt.clear_published_messages();

            topic_param_map.insert("property_name".to_string(), "home_address".to_string());
            let property_home_address_topic = strfmt(
                "{prefix}/{interface_name}/{service_id}/property/{property_name}/update",
                &topic_param_map,
            )
            .unwrap();
            let property_home_address_response_topic = strfmt(
                "client/{client_id}/{interface_name}/property/{property_name}/update/response",
                &topic_param_map,
            )
            .unwrap();
            let correlation_uuid = uuid::Uuid::new_v4();

            // Just to get this test working faster, we're copy-pasting test code from payloads.rs to generate example property payloads.
            let json_str = r#"{
                "address": {"street": "apples", "city": "apples", "state": "apples", "postal_code": "apples", "country": 1} 
            }"#;
            let payload: HomeAddressProperty = serde_json::from_str(json_str).unwrap();

            let update_req = property_update_request(
                &property_home_address_topic,
                &payload,
                initial_property_values.home_address_version,
                correlation_uuid,
                property_home_address_response_topic,
            )
            .unwrap();

            info!("Inject message to {}", update_req.topic);
            let result = mock_mqtt.simulate_receive(update_req);
            assert!(result.is_ok());
            assert_eq!(result.unwrap(), 1);

            tokio::time::sleep(tokio::time::Duration::from_millis(100)).await;

            let received_messages = mock_mqtt.published_messages();
            if received_messages.len() != 2 {
                for (i, msg) in received_messages.iter().enumerate() {
                    println!("Message {}: {:?}", i, msg);
                }
            }
            assert_eq!(received_messages.len(), 2);
        }

        {
            mock_mqtt.clear_published_messages();

            topic_param_map.insert("property_name".to_string(), "favorite_country".to_string());
            let property_favorite_country_topic = strfmt(
                "{prefix}/{interface_name}/{service_id}/property/{property_name}/update",
                &topic_param_map,
            )
            .unwrap();
            let property_favorite_country_response_topic = strfmt(
                "client/{client_id}/{interface_name}/property/{property_name}/update/response",
                &topic_param_map,
            )
            .unwrap();
            let correlation_uuid = uuid::Uuid::new_v4();

            // Just to get this test working faster, we're copy-pasting test code from payloads.rs to generate example property payloads.
            let json_str = r#"{
                "country": 1 
            }"#;
            let payload: FavoriteCountryProperty = serde_json::from_str(json_str).unwrap();

            let update_req = property_update_request(
                &property_favorite_country_topic,
                &payload,
                initial_property_values.favorite_country_version,
                correlation_uuid,
                property_favorite_country_response_topic,
            )
            .unwrap();

            info!("Inject message to {}", update_req.topic);
            let result = mock_mqtt.simulate_receive(update_req);
            assert!(result.is_ok());
            assert_eq!(result.unwrap(), 1);

            tokio::time::sleep(tokio::time::Duration::from_millis(100)).await;

            let received_messages = mock_mqtt.published_messages();
            if received_messages.len() != 2 {
                for (i, msg) in received_messages.iter().enumerate() {
                    println!("Message {}: {:?}", i, msg);
                }
            }
            assert_eq!(received_messages.len(), 2);
        }
    }
}
