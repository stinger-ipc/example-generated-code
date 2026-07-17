

#include <vector>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <syslog.h>
#include <sstream>
#include <typeinfo>
#include <stinger/utils/iconnection.hpp>
#include <stinger/utils/uuid.hpp>
#include <stinger/utils/format.hpp>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/error/en.h>
#include <rapidjson/document.h>
#include "structs.hpp"
#include "client.hpp"
#include "enums.hpp"
#include "discovery.hpp"

namespace stinger {

namespace gen {
namespace prop_only {

constexpr const char PropOnlyClient::NAME[];
constexpr const char PropOnlyClient::INTERFACE_VERSION[];

PropOnlyClient::PropOnlyClient(std::shared_ptr<stinger::utils::IConnection> broker, const InstanceInfo& instanceInfo):
    _broker(broker), _instanceId(instanceInfo.serviceId.value_or("error_service_id_not_found")), _instanceInfo(instanceInfo)
{
    _brokerMessageCallbackHandle = _broker->AddMessageCallback([this](const stinger::mqtt::Message& msg)
                                                               {
                                                                   _receiveMessage(msg);
                                                               });

    std::map<std::string, std::string> topicParams;
    topicParams["client_id"] = _broker->GetClientId();
    topicParams["service_id"] = _instanceInfo.serviceId.value_or("error_service_id_not_found");
    topicParams["interface_name"] = NAME;
    topicParams["prefix"] = _instanceInfo.prefix.value_or("error_prefix_not_found");

    auto homeAddressValueTopic = stinger::utils::format("{prefix}/prop-only/{service_id}/property/home_address/value", topicParams);
    _homeAddressPropertySubscriptionId = _broker->Subscribe(homeAddressValueTopic, 1);
    auto favoriteCountryValueTopic = stinger::utils::format("{prefix}/prop-only/{service_id}/property/favorite_country/value", topicParams);
    _favoriteCountryPropertySubscriptionId = _broker->Subscribe(favoriteCountryValueTopic, 1);

    auto anyPropertyUpdateResponseTopic = stinger::utils::format("client/{client_id}/prop-only/property/+/update/response", topicParams);
    _anyPropertyUpdateResponseSubscriptionId = _broker->Subscribe(anyPropertyUpdateResponseTopic, 1);
}

PropOnlyClient::~PropOnlyClient()
{
    if (_broker && _brokerMessageCallbackHandle != 0) {
        _broker->RemoveMessageCallback(_brokerMessageCallbackHandle);
        _brokerMessageCallbackHandle = 0;
    }
}

void PropOnlyClient::_receiveMessage(const stinger::mqtt::Message& msg)
{
    const int noSubId = -1;
    int subscriptionId = msg.properties.subscriptionId.value_or(noSubId);
    _broker->Log(LOG_DEBUG, "Received message on topic %s with subscription id=%d", msg.topic.c_str(), subscriptionId);
    if (subscriptionId == _homeAddressPropertySubscriptionId) {
        _receiveHomeAddressPropertyUpdate(msg);
    } else if (subscriptionId == _favoriteCountryPropertySubscriptionId) {
        _receiveFavoriteCountryPropertyUpdate(msg);
    } else if (subscriptionId == _anyPropertyUpdateResponseSubscriptionId) {
        _broker->Log(LOG_DEBUG, "Matched topic for any property update response");
    }
}

void PropOnlyClient::_receiveHomeAddressPropertyUpdate(const stinger::mqtt::Message& msg)
{
    rapidjson::Document doc;
    rapidjson::ParseResult ok = doc.Parse(msg.payload.c_str());
    if (!ok) {
        //Log("Could not JSON parse home_address property update payload.");
        throw std::runtime_error(rapidjson::GetParseError_En(ok.Code()));
    }

    if (!doc.IsObject()) {
        throw std::runtime_error("Received 'home_address' property update payload is not an object");
    }
    HomeAddressProperty tempValue;

    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = doc.FindMember("address");
        if (itr != doc.MemberEnd() && itr->value.IsObject()) {
            tempValue.address = Address::FromRapidJsonObject(itr->value);

        } else {
            throw std::runtime_error("Received payload for the 'address' argument doesn't have required value/type");
        }
    }

    { // Scope lock
        std::lock_guard<std::mutex> lock(_homeAddressPropertyMutex);
        _homeAddressProperty = tempValue;
        _lastHomeAddressPropertyVersion = msg.properties.propertyVersion ? *msg.properties.propertyVersion : -1;
    }
    // Notify all registered callbacks.
    { // Scope lock
        std::lock_guard<std::mutex> lock(_homeAddressPropertyCallbacksMutex);
        for (const auto& cb: _homeAddressPropertyCallbacks) {
            // Don't need a mutex since we're using tempValue.
            try {
                cb(tempValue.address);
            } catch (const std::exception& e) {
                _broker->Log(LOG_ERR, "Exception in home_address property callback [%s]: %s", typeid(e).name(), e.what());
            } catch (...) {
                _broker->Log(LOG_ERR, "Unknown exception in home_address property callback");
            }
        }
    }
}

std::optional<Address> PropOnlyClient::getHomeAddressProperty()
{
    std::lock_guard<std::mutex> lock(_homeAddressPropertyMutex);
    if (_homeAddressProperty) {
        return _homeAddressProperty->address;
    }
    return std::nullopt;
}

void PropOnlyClient::registerHomeAddressPropertyCallback(const std::function<void(Address address)>& cb)
{
    std::lock_guard<std::mutex> lock(_homeAddressPropertyCallbacksMutex);
    _homeAddressPropertyCallbacks.push_back(cb);
}

std::future<bool> PropOnlyClient::updateHomeAddressProperty(Address address) const
{
    rapidjson::Document doc;
    doc.SetObject();

    { // Restrict Scope for struct serialization
        rapidjson::Value tempStructValue;

        tempStructValue.SetObject();
        address.AddToRapidJsonObject(tempStructValue, doc.GetAllocator());

        doc.AddMember("address", tempStructValue, doc.GetAllocator());
    }

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    doc.Accept(writer);

    std::map<std::string, std::string> topicParams;
    topicParams["client_id"] = _broker->GetClientId();
    topicParams["service_id"] = _instanceInfo.serviceId.value_or("error_service_id_not_found");
    topicParams["interface_name"] = NAME;
    topicParams["prefix"] = _instanceInfo.prefix.value_or("error_prefix_not_found");

    std::string update_topic = stinger::utils::format("{prefix}/prop-only/{service_id}/property/home_address/update", topicParams);
    std::string response_topic = stinger::utils::format("client/{client_id}/prop-only/property/home_address/update/response", topicParams);
    auto correlationData = stinger::utils::generate_uuid_bytes();
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(update_topic, buf.GetString(), _lastHomeAddressPropertyVersion, correlationData, response_topic);
    return _broker->Publish(msg);
}

void PropOnlyClient::_receiveFavoriteCountryPropertyUpdate(const stinger::mqtt::Message& msg)
{
    rapidjson::Document doc;
    rapidjson::ParseResult ok = doc.Parse(msg.payload.c_str());
    if (!ok) {
        //Log("Could not JSON parse favorite_country property update payload.");
        throw std::runtime_error(rapidjson::GetParseError_En(ok.Code()));
    }

    if (!doc.IsObject()) {
        throw std::runtime_error("Received 'favorite_country' property update payload is not an object");
    }
    FavoriteCountryProperty tempValue;

    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = doc.FindMember("country");
        if (itr != doc.MemberEnd() && itr->value.IsInt()) {
            tempValue.country = static_cast<Country>(itr->value.GetInt());

        } else {
            throw std::runtime_error("Received payload for the 'country' argument doesn't have required value/type");
        }
    }

    { // Scope lock
        std::lock_guard<std::mutex> lock(_favoriteCountryPropertyMutex);
        _favoriteCountryProperty = tempValue;
        _lastFavoriteCountryPropertyVersion = msg.properties.propertyVersion ? *msg.properties.propertyVersion : -1;
    }
    // Notify all registered callbacks.
    { // Scope lock
        std::lock_guard<std::mutex> lock(_favoriteCountryPropertyCallbacksMutex);
        for (const auto& cb: _favoriteCountryPropertyCallbacks) {
            // Don't need a mutex since we're using tempValue.
            try {
                cb(tempValue.country);
            } catch (const std::exception& e) {
                _broker->Log(LOG_ERR, "Exception in favorite_country property callback [%s]: %s", typeid(e).name(), e.what());
            } catch (...) {
                _broker->Log(LOG_ERR, "Unknown exception in favorite_country property callback");
            }
        }
    }
}

std::optional<Country> PropOnlyClient::getFavoriteCountryProperty()
{
    std::lock_guard<std::mutex> lock(_favoriteCountryPropertyMutex);
    if (_favoriteCountryProperty) {
        return _favoriteCountryProperty->country;
    }
    return std::nullopt;
}

void PropOnlyClient::registerFavoriteCountryPropertyCallback(const std::function<void(Country country)>& cb)
{
    std::lock_guard<std::mutex> lock(_favoriteCountryPropertyCallbacksMutex);
    _favoriteCountryPropertyCallbacks.push_back(cb);
}

std::future<bool> PropOnlyClient::updateFavoriteCountryProperty(Country country) const
{
    rapidjson::Document doc;
    doc.SetObject();

    doc.AddMember("country", static_cast<int>(country), doc.GetAllocator());

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    doc.Accept(writer);

    std::map<std::string, std::string> topicParams;
    topicParams["client_id"] = _broker->GetClientId();
    topicParams["service_id"] = _instanceInfo.serviceId.value_or("error_service_id_not_found");
    topicParams["interface_name"] = NAME;
    topicParams["prefix"] = _instanceInfo.prefix.value_or("error_prefix_not_found");

    std::string update_topic = stinger::utils::format("{prefix}/prop-only/{service_id}/property/favorite_country/update", topicParams);
    std::string response_topic = stinger::utils::format("client/{client_id}/prop-only/property/favorite_country/update/response", topicParams);
    auto correlationData = stinger::utils::generate_uuid_bytes();
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(update_topic, buf.GetString(), _lastFavoriteCountryPropertyVersion, correlationData, response_topic);
    return _broker->Publish(msg);
}

} // namespace prop_only

} // namespace gen

} // namespace stinger
