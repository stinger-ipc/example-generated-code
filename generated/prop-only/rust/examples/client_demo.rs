//! Client module for Full IPC
//!
//! This module is only available when the "client" feature is enabled.

/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

This is the Client for the Full interface.

LICENSE: This generated code is not subject to any license restrictions from the generator itself.
TODO: Get license text from stinger file
*/

use mqttier::{Connection, MqttierClient, MqttierOptionsBuilder, TcpConnection};
use prop_only_ipc::client::PropOnlyClient;
use prop_only_ipc::discovery::PropOnlyDiscovery;
#[cfg(feature = "lwt")]
use prop_only_ipc::lwt::StingerAvailability;
#[allow(unused_imports)]
use prop_only_ipc::payloads::{MethodReturnCode, *};
use tokio::join;
use tokio::time::{sleep, Duration};
#[allow(unused_imports)]
use tracing::{debug, error, info, warn};
#[tokio::main]
async fn main() {
    // Initialize tracing subscriber to see log output
    tracing_subscriber::fmt()
        .with_env_filter(
            tracing_subscriber::EnvFilter::try_from_default_env()
                .unwrap_or_else(|_| tracing_subscriber::EnvFilter::new("info")),
        )
        .init();

    info!("Starting prop-only client demo...");

    // Create an MQTT client that implements the MqttPubSub trait.
    // Application code is responsible for managing the client object.
    #[cfg(feature = "lwt")]
    let lwt = StingerAvailability::new("example");
    let mut mqttier_options_builder = MqttierOptionsBuilder::default();
    mqttier_options_builder
        .connection(Connection::Tcp(TcpConnection::from_env_with_defaults(
            "localhost",
            1883,
        )))
        .client_id("rust-client-demo".to_string());
    #[cfg(feature = "lwt")]
    let mqttier_options = mqttier_options_builder
        .availability_helper(Some(lwt))
        .build()
        .unwrap();
    #[cfg(not(feature = "lwt"))]
    let mqttier_options = mqttier_options_builder.build().unwrap();
    let mut mqttier_client = MqttierClient::new(mqttier_options).unwrap();
    let _ = mqttier_client.start().await;

    // We need to discover a service instance before we can create the client.
    // For this demo, we assume a singleton server.
    let service_discovery = PropOnlyDiscovery::new(&mut mqttier_client).await.unwrap();
    // The `discovered_singleton` struct contains the service_id and initial property values.
    let discovered_singleton = service_discovery.get_singleton_service().await;

    #[cfg(feature = "metrics")]
    {
        let metrics = service_discovery
            .metrics
            .lock()
            .expect("Failed to lock metrics");
        println!("Discovery complete.  Metrics: {:?}", metrics);
        println!(
            "Time to first discovery (ms): {:?}",
            metrics.time_to_first_discovery_ms()
        );
    };
    drop(service_discovery);
    let prop_only_client = PropOnlyClient::new(mqttier_client.clone(), discovered_singleton).await;

    let mut client_for_loop = prop_only_client.clone();
    tokio::spawn(async move {
        let _conn_loop = client_for_loop.run_loop().await;
    });

    // This task subscribes to a watch chanel for each property to get notified of changes.
    let client_for_prop_change = prop_only_client.clone();
    let _prop_change_rx_task = tokio::spawn(async move {
        let mut home_address_change_rx = client_for_prop_change.watch_home_address();
        let mut favorite_country_change_rx = client_for_prop_change.watch_favorite_country();

        loop {
            tokio::select! {
                _ = home_address_change_rx.changed() => {
                    println!("Property 'home_address' changed to: {:?}", *home_address_change_rx.borrow());
                }
                _ = favorite_country_change_rx.changed() => {
                    println!("Property 'favorite_country' changed to: {:?}", *favorite_country_change_rx.borrow());
                }
            }
        }
    });

    // Property handles are Send so we can move them into tasks.

    let home_address_handle = prop_only_client.get_home_address_handle();

    let favorite_country_handle = prop_only_client.get_favorite_country_handle();

    let property_update_task = tokio::spawn(async move {
        let mut i = 0;
        loop {
            sleep(Duration::from_secs(20)).await;

            {
                // Scoping for 'home_address' property.  Demonstrates reading the value.
                let current_value_ref = home_address_handle.read().await;
                println!(
                    "=== Current value of property 'home_address': {:?}",
                    *current_value_ref
                );
            }

            sleep(Duration::from_secs(2)).await;
            {
                // Scoping for 'home_address' property.  Demonstrates creating a request to set the value.
                let home_address_new_value = Address {
                    street: "apples".to_string(),
                    city: "apples".to_string(),
                    state: "apples".to_string(),
                    postal_code: "apples".to_string(),
                    country: Country::Usa,
                };
                let mut write_lock = home_address_handle.write().await;
                *write_lock = home_address_new_value;
                println!(
                    "<~~ Sending request to update property 'home_address' to new value: {:?}",
                    *write_lock
                );
            }
            sleep(Duration::from_secs(10)).await;

            {
                // Scoping for 'favorite_country' property.  Demonstrates reading the value.
                let current_value_ref = favorite_country_handle.read().await;
                println!(
                    "=== Current value of property 'favorite_country': {:?}",
                    *current_value_ref
                );
            }

            sleep(Duration::from_secs(2)).await;
            {
                // Scoping for 'favorite_country' property.  Demonstrates creating a request to set the value.
                let favorite_country_new_value = Country::Usa;
                let mut write_lock = favorite_country_handle.write().await;
                *write_lock = favorite_country_new_value;
                println!(
                    "<~~ Sending request to update property 'favorite_country' to new value: {:?}",
                    *write_lock
                );
            }
            sleep(Duration::from_secs(10)).await;

            i += 1;
        }
    });

    println!("Waiting for Ctrl-C to exit...");
    tokio::signal::ctrl_c()
        .await
        .expect("Failed to listen for Ctrl-C");
    println!("Ctrl-C received, shutting down...");

    property_update_task.abort();

    // Join on all the signal emitting tasks.
    let _ = join!(property_update_task,);

    // Ctrl-C to stop
}
