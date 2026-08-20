/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the weather server.  These use the MockConnection from
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

using namespace stinger::gen::weather;

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
class WeatherServerTest: public ::testing::Test {
protected:
    static std::shared_ptr<stinger::utils::MockConnection> _conn;
    static std::unique_ptr<WeatherServer> _server;

    static void SetUpTestSuite()
    {
        _conn = std::make_shared<stinger::utils::MockConnection>(kClientId);
        _server = std::make_unique<WeatherServer>(_conn, kServiceId, "test-prefix");
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
        params["interface_name"] = WeatherServer::NAME;
        params["client_id"] = _conn->GetClientId();
        params["prefix"] = "test-prefix";
        return params;
    }
};

std::shared_ptr<stinger::utils::MockConnection> WeatherServerTest::_conn;
std::unique_ptr<WeatherServer> WeatherServerTest::_server;

} // anonymous namespace

// The server should subscribe to one topic per method request and per property update.
TEST_F(WeatherServerTest, SubscribesToExpectedTopics)
{
    const std::size_t expected = 11;
    EXPECT_EQ(_conn->GetSubscriptions().size(), expected);
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/method/refresh_daily_forecast/request", topicArgs()))) << "Not subscribed to refresh_daily_forecast request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/method/refresh_hourly_forecast/request", topicArgs()))) << "Not subscribed to refresh_hourly_forecast request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/method/refresh_current_conditions/request", topicArgs()))) << "Not subscribed to refresh_current_conditions request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/location/update", topicArgs()))) << "Not subscribed to location update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/current_temperature/update", topicArgs()))) << "Not subscribed to current_temperature update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/current_condition/update", topicArgs()))) << "Not subscribed to current_condition update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/daily_forecast/update", topicArgs()))) << "Not subscribed to daily_forecast update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/hourly_forecast/update", topicArgs()))) << "Not subscribed to hourly_forecast update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/current_condition_refresh_interval/update", topicArgs()))) << "Not subscribed to current_condition_refresh_interval update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/hourly_forecast_refresh_interval/update", topicArgs()))) << "Not subscribed to hourly_forecast_refresh_interval update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/weather/{service_id}/property/daily_forecast_refresh_interval/update", topicArgs()))) << "Not subscribed to daily_forecast_refresh_interval update topic";
}

// Emitting `current_time` should publish the signal to the broker.
TEST_F(WeatherServerTest, CurrentTimeEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/weather/{service_id}/signal/current_time", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitCurrentTimeSignal("apples");

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `refresh_daily_forecast` request should invoke the registered handler and publish a response.
TEST_F(WeatherServerTest, RefreshDailyForecastRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerRefreshDailyForecastHandler([handlerCalled]() -> void
                                                 {
                                                     *handlerCalled = true;
                                                 });

    const std::string payload = "{}";

    const std::string requestTopic = stinger::utils::format("{prefix}/weather/{service_id}/method/refresh_daily_forecast/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/weather/method/refresh_daily_forecast/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `refresh_hourly_forecast` request should invoke the registered handler and publish a response.
TEST_F(WeatherServerTest, RefreshHourlyForecastRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerRefreshHourlyForecastHandler([handlerCalled]() -> void
                                                  {
                                                      *handlerCalled = true;
                                                  });

    const std::string payload = "{}";

    const std::string requestTopic = stinger::utils::format("{prefix}/weather/{service_id}/method/refresh_hourly_forecast/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/weather/method/refresh_hourly_forecast/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `refresh_current_conditions` request should invoke the registered handler and publish a response.
TEST_F(WeatherServerTest, RefreshCurrentConditionsRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerRefreshCurrentConditionsHandler([handlerCalled]() -> void
                                                     {
                                                         *handlerCalled = true;
                                                     });

    const std::string payload = "{}";

    const std::string requestTopic = stinger::utils::format("{prefix}/weather/{service_id}/method/refresh_current_conditions/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/weather/method/refresh_current_conditions/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// Setting `location` on the server should publish its value to the broker.
TEST_F(WeatherServerTest, LocationSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/location/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateLocationProperty(3.14, 3.14);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `location` update request should be applied and re-published as the new value.
TEST_F(WeatherServerTest, LocationUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/location/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/location/value", topicArgs());

    LocationProperty newValue{ 3.14, 3.14 };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `current_temperature` on the server should publish its value to the broker.
TEST_F(WeatherServerTest, CurrentTemperatureSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/current_temperature/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateCurrentTemperatureProperty(3.14);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `current_temperature` update request should be applied and re-published as the new value.
TEST_F(WeatherServerTest, CurrentTemperatureUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/current_temperature/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/current_temperature/value", topicArgs());

    CurrentTemperatureProperty newValue{ 3.14 };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `current_condition` on the server should publish its value to the broker.
TEST_F(WeatherServerTest, CurrentConditionSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/current_condition/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateCurrentConditionProperty(WeatherCondition::SNOWY, "apples");

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `current_condition` update request should be applied and re-published as the new value.
TEST_F(WeatherServerTest, CurrentConditionUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/current_condition/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/current_condition/value", topicArgs());

    CurrentConditionProperty newValue{ WeatherCondition::SNOWY, "apples" };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `daily_forecast` on the server should publish its value to the broker.
TEST_F(WeatherServerTest, DailyForecastSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/daily_forecast/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateDailyForecastProperty(ForecastForDay{ 3.14, 3.14, WeatherCondition::SNOWY, "apples", "apples" }, ForecastForDay{ 3.14, 3.14, WeatherCondition::SNOWY, "apples", "apples" }, ForecastForDay{ 3.14, 3.14, WeatherCondition::SNOWY, "apples", "apples" });

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `daily_forecast` update request should be applied and re-published as the new value.
TEST_F(WeatherServerTest, DailyForecastUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/daily_forecast/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/daily_forecast/value", topicArgs());

    DailyForecastProperty newValue{ ForecastForDay{ 3.14, 3.14, WeatherCondition::SNOWY, "apples", "apples" }, ForecastForDay{ 3.14, 3.14, WeatherCondition::SNOWY, "apples", "apples" }, ForecastForDay{ 3.14, 3.14, WeatherCondition::SNOWY, "apples", "apples" } };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `hourly_forecast` on the server should publish its value to the broker.
TEST_F(WeatherServerTest, HourlyForecastSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/hourly_forecast/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateHourlyForecastProperty(ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY }, ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY }, ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY }, ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY });

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `hourly_forecast` update request should be applied and re-published as the new value.
TEST_F(WeatherServerTest, HourlyForecastUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/hourly_forecast/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/hourly_forecast/value", topicArgs());

    HourlyForecastProperty newValue{ ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY }, ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY }, ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY }, ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY } };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `current_condition_refresh_interval` on the server should publish its value to the broker.
TEST_F(WeatherServerTest, CurrentConditionRefreshIntervalSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/current_condition_refresh_interval/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateCurrentConditionRefreshIntervalProperty(42);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `current_condition_refresh_interval` update request should be applied and re-published as the new value.
TEST_F(WeatherServerTest, CurrentConditionRefreshIntervalUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/current_condition_refresh_interval/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/current_condition_refresh_interval/value", topicArgs());

    CurrentConditionRefreshIntervalProperty newValue{ 42 };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `hourly_forecast_refresh_interval` on the server should publish its value to the broker.
TEST_F(WeatherServerTest, HourlyForecastRefreshIntervalSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/hourly_forecast_refresh_interval/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateHourlyForecastRefreshIntervalProperty(42);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `hourly_forecast_refresh_interval` update request should be applied and re-published as the new value.
TEST_F(WeatherServerTest, HourlyForecastRefreshIntervalUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/hourly_forecast_refresh_interval/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/hourly_forecast_refresh_interval/value", topicArgs());

    HourlyForecastRefreshIntervalProperty newValue{ 42 };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `daily_forecast_refresh_interval` on the server should publish its value to the broker.
TEST_F(WeatherServerTest, DailyForecastRefreshIntervalSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/daily_forecast_refresh_interval/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateDailyForecastRefreshIntervalProperty(42);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `daily_forecast_refresh_interval` update request should be applied and re-published as the new value.
TEST_F(WeatherServerTest, DailyForecastRefreshIntervalUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/daily_forecast_refresh_interval/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/weather/{service_id}/property/daily_forecast_refresh_interval/value", topicArgs());

    DailyForecastRefreshIntervalProperty newValue{ 42 };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}
