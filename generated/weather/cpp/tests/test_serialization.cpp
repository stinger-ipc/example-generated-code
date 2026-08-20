/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the weather interface.  These exercise the (de)serialization
round-trip and schema-validation code paths of the generated payload structures without
requiring a broker connection.


*/

#include <gtest/gtest.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>

#include "structs.hpp"
#include "signal_payloads.hpp"
#include "method_payloads.hpp"
#include "enums.hpp"
#include "property_structs.hpp"

using namespace stinger::gen::weather;

namespace {

// Serializes any generated payload/struct (anything exposing AddToRapidJsonObject)
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

} // anonymous namespace

// --- current_time signal payload ---
TEST(SignalPayloadSerialization, CurrentTimeRoundTrip)
{
    CurrentTimePayload original{ "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CurrentTimePayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- refresh_daily_forecast method request arguments ---
TEST(MethodRequestSerialization, RefreshDailyForecastRoundTrip)
{
    RefreshDailyForecastRequestArguments original{};
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = RefreshDailyForecastRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- refresh_daily_forecast method return values ---
TEST(MethodReturnSerialization, RefreshDailyForecastRoundTrip)
{
    RefreshDailyForecastReturnValues original{};
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = RefreshDailyForecastReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- refresh_hourly_forecast method request arguments ---
TEST(MethodRequestSerialization, RefreshHourlyForecastRoundTrip)
{
    RefreshHourlyForecastRequestArguments original{};
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = RefreshHourlyForecastRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- refresh_hourly_forecast method return values ---
TEST(MethodReturnSerialization, RefreshHourlyForecastRoundTrip)
{
    RefreshHourlyForecastReturnValues original{};
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = RefreshHourlyForecastReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- refresh_current_conditions method request arguments ---
TEST(MethodRequestSerialization, RefreshCurrentConditionsRoundTrip)
{
    RefreshCurrentConditionsRequestArguments original{};
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = RefreshCurrentConditionsRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- refresh_current_conditions method return values ---
TEST(MethodReturnSerialization, RefreshCurrentConditionsRoundTrip)
{
    RefreshCurrentConditionsReturnValues original{};
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = RefreshCurrentConditionsReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- location property ---
TEST(PropertySerialization, LocationRoundTrip)
{
    LocationProperty original{ 3.14, 3.14 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = LocationProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- current_temperature property ---
TEST(PropertySerialization, CurrentTemperatureRoundTrip)
{
    CurrentTemperatureProperty original{ 3.14 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CurrentTemperatureProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- current_condition property ---
TEST(PropertySerialization, CurrentConditionRoundTrip)
{
    CurrentConditionProperty original{ WeatherCondition::SNOWY, "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CurrentConditionProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- daily_forecast property ---
TEST(PropertySerialization, DailyForecastRoundTrip)
{
    DailyForecastProperty original{ ForecastForDay{ 3.14, 3.14, WeatherCondition::SNOWY, "apples", "apples" }, ForecastForDay{ 3.14, 3.14, WeatherCondition::SNOWY, "apples", "apples" }, ForecastForDay{ 3.14, 3.14, WeatherCondition::SNOWY, "apples", "apples" } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = DailyForecastProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- hourly_forecast property ---
TEST(PropertySerialization, HourlyForecastRoundTrip)
{
    HourlyForecastProperty original{ ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY }, ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY }, ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY }, ForecastForHour{ 3.14, std::chrono::system_clock::now(), WeatherCondition::SNOWY } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = HourlyForecastProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- current_condition_refresh_interval property ---
TEST(PropertySerialization, CurrentConditionRefreshIntervalRoundTrip)
{
    CurrentConditionRefreshIntervalProperty original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CurrentConditionRefreshIntervalProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- hourly_forecast_refresh_interval property ---
TEST(PropertySerialization, HourlyForecastRefreshIntervalRoundTrip)
{
    HourlyForecastRefreshIntervalProperty original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = HourlyForecastRefreshIntervalProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- daily_forecast_refresh_interval property ---
TEST(PropertySerialization, DailyForecastRefreshIntervalRoundTrip)
{
    DailyForecastRefreshIntervalProperty original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = DailyForecastRefreshIntervalProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}
