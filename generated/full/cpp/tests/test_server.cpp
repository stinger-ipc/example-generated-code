/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the Full server.  These use the MockConnection from
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

using namespace stinger::gen::full;

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
class FullServerTest: public ::testing::Test {
protected:
    static std::shared_ptr<stinger::utils::MockConnection> _conn;
    static std::unique_ptr<FullServer> _server;

    static void SetUpTestSuite()
    {
        _conn = std::make_shared<stinger::utils::MockConnection>(kClientId);
        _server = std::make_unique<FullServer>(_conn, kServiceId, "test-prefix");
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
        params["interface_name"] = FullServer::NAME;
        params["client_id"] = _conn->GetClientId();
        params["prefix"] = "test-prefix";
        return params;
    }
};

std::shared_ptr<stinger::utils::MockConnection> FullServerTest::_conn;
std::unique_ptr<FullServer> FullServerTest::_server;

} // anonymous namespace

// The server should subscribe to one topic per method request and per property update.
TEST_F(FullServerTest, SubscribesToExpectedTopics)
{
    const std::size_t expected = 10;
    EXPECT_EQ(_conn->GetSubscriptions().size(), expected);
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/method/addNumbers/request", topicArgs()))) << "Not subscribed to addNumbers request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/method/doSomething/request", topicArgs()))) << "Not subscribed to doSomething request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/method/what_time_is_it/request", topicArgs()))) << "Not subscribed to what_time_is_it request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/method/hold_temperature/request", topicArgs()))) << "Not subscribed to hold_temperature request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_number/update", topicArgs()))) << "Not subscribed to favorite_number update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_foods/update", topicArgs()))) << "Not subscribed to favorite_foods update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/lunch_menu/update", topicArgs()))) << "Not subscribed to lunch_menu update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/family_name/update", topicArgs()))) << "Not subscribed to family_name update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/last_breakfast_time/update", topicArgs()))) << "Not subscribed to last_breakfast_time update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/last_birthdays/update", topicArgs()))) << "Not subscribed to last_birthdays update topic";
}

// Emitting `todayIs` should publish the signal to the broker.
TEST_F(FullServerTest, TodayIsEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/Full/{service_id}/signal/todayIs", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitTodayIsSignal(42, DayOfTheWeek::SATURDAY);

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `randomWord` should publish the signal to the broker.
TEST_F(FullServerTest, RandomWordEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/Full/{service_id}/signal/randomWord", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitRandomWordSignal("apples", std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `addNumbers` request should invoke the registered handler and publish a response.
TEST_F(FullServerTest, AddNumbersRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerAddNumbersHandler([handlerCalled](int, int, std::optional<int>) -> int
                                       {
                                           *handlerCalled = true;
                                           return 42;
                                       });

    AddNumbersRequestArguments requestArgs{ 42, 42, 42 };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/Full/{service_id}/method/addNumbers/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/Full/method/addNumbers/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `doSomething` request should invoke the registered handler and publish a response.
TEST_F(FullServerTest, DoSomethingRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerDoSomethingHandler([handlerCalled](std::string) -> DoSomethingReturnValues
                                        {
                                            *handlerCalled = true;
                                            return DoSomethingReturnValues{ "apples", 42 };
                                        });

    DoSomethingRequestArguments requestArgs{ "apples" };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/Full/{service_id}/method/doSomething/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/Full/method/doSomething/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `what_time_is_it` request should invoke the registered handler and publish a response.
TEST_F(FullServerTest, WhatTimeIsItRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerWhatTimeIsItHandler([handlerCalled]() -> std::chrono::time_point<std::chrono::system_clock>
                                         {
                                             *handlerCalled = true;
                                             return std::chrono::system_clock::now();
                                         });

    const std::string payload = "{}";

    const std::string requestTopic = stinger::utils::format("{prefix}/Full/{service_id}/method/what_time_is_it/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/Full/method/what_time_is_it/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `hold_temperature` request should invoke the registered handler and publish a response.
TEST_F(FullServerTest, HoldTemperatureRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerHoldTemperatureHandler([handlerCalled](double) -> bool
                                            {
                                                *handlerCalled = true;
                                                return true;
                                            });

    HoldTemperatureRequestArguments requestArgs{ 3.14 };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/Full/{service_id}/method/hold_temperature/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/Full/method/hold_temperature/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// Setting `favorite_number` on the server should publish its value to the broker.
TEST_F(FullServerTest, FavoriteNumberSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_number/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateFavoriteNumberProperty(42);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `favorite_number` update request should be applied and re-published as the new value.
TEST_F(FullServerTest, FavoriteNumberUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_number/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_number/value", topicArgs());

    FavoriteNumberProperty newValue{ 42 };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `favorite_foods` on the server should publish its value to the broker.
TEST_F(FullServerTest, FavoriteFoodsSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_foods/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateFavoriteFoodsProperty("apples", 42, std::make_optional(std::string("apples")));

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `favorite_foods` update request should be applied and re-published as the new value.
TEST_F(FullServerTest, FavoriteFoodsUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_foods/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_foods/value", topicArgs());

    FavoriteFoodsProperty newValue{ "apples", 42, std::make_optional(std::string("apples")) };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `lunch_menu` on the server should publish its value to the broker.
TEST_F(FullServerTest, LunchMenuSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/lunch_menu/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateLunchMenuProperty(Lunch{ true, "apples", 3.14, DayOfTheWeek::SATURDAY, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, Lunch{ true, "apples", 3.14, DayOfTheWeek::SATURDAY, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) });

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `lunch_menu` update request should be applied and re-published as the new value.
TEST_F(FullServerTest, LunchMenuUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/lunch_menu/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/lunch_menu/value", topicArgs());

    LunchMenuProperty newValue{ Lunch{ true, "apples", 3.14, DayOfTheWeek::SATURDAY, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, Lunch{ true, "apples", 3.14, DayOfTheWeek::SATURDAY, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) } };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `family_name` on the server should publish its value to the broker.
TEST_F(FullServerTest, FamilyNameSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/family_name/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateFamilyNameProperty("apples");

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `family_name` update request should be applied and re-published as the new value.
TEST_F(FullServerTest, FamilyNameUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/family_name/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/family_name/value", topicArgs());

    FamilyNameProperty newValue{ "apples" };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `last_breakfast_time` on the server should publish its value to the broker.
TEST_F(FullServerTest, LastBreakfastTimeSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/last_breakfast_time/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateLastBreakfastTimeProperty(std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `last_breakfast_time` update request should be applied and re-published as the new value.
TEST_F(FullServerTest, LastBreakfastTimeUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/last_breakfast_time/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/last_breakfast_time/value", topicArgs());

    LastBreakfastTimeProperty newValue{ std::chrono::system_clock::now() };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `last_birthdays` on the server should publish its value to the broker.
TEST_F(FullServerTest, LastBirthdaysSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/last_birthdays/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateLastBirthdaysProperty(std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now(), 42);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `last_birthdays` update request should be applied and re-published as the new value.
TEST_F(FullServerTest, LastBirthdaysUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/last_birthdays/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/Full/{service_id}/property/last_birthdays/value", topicArgs());

    LastBirthdaysProperty newValue{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now(), 42 };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}
