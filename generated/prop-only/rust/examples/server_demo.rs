/*
DO NOT MODIFY THIS FILE .  It is automatically generated and changes will be over-written
on the next generation.

It contains enumerations used by the prop-only interface.

LICENSE: This generated code is not subject to any license restrictions from the generator itself.
TODO: Get license text from stinger file
*/
use std::any::Any;
use std::sync::Arc;

use mqttier::{Connection, MqttierClient, MqttierOptionsBuilder, TcpConnection};
#[cfg(feature = "lwt")]
use prop_only_ipc::lwt::StingerAvailability;
use prop_only_ipc::property::PropOnlyInitialPropertyValues;
use prop_only_ipc::server::PropOnlyServer;
use tokio::time::{sleep, Duration};

#[allow(unused_imports)]
use prop_only_ipc::payloads::{MethodReturnCode, *};
use tokio::join;

#[tokio::main]
async fn main() {
    // Initialize tracing subscriber to see log output
    tracing_subscriber::fmt()
        .with_env_filter(
            tracing_subscriber::EnvFilter::try_from_default_env()
                .unwrap_or_else(|_| tracing_subscriber::EnvFilter::new("info")),
        )
        .init();

    // Set up an MQTT client connection.
    #[cfg(feature = "lwt")]
    let lwt = StingerAvailability::new("example");
    let mut mqttier_options_builder = MqttierOptionsBuilder::default();
    mqttier_options_builder
        .connection(Connection::Tcp(TcpConnection::from_env_with_defaults(
            "localhost",
            1883,
        )))
        .client_id("rust-server-demo".to_string());
    #[cfg(feature = "lwt")]
    let mqttier_options = mqttier_options_builder
        .availability_helper(Some(lwt))
        .build()
        .unwrap();
    #[cfg(not(feature = "lwt"))]
    let mqttier_options = mqttier_options_builder.build().unwrap();
    let mut connection = MqttierClient::new(mqttier_options).unwrap();
    let _ = connection.start().await.unwrap();

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

    // Create the server object.
    let mut server = PropOnlyServer::new(
        connection,
        "rust-server-demo:1".to_string(),
        initial_property_values,
        "example".to_string(),
    )
    .await;

    // Start the server connection loop in a separate task.
    let mut looping_server = server.clone();
    let _loop_join_handle = tokio::spawn(async move {
        println!("Starting connection loop");
        let _conn_loop = looping_server.run_loop().await;
    });

    let signal_publish_task = tokio::spawn(async move {
        loop {
            sleep(Duration::from_secs(67)).await;
        }
    });

    // Provide property handles to the property_publish_task which will use them to continuously update property values.

    let home_address_property = server.get_home_address_handle();
    let home_address_property_monitor = tokio::spawn({
        let mut property_handle_watcher = home_address_property.subscribe();
        async move {
            while property_handle_watcher.changed().await.is_ok() {
                println!(
                    "Property 'home_address' value has changed to: {:?}",
                    *(property_handle_watcher.borrow_and_update())
                );
            }
        }
    });

    let favorite_country_property = server.get_favorite_country_handle();
    let favorite_country_property_monitor = tokio::spawn({
        let mut property_handle_watcher = favorite_country_property.subscribe();
        async move {
            while property_handle_watcher.changed().await.is_ok() {
                println!(
                    "Property 'favorite_country' value has changed to: {:?}",
                    *(property_handle_watcher.borrow_and_update())
                );
            }
        }
    });
    let property_publish_task = tokio::spawn(async move {
        loop {
            sleep(Duration::from_secs(51)).await;

            sleep(Duration::from_secs(1)).await;
            {
                println!("Demo code periodic change of property 'home_address'");
                let mut home_address_guard = home_address_property.write().await;
                *home_address_guard = Address {
                    street: "foo".to_string(),
                    city: "foo".to_string(),
                    state: "foo".to_string(),
                    postal_code: "foo".to_string(),
                    country: Country::Usa,
                };
                // Value is changed and published when home_address_guard goes out of scope and is dropped.
            }

            sleep(Duration::from_secs(1)).await;
            {
                println!("Demo code periodic change of property 'favorite_country'");
                let mut favorite_country_guard = favorite_country_property.write().await;
                *favorite_country_guard = Country::Usa;
                // Value is changed and published when favorite_country_guard goes out of scope and is dropped.
            }
        }
    });

    let _ = join!(signal_publish_task, property_publish_task);

    // Ctrl-C to stop
}
