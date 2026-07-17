/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

It contains enumerations used by the prop-only interface.

LICENSE: This generated code is not subject to any license restrictions from the generator itself.
TODO: Get license text from stinger file
*/

#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <memory>
#include <exception>
#include <mutex>
#include <chrono>
#include <thread>
#include <atomic>
#include <rapidjson/document.h>

#include "property_structs.hpp"

#include <stinger/utils/iconnection.hpp>
#include <stinger/mqtt/properties.hpp>
#include "enums.hpp"

namespace stinger {

namespace gen {
namespace prop_only {

class PropOnlyServer {
public:
    static constexpr const char NAME[] = "prop-only";
    static constexpr const char INTERFACE_VERSION[] = "0.0.1";

    PropOnlyServer(std::shared_ptr<stinger::utils::IConnection> broker, const std::string& instanceId, const std::string& prefix);

    virtual ~PropOnlyServer();

    // ---home_address Property---

    // Gets the latest value of the `home_address` property, if one has been received.
    // If no value has been received yet, an empty optional is returned.

    std::optional<Address> getHomeAddressProperty();

    // Add a callback that will be called whenever the `home_address` property is updated.
    // The provided method will be called whenever a new value for the `home_address` property is received.
    void registerHomeAddressPropertyCallback(const std::function<void(Address)>& cb);

    void updateHomeAddressProperty(Address);

    void republishHomeAddressProperty() const;

    // ---favorite_country Property---

    // Gets the latest value of the `favorite_country` property, if one has been received.
    // If no value has been received yet, an empty optional is returned.

    std::optional<Country> getFavoriteCountryProperty();

    // Add a callback that will be called whenever the `favorite_country` property is updated.
    // The provided method will be called whenever a new value for the `favorite_country` property is received.
    void registerFavoriteCountryPropertyCallback(const std::function<void(Country)>& cb);

    void updateFavoriteCountryProperty(Country);

    void republishFavoriteCountryProperty() const;

private:
    std::shared_ptr<stinger::utils::IConnection> _broker;
    std::string _instanceId;

    std::string _prefixTopicParam;

    stinger::utils::CallbackHandleType _brokerMessageCallbackHandle = 0;
    void _receiveMessage(const stinger::mqtt::Message& msg);

    // ---------------- PROPERTIES ------------------

    // ---home_address Property---

    // Current value for the `home_address` property.
    std::optional<HomeAddressProperty> _homeAddressProperty;

    // This is the property version  of `home_address`.
    int _lastHomeAddressPropertyVersion = -1;

    // Mutex for protecting access to the `home_address` property and its version.
    mutable std::mutex _homeAddressPropertyMutex;

    // MQTT Subscription ID for `home_address` property update requests.
    int _homeAddressPropertySubscriptionId;

    // Method for parsing a JSON payload that updates the `home_address` property.
    void _receiveHomeAddressPropertyUpdate(const stinger::mqtt::Message& msg);

    // Callbacks registered for changes to the `home_address` property.
    std::vector<std::function<void(Address)>> _homeAddressPropertyCallbacks;
    std::mutex _homeAddressPropertyCallbacksMutex;

    // ---favorite_country Property---

    // Current value for the `favorite_country` property.
    std::optional<FavoriteCountryProperty> _favoriteCountryProperty;

    // This is the property version  of `favorite_country`.
    int _lastFavoriteCountryPropertyVersion = -1;

    // Mutex for protecting access to the `favorite_country` property and its version.
    mutable std::mutex _favoriteCountryPropertyMutex;

    // MQTT Subscription ID for `favorite_country` property update requests.
    int _favoriteCountryPropertySubscriptionId;

    // Method for parsing a JSON payload that updates the `favorite_country` property.
    void _receiveFavoriteCountryPropertyUpdate(const stinger::mqtt::Message& msg);

    // Callbacks registered for changes to the `favorite_country` property.
    std::vector<std::function<void(Country)>> _favoriteCountryPropertyCallbacks;
    std::mutex _favoriteCountryPropertyCallbacksMutex;

    // ---------------- SERVICE ADVERTISEMENT ------------------

    // Thread for publishing service advertisement messages
    std::thread _advertisementThread;

    // Flag to signal the advertisement thread to stop
    std::atomic<bool> _advertisementThreadRunning;

    // Method that runs in the advertisement thread
    void _advertisementThreadLoop();
};

} // namespace prop_only

} // namespace gen

} // namespace stinger
