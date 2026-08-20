/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the SignalOnly server.  These use the MockConnection from
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

using namespace stinger::gen::signal_only;

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
class SignalOnlyServerTest: public ::testing::Test {
protected:
    static std::shared_ptr<stinger::utils::MockConnection> _conn;
    static std::unique_ptr<SignalOnlyServer> _server;

    static void SetUpTestSuite()
    {
        _conn = std::make_shared<stinger::utils::MockConnection>(kClientId);
        _server = std::make_unique<SignalOnlyServer>(_conn, kServiceId, "test-prefix");
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
        params["interface_name"] = SignalOnlyServer::NAME;
        params["client_id"] = _conn->GetClientId();
        params["prefix"] = "test-prefix";
        return params;
    }
};

std::shared_ptr<stinger::utils::MockConnection> SignalOnlyServerTest::_conn;
std::unique_ptr<SignalOnlyServer> SignalOnlyServerTest::_server;

} // anonymous namespace

// The server should subscribe to one topic per method request and per property update.
TEST_F(SignalOnlyServerTest, SubscribesToExpectedTopics)
{
    const std::size_t expected = 0;
    EXPECT_EQ(_conn->GetSubscriptions().size(), expected);
}

// Emitting `anotherSignal` should publish the signal to the broker.
TEST_F(SignalOnlyServerTest, AnotherSignalEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/anotherSignal", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitAnotherSignalSignal(3.14, true, "apples");

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `bark` should publish the signal to the broker.
TEST_F(SignalOnlyServerTest, BarkEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/bark", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitBarkSignal("apples");

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `maybe_number` should publish the signal to the broker.
TEST_F(SignalOnlyServerTest, MaybeNumberEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/maybe_number", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitMaybeNumberSignal(42);

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `maybe_name` should publish the signal to the broker.
TEST_F(SignalOnlyServerTest, MaybeNameEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/maybe_name", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitMaybeNameSignal(std::make_optional(std::string("apples")));

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `now` should publish the signal to the broker.
TEST_F(SignalOnlyServerTest, NowEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/SignalOnly/{service_id}/signal/now", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitNowSignal(std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}
