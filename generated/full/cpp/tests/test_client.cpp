/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the Full client.  These use the MockConnection from
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

using namespace stinger::gen::full;

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

class FullClientTest: public ::testing::Test {
protected:
    std::shared_ptr<stinger::utils::MockConnection> _conn;
    std::unique_ptr<FullClient> _client;

    void SetUp() override
    {
        _conn = std::make_shared<stinger::utils::MockConnection>(kClientId);

        InstanceInfo info;
        info.serviceId = kServiceId;
        info.prefix = "test-prefix";

        _client = std::make_unique<FullClient>(_conn, info);
    }

    // Mirrors the topic parameters the client uses internally so tests can
    // reconstruct the exact concrete topic strings.
    std::map<std::string, std::string> topicParams() const
    {
        std::map<std::string, std::string> params;
        params["client_id"] = _conn->GetClientId();
        params["service_id"] = kServiceId;
        params["interface_name"] = FullClient::NAME;
        params["prefix"] = "test-prefix";
        return params;
    }
};

} // anonymous namespace

// The client should subscribe to one topic per signal, method response,
// and property value, plus the aggregate property-update response topic.
TEST_F(FullClientTest, SubscribesToExpectedTopics)
{
    const std::size_t expected = 13;
    EXPECT_EQ(_conn->GetSubscriptions().size(), expected);
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/signal/todayIs", topicParams()))) << "Not subscribed to todayIs signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/signal/randomWord", topicParams()))) << "Not subscribed to randomWord signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/Full/method/addNumbers/response", topicParams()))) << "Not subscribed to addNumbers response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/Full/method/doSomething/response", topicParams()))) << "Not subscribed to doSomething response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/Full/method/what_time_is_it/response", topicParams()))) << "Not subscribed to what_time_is_it response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/Full/method/hold_temperature/response", topicParams()))) << "Not subscribed to hold_temperature response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_number/value", topicParams()))) << "Not subscribed to favorite_number property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_foods/value", topicParams()))) << "Not subscribed to favorite_foods property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/lunch_menu/value", topicParams()))) << "Not subscribed to lunch_menu property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/family_name/value", topicParams()))) << "Not subscribed to family_name property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/last_breakfast_time/value", topicParams()))) << "Not subscribed to last_breakfast_time property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/Full/{service_id}/property/last_birthdays/value", topicParams()))) << "Not subscribed to last_birthdays property value topic";
}

// A simulated incoming `todayIs` signal should be dispatched to the registered callback.
TEST_F(FullClientTest, TodayIsSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerTodayIsCallback([&callbackInvoked](int, DayOfTheWeek)
                                     {
                                         callbackInvoked = true;
                                     });

    TodayIsPayload payload{ 42, DayOfTheWeek::SATURDAY };
    const std::string topic = stinger::utils::format("{prefix}/Full/{service_id}/signal/todayIs", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `randomWord` signal should be dispatched to the registered callback.
TEST_F(FullClientTest, RandomWordSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerRandomWordCallback([&callbackInvoked](std::string, std::chrono::time_point<std::chrono::system_clock>)
                                        {
                                            callbackInvoked = true;
                                        });

    RandomWordPayload payload{ "apples", std::chrono::system_clock::now() };
    const std::string topic = stinger::utils::format("{prefix}/Full/{service_id}/signal/randomWord", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// Calling `addNumbers` should publish a request to the broker.
TEST_F(FullClientTest, AddNumbersPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->addNumbers(42, 42, 42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/Full/{service_id}/method/addNumbers/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `doSomething` should publish a request to the broker.
TEST_F(FullClientTest, DoSomethingPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->doSomething("apples");

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/Full/{service_id}/method/doSomething/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `what_time_is_it` should publish a request to the broker.
TEST_F(FullClientTest, WhatTimeIsItPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->whatTimeIsIt();

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/Full/{service_id}/method/what_time_is_it/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `hold_temperature` should publish a request to the broker.
TEST_F(FullClientTest, HoldTemperaturePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->holdTemperature(3.14);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/Full/{service_id}/method/hold_temperature/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `favorite_number` should publish an update request to the broker.
TEST_F(FullClientTest, FavoriteNumberUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateFavoriteNumberProperty(42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_number/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `favorite_foods` should publish an update request to the broker.
TEST_F(FullClientTest, FavoriteFoodsUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateFavoriteFoodsProperty("apples", 42, std::make_optional(std::string("apples")));

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/Full/{service_id}/property/favorite_foods/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `family_name` should publish an update request to the broker.
TEST_F(FullClientTest, FamilyNameUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateFamilyNameProperty("apples");

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/Full/{service_id}/property/family_name/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `last_breakfast_time` should publish an update request to the broker.
TEST_F(FullClientTest, LastBreakfastTimeUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateLastBreakfastTimeProperty(std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/Full/{service_id}/property/last_breakfast_time/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `last_birthdays` should publish an update request to the broker.
TEST_F(FullClientTest, LastBirthdaysUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateLastBirthdaysProperty(std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now(), 42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/Full/{service_id}/property/last_birthdays/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}
