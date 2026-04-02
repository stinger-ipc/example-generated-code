/*
Discovery service for finding available service instances.
*/

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <stinger/utils/iconnection.hpp>
#include <stinger/mqtt/properties.hpp>
#include "structs.hpp"
#include "enums.hpp"
#include "property_structs.hpp"

namespace stinger {

namespace gen {
namespace full {

struct InitialPropertyValues {
public:
    std::optional<FavoriteNumberProperty> favoriteNumber;
    int favoriteNumberVersion = 0;

    std::optional<FavoriteFoodsProperty> favoriteFoods;
    int favoriteFoodsVersion = 0;

    std::optional<LunchMenuProperty> lunchMenu;
    int lunchMenuVersion = 0;

    std::optional<FamilyNameProperty> familyName;
    int familyNameVersion = 0;

    std::optional<LastBreakfastTimeProperty> lastBreakfastTime;
    int lastBreakfastTimeVersion = 0;

    std::optional<LastBirthdaysProperty> lastBirthdays;
    int lastBirthdaysVersion = 0;

    bool isComplete() const;
};

/**
 * Information about a discovered service instance, including its service ID, topic parameters, and initial property values.
 * This is updated as information is received over MQTT about the interface.
 */
struct InstanceInfo {
public:
    std::optional<std::string> serviceId; // In the payload, this is the 'instance' field.
    std::optional<std::string> prefix;
    InitialPropertyValues initial_property_values; // Not included in (de-)serialization.

    /**
     * This is the deserialization method for the InstanceInfo struct. 
     * The JSON text should already be parsed into a rapidjson::Value object, and this method will extract the relevant fields to populate the InstanceInfo struct.
     */
    void UpdateFromRapidJsonObject(const rapidjson::Value& jsonObj);

    /**
    * This is the serialization method for the InstanceInfo struct.
    */
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;

    bool isComplete() const;
};

/**
 * This uses an IConnection object to subscribe to MQTT topics to receive messages that describe the Full interface.
 */
class FullDiscovery {
public:
    // Constructor taking a broker connection and service_id
    FullDiscovery(std::shared_ptr<stinger::utils::IConnection> broker);

    virtual ~FullDiscovery();

    // Set a callback to be invoked when a new service instance is discovered
    void SetDiscoveryCallback(const std::function<void(const InstanceInfo&)>& cb);

    // Get a singleton instance ID. Returns immediately if one is available,
    // otherwise waits until one is discovered.
    std::future<InstanceInfo> GetSingleton();

    // Get all discovered instance IDs
    std::vector<InstanceInfo> GetInstances() const;

private:
    void _onMessage(const stinger::mqtt::Message& msg);
    int _discoverySubscriptionId = -1;
    int _allPropertySubscriptionId = -1;
    stinger::utils::CallbackHandleType _brokerMessageCallbackHandle = 0;
    std::shared_ptr<stinger::utils::IConnection> _broker;
    std::map<uint64_t, InstanceInfo> _discoveredInstances; // Keyed by a hash of service_id, and prefix
    std::function<void(const InstanceInfo&)> _discovery_callback;

    mutable std::mutex _mutex;
    std::vector<std::promise<InstanceInfo>> _pending_promises;
};

} // namespace full

} // namespace gen

} // namespace stinger
