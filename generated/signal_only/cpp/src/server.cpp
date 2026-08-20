
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
#include "signal_payloads.hpp"
#include "enums.hpp"
#include <stinger/utils/iconnection.hpp>
#include <stinger/utils/format.hpp>
#include <stinger/error/return_codes.hpp>

namespace stinger {

namespace gen {
namespace signal_only {

constexpr const char SignalOnlyServer::NAME[];
constexpr const char SignalOnlyServer::INTERFACE_VERSION[];

SignalOnlyServer::SignalOnlyServer(std::shared_ptr<stinger::utils::IConnection> broker, const std::string& instanceId, const std::string& prefix):
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

    // Start the service advertisement thread
    _advertisementThreadRunning = true;
    _advertisementThread = std::thread(&SignalOnlyServer::_advertisementThreadLoop, this);
}

SignalOnlyServer::~SignalOnlyServer()
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

    std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/interface", topicArgs);
    auto msg = stinger::mqtt::Message::ServiceOffline(topic);
    _broker->Publish(msg);
}

void SignalOnlyServer::_receiveMessage(const stinger::mqtt::Message& msg)
{
    const int noSubId = -1;
    int subscriptionId = msg.properties.subscriptionId.value_or(noSubId);
}

std::future<bool> SignalOnlyServer::emitAnotherSignalSignal(double one, bool two, std::string three)
{
    AnotherSignalPayload signalPayload{ one, two, three };
    if (!signalPayload.ValidateSchema()) {
        _broker->Log(LOG_WARNING, "Payload for 'anotherSignal' signal failed schema validation; not emitting.");
        std::promise<bool> failedPromise;
        failedPromise.set_value(false);
        return failedPromise.get_future();
    }

    rapidjson::Document doc;
    doc.SetObject();
    signalPayload.AddToRapidJsonObject(doc, doc.GetAllocator());

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    doc.Accept(writer);

    std::map<std::string, std::string> topicArgs;
    topicArgs["service_id"] = _instanceId;
    topicArgs["interface_name"] = NAME;
    topicArgs["client_id"] = _broker->GetClientId();
    topicArgs["signal_name"] = "anotherSignal";
    topicArgs["prefix"] = _prefixTopicParam;
    auto topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/anotherSignal", topicArgs);
    auto msg = stinger::mqtt::Message::Signal(topic, buf.GetString());
    return _broker->Publish(msg);
}

std::future<bool> SignalOnlyServer::emitBarkSignal(std::string word)
{
    BarkPayload signalPayload{ word };
    if (!signalPayload.ValidateSchema()) {
        _broker->Log(LOG_WARNING, "Payload for 'bark' signal failed schema validation; not emitting.");
        std::promise<bool> failedPromise;
        failedPromise.set_value(false);
        return failedPromise.get_future();
    }

    rapidjson::Document doc;
    doc.SetObject();
    signalPayload.AddToRapidJsonObject(doc, doc.GetAllocator());

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    doc.Accept(writer);

    std::map<std::string, std::string> topicArgs;
    topicArgs["service_id"] = _instanceId;
    topicArgs["interface_name"] = NAME;
    topicArgs["client_id"] = _broker->GetClientId();
    topicArgs["signal_name"] = "bark";
    topicArgs["prefix"] = _prefixTopicParam;
    auto topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/bark", topicArgs);
    auto msg = stinger::mqtt::Message::Signal(topic, buf.GetString());
    return _broker->Publish(msg);
}

std::future<bool> SignalOnlyServer::emitMaybeNumberSignal(std::optional<int> number)
{
    MaybeNumberPayload signalPayload{ number };
    if (!signalPayload.ValidateSchema()) {
        _broker->Log(LOG_WARNING, "Payload for 'maybe_number' signal failed schema validation; not emitting.");
        std::promise<bool> failedPromise;
        failedPromise.set_value(false);
        return failedPromise.get_future();
    }

    rapidjson::Document doc;
    doc.SetObject();
    signalPayload.AddToRapidJsonObject(doc, doc.GetAllocator());

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    doc.Accept(writer);

    std::map<std::string, std::string> topicArgs;
    topicArgs["service_id"] = _instanceId;
    topicArgs["interface_name"] = NAME;
    topicArgs["client_id"] = _broker->GetClientId();
    topicArgs["signal_name"] = "maybe_number";
    topicArgs["prefix"] = _prefixTopicParam;
    auto topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/maybe_number", topicArgs);
    auto msg = stinger::mqtt::Message::Signal(topic, buf.GetString());
    return _broker->Publish(msg);
}

std::future<bool> SignalOnlyServer::emitMaybeNameSignal(std::optional<std::string> name)
{
    MaybeNamePayload signalPayload{ name };
    if (!signalPayload.ValidateSchema()) {
        _broker->Log(LOG_WARNING, "Payload for 'maybe_name' signal failed schema validation; not emitting.");
        std::promise<bool> failedPromise;
        failedPromise.set_value(false);
        return failedPromise.get_future();
    }

    rapidjson::Document doc;
    doc.SetObject();
    signalPayload.AddToRapidJsonObject(doc, doc.GetAllocator());

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    doc.Accept(writer);

    std::map<std::string, std::string> topicArgs;
    topicArgs["service_id"] = _instanceId;
    topicArgs["interface_name"] = NAME;
    topicArgs["client_id"] = _broker->GetClientId();
    topicArgs["signal_name"] = "maybe_name";
    topicArgs["prefix"] = _prefixTopicParam;
    auto topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/maybe_name", topicArgs);
    auto msg = stinger::mqtt::Message::Signal(topic, buf.GetString());
    return _broker->Publish(msg);
}

std::future<bool> SignalOnlyServer::emitNowSignal(std::chrono::time_point<std::chrono::system_clock> timestamp)
{
    NowPayload signalPayload{ timestamp };
    if (!signalPayload.ValidateSchema()) {
        _broker->Log(LOG_WARNING, "Payload for 'now' signal failed schema validation; not emitting.");
        std::promise<bool> failedPromise;
        failedPromise.set_value(false);
        return failedPromise.get_future();
    }

    rapidjson::Document doc;
    doc.SetObject();
    signalPayload.AddToRapidJsonObject(doc, doc.GetAllocator());

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    doc.Accept(writer);

    std::map<std::string, std::string> topicArgs;
    topicArgs["service_id"] = _instanceId;
    topicArgs["interface_name"] = NAME;
    topicArgs["client_id"] = _broker->GetClientId();
    topicArgs["signal_name"] = "now";
    topicArgs["prefix"] = _prefixTopicParam;
    auto topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/now", topicArgs);
    auto msg = stinger::mqtt::Message::Signal(topic, buf.GetString());
    return _broker->Publish(msg);
}

void SignalOnlyServer::_advertisementThreadLoop()
{
    while (_advertisementThreadRunning) {
        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        std::string timestamp = stinger::utils::timePointToIsoString(now);

        // Build JSON message
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

        doc.AddMember("interface_name", rapidjson::Value("SignalOnly", allocator), allocator);
        doc.AddMember("instance", rapidjson::Value(_instanceId.c_str(), allocator), allocator);
        doc.AddMember("title", rapidjson::Value("SignalOnly", allocator), allocator);
        doc.AddMember("version", rapidjson::Value("0.0.1", allocator), allocator);
        doc.AddMember("connection_topic", rapidjson::Value(_broker->GetOnlineTopic().c_str(), allocator), allocator);
        doc.AddMember("timestamp", rapidjson::Value(timestamp.c_str(), allocator), allocator);

        doc.AddMember("prefix", rapidjson::Value(_prefixTopicParam.c_str(), allocator), allocator);

        {
            rapidjson::Value methodsObj(rapidjson::kObjectType);

            doc.AddMember("methods", methodsObj, allocator);
        }

        // Convert to JSON string
        rapidjson::StringBuffer buf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
        doc.Accept(writer);

        std::map<std::string, std::string> topicArgs;
        topicArgs["service_id"] = _instanceId;
        topicArgs["interface_name"] = NAME;
        topicArgs["client_id"] = _broker->GetClientId();
        topicArgs["prefix"] = _prefixTopicParam;

        // Publish to "{prefix}/SignalOnly/{service_id}/interface"
        std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/interface", topicArgs);
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

} // namespace signal_only

} // namespace gen

} // namespace stinger
