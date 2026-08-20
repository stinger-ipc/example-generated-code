/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the SignalOnly client.  These use the MockConnection from
stinger-cpp-utils to drive the client without a real MQTT broker:
  - construction registers the expected topic subscriptions,
  - calling a method publishes a request to the broker,
  - updating a (writable) property publishes an update request,
  - a simulated incoming signal message invokes the registered callback.


*/

#include <gtest/gtest.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <map>
#include <memory>
#include <string>

#include <stinger/utils/mockconnection.hpp>
#include <stinger/utils/format.hpp>
#include <stinger/mqtt/message.hpp>

#include "client.hpp"
#include "discovery.hpp"
#include "structs.hpp"
#include "signal_payloads.hpp"
#include "method_payloads.hpp"
#include "enums.hpp"

using namespace stinger::gen::signal_only;

namespace {

// Values used to construct the client's InstanceInfo and to reconstruct the
// concrete topics the client subscribes/publishes to.
constexpr const char* kClientId = "test-client";
constexpr const char* kServiceId = "test-service";

// Serializes a generated payload/struct (anything exposing AddToRapidJsonObject)
// to a compact JSON string.
template<typename T>
std::string serializeToString(const T& obj)
{
    rapidjson::Document doc;
    doc.SetObject();
    obj.AddToRapidJsonObject(doc, doc.GetAllocator());
    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    doc.Accept(writer);
    return buf.GetString();
}

class SignalOnlyClientTest: public ::testing::Test {
protected:
    std::shared_ptr<stinger::utils::MockConnection> _conn;
    std::unique_ptr<SignalOnlyClient> _client;

    void SetUp() override
    {
        _conn = std::make_shared<stinger::utils::MockConnection>(kClientId);

        InstanceInfo info;
        info.serviceId = kServiceId;
        info.prefix = "test-prefix";

        _client = std::make_unique<SignalOnlyClient>(_conn, info);
    }

    // Mirrors the topic parameters the client uses internally so tests can
    // reconstruct the exact concrete topic strings.
    std::map<std::string, std::string> topicParams() const
    {
        std::map<std::string, std::string> params;
        params["client_id"] = _conn->GetClientId();
        params["service_id"] = kServiceId;
        params["interface_name"] = SignalOnlyClient::NAME;
        params["prefix"] = "test-prefix";
        return params;
    }
};

} // anonymous namespace

// The client should subscribe to one topic per signal, method response,
// and property value.
TEST_F(SignalOnlyClientTest, SubscribesToExpectedTopics)
{
    const std::size_t expected = 5;
    EXPECT_EQ(_conn->GetSubscriptions().size(), expected);
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/anotherSignal", topicParams()))) << "Not subscribed to anotherSignal signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/bark", topicParams()))) << "Not subscribed to bark signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/maybe_number", topicParams()))) << "Not subscribed to maybe_number signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/maybe_name", topicParams()))) << "Not subscribed to maybe_name signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/now", topicParams()))) << "Not subscribed to now signal topic";
}

// A simulated incoming `anotherSignal` signal should be dispatched to the registered callback.
TEST_F(SignalOnlyClientTest, AnotherSignalSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerAnotherSignalCallback([&callbackInvoked](double, bool, std::string)
                                           {
                                               callbackInvoked = true;
                                           });

    AnotherSignalPayload payload{ 3.14, true, "apples" };
    const std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/anotherSignal", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `bark` signal should be dispatched to the registered callback.
TEST_F(SignalOnlyClientTest, BarkSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerBarkCallback([&callbackInvoked](std::string)
                                  {
                                      callbackInvoked = true;
                                  });

    BarkPayload payload{ "apples" };
    const std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/bark", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `maybe_number` signal should be dispatched to the registered callback.
TEST_F(SignalOnlyClientTest, MaybeNumberSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerMaybeNumberCallback([&callbackInvoked](std::optional<int>)
                                         {
                                             callbackInvoked = true;
                                         });

    MaybeNumberPayload payload{ 42 };
    const std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/maybe_number", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `maybe_name` signal should be dispatched to the registered callback.
TEST_F(SignalOnlyClientTest, MaybeNameSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerMaybeNameCallback([&callbackInvoked](std::optional<std::string>)
                                       {
                                           callbackInvoked = true;
                                       });

    MaybeNamePayload payload{ std::make_optional(std::string("apples")) };
    const std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/maybe_name", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `now` signal should be dispatched to the registered callback.
TEST_F(SignalOnlyClientTest, NowSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerNowCallback([&callbackInvoked](std::chrono::time_point<std::chrono::system_clock>)
                                 {
                                     callbackInvoked = true;
                                 });

    NowPayload payload{ std::chrono::system_clock::now() };
    const std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/now", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}
