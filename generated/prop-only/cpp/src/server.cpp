
#include <vector>
#include <iostream>
#include <syslog.h>
#include <typeinfo>
#include <future>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/error/en.h>
#include <rapidjson/document.h>
#include "structs.hpp"
#include "server.hpp"
#include "method_payloads.hpp"
#include "enums.hpp"
#include <stinger/utils/iconnection.hpp>
#include <stinger/utils/format.hpp>
#include <stinger/error/return_codes.hpp>

namespace stinger {

namespace gen {
namespace prop_only {

constexpr const char PropOnlyServer::NAME[];
constexpr const char PropOnlyServer::INTERFACE_VERSION[];

PropOnlyServer::PropOnlyServer(std::shared_ptr<stinger::utils::IConnection> broker, const std::string& instanceId, const std::string& prefix):
    _broker(broker), _instanceId(instanceId), _advertisementThreadRunning(false), _prefixTopicParam(prefix)

{
    _brokerMessageCallbackHandle = _broker->AddMessageCallback([this](
                                                                       const stinger::mqtt::Message& msg
                                                               )
                                                               {
                                                                   _receiveMessage(msg);
                                                               });

    std::map<std::string, std::string> topicArgs;
    topicArgs["service_id"] = _instanceId;
    topicArgs["interface_name"] = NAME;
    topicArgs["client_id"] = _broker->GetClientId();
    topicArgs["prefix"] = _prefixTopicParam;

    _homeAddressPropertySubscriptionId = _broker->Subscribe(stinger::utils::format("{prefix}/prop-only/{service_id}/property/home_address/update", topicArgs), 1);
    _favoriteCountryPropertySubscriptionId = _broker->Subscribe(stinger::utils::format("{prefix}/prop-only/{service_id}/property/favorite_country/update", topicArgs), 1);

    // Start the service advertisement thread
    _advertisementThreadRunning = true;
    _advertisementThread = std::thread(&PropOnlyServer::_advertisementThreadLoop, this);
}

PropOnlyServer::~PropOnlyServer()
{
    // Unregister the message callback from the broker.
    if (_broker && _brokerMessageCallbackHandle != 0) {
        _broker->RemoveMessageCallback(_brokerMessageCallbackHandle);
        _brokerMessageCallbackHandle = 0;
    }

    // Stop the advertisement thread
    _advertisementThreadRunning = false;
    if (_advertisementThread.joinable()) {
        _advertisementThread.join();
    }

    std::map<std::string, std::string> topicArgs;
    topicArgs["service_id"] = _instanceId;
    topicArgs["interface_name"] = NAME;
    topicArgs["client_id"] = _broker->GetClientId();
    topicArgs["prefix"] = _prefixTopicParam;

    std::string topic = stinger::utils::format("{prefix}/prop-only/{service_id}/interface", topicArgs);
    auto msg = stinger::mqtt::Message::ServiceOffline(topic);
    _broker->Publish(msg);

    _broker->Unsubscribe(stinger::utils::format("{prefix}/prop-only/{service_id}/property/home_address/update", topicArgs));
    _broker->Unsubscribe(stinger::utils::format("{prefix}/prop-only/{service_id}/property/favorite_country/update", topicArgs));
}

void PropOnlyServer::_receiveMessage(const stinger::mqtt::Message& msg)
{
    const int noSubId = -1;
    int subscriptionId = msg.properties.subscriptionId.value_or(noSubId);

    if (subscriptionId == _homeAddressPropertySubscriptionId) {
        _broker->Log(LOG_INFO, "Message to `%s` matched as home_address property update.", msg.topic.c_str());
        _receiveHomeAddressPropertyUpdate(msg);
    }

    else if (subscriptionId == _favoriteCountryPropertySubscriptionId) {
        _broker->Log(LOG_INFO, "Message to `%s` matched as favorite_country property update.", msg.topic.c_str());
        _receiveFavoriteCountryPropertyUpdate(msg);
    }
}

std::optional<Address> PropOnlyServer::getHomeAddressProperty()
{
    std::lock_guard<std::mutex> lock(_homeAddressPropertyMutex);
    if (_homeAddressProperty) {
        return _homeAddressProperty->address;
    }
    return std::nullopt;
}

void PropOnlyServer::registerHomeAddressPropertyCallback(const std::function<void(Address address)>& cb)
{
    std::lock_guard<std::mutex> lock(_homeAddressPropertyCallbacksMutex);
    _homeAddressPropertyCallbacks.push_back(cb);
}

void PropOnlyServer::updateHomeAddressProperty(Address address)
{
    { // Scope lock
        std::lock_guard<std::mutex> lock(_homeAddressPropertyMutex);
        _homeAddressProperty = HomeAddressProperty{ address };
        _lastHomeAddressPropertyVersion++;
    }
    { // Scope lock
        std::lock_guard<std::mutex> lock(_homeAddressPropertyCallbacksMutex);
        for (const auto& cb: _homeAddressPropertyCallbacks) {
            try {
                cb(address);
            } catch (const std::exception& e) {
                _broker->Log(LOG_ERR, "Exception in home_address property callback [%s]: %s", typeid(e).name(), e.what());
            } catch (...) {
                _broker->Log(LOG_ERR, "Unknown exception in home_address property callback");
            }
        }
    }
    republishHomeAddressProperty();
}

void PropOnlyServer::republishHomeAddressProperty() const
{
    std::lock_guard<std::mutex> lock(_homeAddressPropertyMutex);
    rapidjson::Document doc;
    if (_homeAddressProperty) {
        doc.SetObject();
        _homeAddressProperty->AddToRapidJsonObject(doc, doc.GetAllocator());
    } else {
        doc.SetNull();
    }

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    doc.Accept(writer);

    std::map<std::string, std::string> topicArgs;
    topicArgs["service_id"] = _instanceId;
    topicArgs["interface_name"] = NAME;
    topicArgs["client_id"] = _broker->GetClientId();
    topicArgs["property_name"] = "home_address";
    topicArgs["prefix"] = _prefixTopicParam;

    auto topic = stinger::utils::format("{prefix}/prop-only/{service_id}/property/home_address/value", topicArgs);
    auto msg = stinger::mqtt::Message::PropertyValue(topic, buf.GetString(), _lastHomeAddressPropertyVersion);
    _broker->Publish(msg);
}

void PropOnlyServer::_receiveHomeAddressPropertyUpdate(const stinger::mqtt::Message& msg)
{
    rapidjson::Document doc;
    rapidjson::ParseResult ok = doc.Parse(msg.payload.c_str());
    if (!ok) {
        //Log("Could not JSON parse home_address property update payload.");
        throw std::runtime_error(rapidjson::GetParseError_En(ok.Code()));
    }

    if (!doc.IsObject() && !doc.IsNull()) {
        throw std::runtime_error("Received home_address payload is not an object or null");
    }

    // TODO: Check _lastHomeAddressPropertyVersion against optPropertyVersion and
    // reject the update if it's older than what we have.

    // Deserialize 1 values into struct.
    HomeAddressProperty tempValue = HomeAddressProperty::FromRapidJsonObject(doc);

    { // Scope lock
        std::lock_guard<std::mutex> lock(_homeAddressPropertyMutex);
        _homeAddressProperty = tempValue;
        _lastHomeAddressPropertyVersion++;
    }
    republishHomeAddressProperty();
}

std::optional<Country> PropOnlyServer::getFavoriteCountryProperty()
{
    std::lock_guard<std::mutex> lock(_favoriteCountryPropertyMutex);
    if (_favoriteCountryProperty) {
        return _favoriteCountryProperty->country;
    }
    return std::nullopt;
}

void PropOnlyServer::registerFavoriteCountryPropertyCallback(const std::function<void(Country country)>& cb)
{
    std::lock_guard<std::mutex> lock(_favoriteCountryPropertyCallbacksMutex);
    _favoriteCountryPropertyCallbacks.push_back(cb);
}

void PropOnlyServer::updateFavoriteCountryProperty(Country country)
{
    { // Scope lock
        std::lock_guard<std::mutex> lock(_favoriteCountryPropertyMutex);
        _favoriteCountryProperty = FavoriteCountryProperty{ country };
        _lastFavoriteCountryPropertyVersion++;
    }
    { // Scope lock
        std::lock_guard<std::mutex> lock(_favoriteCountryPropertyCallbacksMutex);
        for (const auto& cb: _favoriteCountryPropertyCallbacks) {
            try {
                cb(country);
            } catch (const std::exception& e) {
                _broker->Log(LOG_ERR, "Exception in favorite_country property callback [%s]: %s", typeid(e).name(), e.what());
            } catch (...) {
                _broker->Log(LOG_ERR, "Unknown exception in favorite_country property callback");
            }
        }
    }
    republishFavoriteCountryProperty();
}

void PropOnlyServer::republishFavoriteCountryProperty() const
{
    std::lock_guard<std::mutex> lock(_favoriteCountryPropertyMutex);
    rapidjson::Document doc;
    if (_favoriteCountryProperty) {
        doc.SetObject();
        _favoriteCountryProperty->AddToRapidJsonObject(doc, doc.GetAllocator());
    } else {
        doc.SetNull();
    }

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    doc.Accept(writer);

    std::map<std::string, std::string> topicArgs;
    topicArgs["service_id"] = _instanceId;
    topicArgs["interface_name"] = NAME;
    topicArgs["client_id"] = _broker->GetClientId();
    topicArgs["property_name"] = "favorite_country";
    topicArgs["prefix"] = _prefixTopicParam;

    auto topic = stinger::utils::format("{prefix}/prop-only/{service_id}/property/favorite_country/value", topicArgs);
    auto msg = stinger::mqtt::Message::PropertyValue(topic, buf.GetString(), _lastFavoriteCountryPropertyVersion);
    _broker->Publish(msg);
}

void PropOnlyServer::_receiveFavoriteCountryPropertyUpdate(const stinger::mqtt::Message& msg)
{
    rapidjson::Document doc;
    rapidjson::ParseResult ok = doc.Parse(msg.payload.c_str());
    if (!ok) {
        //Log("Could not JSON parse favorite_country property update payload.");
        throw std::runtime_error(rapidjson::GetParseError_En(ok.Code()));
    }

    if (!doc.IsObject() && !doc.IsNull()) {
        throw std::runtime_error("Received favorite_country payload is not an object or null");
    }

    // TODO: Check _lastFavoriteCountryPropertyVersion against optPropertyVersion and
    // reject the update if it's older than what we have.

    // Deserialize 1 values into struct.
    FavoriteCountryProperty tempValue = FavoriteCountryProperty::FromRapidJsonObject(doc);

    { // Scope lock
        std::lock_guard<std::mutex> lock(_favoriteCountryPropertyMutex);
        _favoriteCountryProperty = tempValue;
        _lastFavoriteCountryPropertyVersion++;
    }
    republishFavoriteCountryProperty();
}

void PropOnlyServer::_advertisementThreadLoop()
{
    while (_advertisementThreadRunning) {
        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        std::string timestamp = stinger::utils::timePointToIsoString(now);

        // Build JSON message
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

        doc.AddMember("interface_name", rapidjson::Value("prop-only", allocator), allocator);
        doc.AddMember("instance", rapidjson::Value(_instanceId.c_str(), allocator), allocator);
        doc.AddMember("title", rapidjson::Value("Property Only Interface", allocator), allocator);
        doc.AddMember("version", rapidjson::Value("0.0.1", allocator), allocator);
        doc.AddMember("connection_topic", rapidjson::Value(_broker->GetOnlineTopic().c_str(), allocator), allocator);
        doc.AddMember("timestamp", rapidjson::Value(timestamp.c_str(), allocator), allocator);

        doc.AddMember("prefix", rapidjson::Value(_prefixTopicParam.c_str(), allocator), allocator);

        // Convert to JSON string
        rapidjson::StringBuffer buf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
        doc.Accept(writer);

        std::map<std::string, std::string> topicArgs;
        topicArgs["service_id"] = _instanceId;
        topicArgs["interface_name"] = NAME;
        topicArgs["client_id"] = _broker->GetClientId();
        topicArgs["prefix"] = _prefixTopicParam;

        // Publish to "{prefix}/prop-only/{service_id}/interface"
        std::string topic = stinger::utils::format("{prefix}/prop-only/{service_id}/interface", topicArgs);
        auto msg = stinger::mqtt::Message::ServiceOnline(topic, buf.GetString(), 120);
        _broker->Publish(msg);

        _broker->Log(LOG_INFO, "Published service advertisement to %s", topic.c_str());

        // Wait for 120 seconds or until thread should stop
        // Use smaller sleep intervals to allow quick shutdown
        for (int i = 0; i < 120 && _advertisementThreadRunning; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

} // namespace prop_only

} // namespace gen

} // namespace stinger
