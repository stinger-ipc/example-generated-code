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
#include <memory>
#include <exception>
#include <mutex>
#include <chrono>
#include <rapidjson/document.h>
#include <stinger/mqtt/properties.hpp>
#include <stinger/utils/iconnection.hpp>
#include <stinger/error/return_codes.hpp>
#include "enums.hpp"
#include "method_payloads.hpp"
#include "signal_payloads.hpp"
#include "discovery.hpp"

#include "property_structs.hpp"

namespace stinger {

namespace gen {
namespace prop_only {

class PropOnlyClient {
public:
    // This is the name of the API.
    static constexpr const char NAME[] = "prop-only";
    // This is the version of the API contract.
    static constexpr const char INTERFACE_VERSION[] = "0.0.1";

    // Constructor taking a connection object.
    PropOnlyClient(std::shared_ptr<stinger::utils::IConnection> broker, const InstanceInfo& instanceInfo);

    virtual ~PropOnlyClient();

    // ---------------- PROPERTIES ------------------

    // ---home_address Property---

    // Gets the latest value of the `home_address` property, if one has been received.
    // If no value has been received yet, an empty optional is returned.

    std::optional<Address> getHomeAddressProperty();

    // Add a callback that will be called whenever the `home_address` property is updated.
    // The provided method will be called whenever a new value for the `home_address` property is received.
    void registerHomeAddressPropertyCallback(const std::function<void(Address)>& cb);

    std::future<bool> updateHomeAddressProperty(Address) const;

    // ---favorite_country Property---

    // Gets the latest value of the `favorite_country` property, if one has been received.
    // If no value has been received yet, an empty optional is returned.

    std::optional<Country> getFavoriteCountryProperty();

    // Add a callback that will be called whenever the `favorite_country` property is updated.
    // The provided method will be called whenever a new value for the `favorite_country` property is received.
    void registerFavoriteCountryPropertyCallback(const std::function<void(Country)>& cb);

    std::future<bool> updateFavoriteCountryProperty(Country) const;

private:
    // Pointer to the broker connection.
    std::shared_ptr<stinger::utils::IConnection> _broker;

    // Service Instance ID that this client is connected to.
    std::string _instanceId;
    InstanceInfo _instanceInfo;

    stinger::utils::CallbackHandleType _brokerMessageCallbackHandle = 0;

    // Internal method for receiving messages from the broker.
    void _receiveMessage(const stinger::mqtt::Message& msg);

    // ---------------- PROPERTIES ------------------
    int _anyPropertyUpdateResponseSubscriptionId = -1;

    // ---home_address Property---

    // Last received value for the `home_address` property.
    std::optional<HomeAddressProperty> _homeAddressProperty;

    // This is the property version of the last received `home_address` property update.
    int _lastHomeAddressPropertyVersion = -1;

    // Mutex for protecting access to the `home_address` property and its version.
    mutable std::mutex _homeAddressPropertyMutex;

    // MQTT Subscription ID for `home_address` property updates.
    int _homeAddressPropertySubscriptionId;

    // Method for parsing a JSON payload that updates the `home_address` property.
    void _receiveHomeAddressPropertyUpdate(const stinger::mqtt::Message& msg);

    // Callbacks registered for changes to the `home_address` property.
    std::vector<std::function<void(Address)>> _homeAddressPropertyCallbacks;
    std::mutex _homeAddressPropertyCallbacksMutex;

    // ---favorite_country Property---

    // Last received value for the `favorite_country` property.
    std::optional<FavoriteCountryProperty> _favoriteCountryProperty;

    // This is the property version of the last received `favorite_country` property update.
    int _lastFavoriteCountryPropertyVersion = -1;

    // Mutex for protecting access to the `favorite_country` property and its version.
    mutable std::mutex _favoriteCountryPropertyMutex;

    // MQTT Subscription ID for `favorite_country` property updates.
    int _favoriteCountryPropertySubscriptionId;

    // Method for parsing a JSON payload that updates the `favorite_country` property.
    void _receiveFavoriteCountryPropertyUpdate(const stinger::mqtt::Message& msg);

    // Callbacks registered for changes to the `favorite_country` property.
    std::vector<std::function<void(Country)>> _favoriteCountryPropertyCallbacks;
    std::mutex _favoriteCountryPropertyCallbacksMutex;
};

} // namespace prop_only

} // namespace gen

} // namespace stinger
