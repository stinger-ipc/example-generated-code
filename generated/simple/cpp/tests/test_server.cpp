/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the Simple server.  These use the MockConnection from
stinger-cpp-utils to drive the server without a real MQTT broker:
  - construction registers the expected request/update subscriptions,
  - emitting a signal publishes it to the broker,
  - a simulated incoming method request invokes the registered handler and publishes a response,
  - setting a property publishes its value, and a simulated update request is re-published.


*/

#include <gtest/gtest.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <stinger/utils/mockconnection.hpp>
#include <stinger/utils/format.hpp>
#include <stinger/mqtt/message.hpp>
#include <stinger/error/return_codes.hpp>

#include "server.hpp"
#include "structs.hpp"
#include "signal_payloads.hpp"
#include "method_payloads.hpp"
#include "enums.hpp"
#include "property_structs.hpp"

using namespace stinger::gen::simple;

namespace {

constexpr const char* kClientId = "test-server-client";
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

// The server starts a service-advertisement thread in its constructor; tearing it
// down joins that thread, which can take up to a second.  To keep the suite fast,
// the server is constructed once per suite and shared across tests.  Each test
// clears the broker's published-message log so assertions stay isolated.
class SimpleServerTest: public ::testing::Test {
protected:
    static std::shared_ptr<stinger::utils::MockConnection> _conn;
    static std::unique_ptr<SimpleServer> _server;

    static void SetUpTestSuite()
    {
        _conn = std::make_shared<stinger::utils::MockConnection>(kClientId);
        _server = std::make_unique<SimpleServer>(_conn, kServiceId, "test-prefix");
    }

    static void TearDownTestSuite()
    {
        // Destroy the server (stops its advertisement thread) before the connection.
        _server.reset();
        _conn.reset();
    }

    void SetUp() override
    {
        _conn->ClearPublishedMessages();
    }

    // Mirrors the topic parameters the server uses internally so tests can
    // reconstruct the exact concrete topic strings.
    static std::map<std::string, std::string> topicArgs()
    {
        std::map<std::string, std::string> params;
        params["service_id"] = kServiceId;
        params["interface_name"] = SimpleServer::NAME;
        params["client_id"] = _conn->GetClientId();
        params["prefix"] = "test-prefix";
        return params;
    }
};

std::shared_ptr<stinger::utils::MockConnection> SimpleServerTest::_conn;
std::unique_ptr<SimpleServer> SimpleServerTest::_server;

} // anonymous namespace

// The server should subscribe to one topic per method request and per property update.
TEST_F(SimpleServerTest, SubscribesToExpectedTopics)
{
    const std::size_t expected = 2;
    EXPECT_EQ(_conn->GetSubscriptions().size(), expected);
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Simple/{service_id}/method/trade_numbers/request", topicArgs()))) << "Not subscribed to trade_numbers request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Simple/{service_id}/property/school/update", topicArgs()))) << "Not subscribed to school update topic";
}

// Emitting `person_entered` should publish the signal to the broker.
TEST_F(SimpleServerTest, PersonEnteredEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/Simple/{service_id}/signal/person_entered", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitPersonEnteredSignal(Person{ "apples", Gender::MALE });

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Setting `school` on the server should publish its value to the broker.
TEST_F(SimpleServerTest, SchoolSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/Simple/{service_id}/property/school/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateSchoolProperty("apples");

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `school` update request should be applied and re-published as the new value.
TEST_F(SimpleServerTest, SchoolUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/Simple/{service_id}/property/school/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/Simple/{service_id}/property/school/value", topicArgs());

    SchoolProperty newValue{ "apples" };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}
