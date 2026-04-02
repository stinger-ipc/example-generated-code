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
namespace weather {

struct InitialPropertyValues {
public:
    std::optional<LocationProperty> location;
    int locationVersion = 0;

    std::optional<CurrentTemperatureProperty> currentTemperature;
    int currentTemperatureVersion = 0;

    std::optional<CurrentConditionProperty> currentCondition;
    int currentConditionVersion = 0;

    std::optional<DailyForecastProperty> dailyForecast;
    int dailyForecastVersion = 0;

    std::optional<HourlyForecastProperty> hourlyForecast;
    int hourlyForecastVersion = 0;

    std::optional<CurrentConditionRefreshIntervalProperty> currentConditionRefreshInterval;
    int currentConditionRefreshIntervalVersion = 0;

    std::optional<HourlyForecastRefreshIntervalProperty> hourlyForecastRefreshInterval;
    int hourlyForecastRefreshIntervalVersion = 0;

    std::optional<DailyForecastRefreshIntervalProperty> dailyForecastRefreshInterval;
    int dailyForecastRefreshIntervalVersion = 0;

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
 * This uses an IConnection object to subscribe to MQTT topics to receive messages that describe the weather interface.
 */
class WeatherDiscovery {
public:
    // Constructor taking a broker connection and service_id
    WeatherDiscovery(std::shared_ptr<stinger::utils::IConnection> broker);

    virtual ~WeatherDiscovery();

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

} // namespace weather

} // namespace gen

} // namespace stinger
