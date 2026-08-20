/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the Simple client.  These use the MockConnection from
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
#include "property_structs.hpp"

using namespace stinger::gen::simple;

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

class SimpleClientTest: public ::testing::Test {
protected:
    std::shared_ptr<stinger::utils::MockConnection> _conn;
    std::unique_ptr<SimpleClient> _client;

    void SetUp() override
    {
        _conn = std::make_shared<stinger::utils::MockConnection>(kClientId);

        InstanceInfo info;
        info.serviceId = kServiceId;
        info.prefix = "test-prefix";

        _client = std::make_unique<SimpleClient>(_conn, info);
    }

    // Mirrors the topic parameters the client uses internally so tests can
    // reconstruct the exact concrete topic strings.
    std::map<std::string, std::string> topicParams() const
    {
        std::map<std::string, std::string> params;
        params["client_id"] = _conn->GetClientId();
        params["service_id"] = kServiceId;
        params["interface_name"] = SimpleClient::NAME;
        params["prefix"] = "test-prefix";
        return params;
    }
};

} // anonymous namespace

// The client should subscribe to one topic per signal, method response,
// and property value, plus the aggregate property-update response topic.
TEST_F(SimpleClientTest, SubscribesToExpectedTopics)
{
    const std::size_t expected = 4;
    EXPECT_EQ(_conn->GetSubscriptions().size(), expected);
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Simple/{service_id}/signal/person_entered", topicParams()))) << "Not subscribed to person_entered signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/Simple/method/trade_numbers/response", topicParams()))) << "Not subscribed to trade_numbers response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Simple/{service_id}/property/school/value", topicParams()))) << "Not subscribed to school property value topic";
}

// A simulated incoming `person_entered` signal should be dispatched to the registered callback.
TEST_F(SimpleClientTest, PersonEnteredSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerPersonEnteredCallback([&callbackInvoked](Person)
                                           {
                                               callbackInvoked = true;
                                           });

    PersonEnteredPayload payload{ Person{ "apples", Gender::MALE } };
    const std::string topic = stinger::utils::format("{prefix}/Simple/{service_id}/signal/person_entered", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// Updating `school` should publish an update request to the broker.
TEST_F(SimpleClientTest, SchoolUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateSchoolProperty("apples");

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/Simple/{service_id}/property/school/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}
