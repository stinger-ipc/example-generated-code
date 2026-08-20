/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the weather client.  These use the MockConnection from
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

using namespace stinger::gen::weather;

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

class WeatherClientTest: public ::testing::Test {
protected:
    std::shared_ptr<stinger::utils::MockConnection> _conn;
    std::unique_ptr<WeatherClient> _client;

    void SetUp() override
    {
        _conn = std::make_shared<stinger::utils::MockConnection>(kClientId);

        InstanceInfo info;
        info.serviceId = kServiceId;
        info.prefix = "test-prefix";

        _client = std::make_unique<WeatherClient>(_conn, info);
    }

    // Mirrors the topic parameters the client uses internally so tests can
    // reconstruct the exact concrete topic strings.
    std::map<std::string, std::string> topicParams() const
    {
        std::map<std::string, std::string> params;
        params["client_id"] = _conn->GetClientId();
        params["service_id"] = kServiceId;
        params["interface_name"] = WeatherClient::NAME;
        params["prefix"] = "test-prefix";
        return params;
    }
};

} // anonymous namespace

// The client should subscribe to one topic per signal, method response,
// and property value, plus the aggregate property-update response topic.
TEST_F(WeatherClientTest, SubscribesToExpectedTopics)
{
    const std::size_t expected = 13;
    EXPECT_EQ(_conn->GetSubscriptions().size(), expected);
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/signal/current_time", topicParams()))) << "Not subscribed to current_time signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/weather/method/refresh_daily_forecast/response", topicParams()))) << "Not subscribed to refresh_daily_forecast response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/weather/method/refresh_hourly_forecast/response", topicParams()))) << "Not subscribed to refresh_hourly_forecast response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/weather/method/refresh_current_conditions/response", topicParams()))) << "Not subscribed to refresh_current_conditions response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/location/value", topicParams()))) << "Not subscribed to location property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/current_temperature/value", topicParams()))) << "Not subscribed to current_temperature property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/current_condition/value", topicParams()))) << "Not subscribed to current_condition property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/daily_forecast/value", topicParams()))) << "Not subscribed to daily_forecast property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/hourly_forecast/value", topicParams()))) << "Not subscribed to hourly_forecast property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/current_condition_refresh_interval/value", topicParams()))) << "Not subscribed to current_condition_refresh_interval property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/hourly_forecast_refresh_interval/value", topicParams()))) << "Not subscribed to hourly_forecast_refresh_interval property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/daily_forecast_refresh_interval/value", topicParams()))) << "Not subscribed to daily_forecast_refresh_interval property value topic";
}

// A simulated incoming `current_time` signal should be dispatched to the registered callback.
TEST_F(WeatherClientTest, CurrentTimeSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerCurrentTimeCallback([&callbackInvoked](std::string)
                                         {
                                             callbackInvoked = true;
                                         });

    CurrentTimePayload payload{ "apples" };
    const std::string topic = stinger::utils::format("{prefix}/weather/{service_id}/signal/current_time", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// Calling `refresh_daily_forecast` should publish a request to the broker.
TEST_F(WeatherClientTest, RefreshDailyForecastPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->refreshDailyForecast();

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/weather/{service_id}/method/refresh_daily_forecast/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `refresh_hourly_forecast` should publish a request to the broker.
TEST_F(WeatherClientTest, RefreshHourlyForecastPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->refreshHourlyForecast();

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/weather/{service_id}/method/refresh_hourly_forecast/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `refresh_current_conditions` should publish a request to the broker.
TEST_F(WeatherClientTest, RefreshCurrentConditionsPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->refreshCurrentConditions();

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/weather/{service_id}/method/refresh_current_conditions/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `location` should publish an update request to the broker.
TEST_F(WeatherClientTest, LocationUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateLocationProperty(3.14, 3.14);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/weather/{service_id}/property/location/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `current_condition_refresh_interval` should publish an update request to the broker.
TEST_F(WeatherClientTest, CurrentConditionRefreshIntervalUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateCurrentConditionRefreshIntervalProperty(42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/weather/{service_id}/property/current_condition_refresh_interval/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `hourly_forecast_refresh_interval` should publish an update request to the broker.
TEST_F(WeatherClientTest, HourlyForecastRefreshIntervalUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateHourlyForecastRefreshIntervalProperty(42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/weather/{service_id}/property/hourly_forecast_refresh_interval/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `daily_forecast_refresh_interval` should publish an update request to the broker.
TEST_F(WeatherClientTest, DailyForecastRefreshIntervalUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateDailyForecastRefreshIntervalProperty(42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/weather/{service_id}/property/daily_forecast_refresh_interval/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}
