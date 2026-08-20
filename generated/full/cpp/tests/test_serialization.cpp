/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the Full interface.  These exercise the (de)serialization
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

using namespace stinger::gen::full;

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

// --- todayIs signal payload ---
TEST(SignalPayloadSerialization, TodayIsRoundTrip)
{
    TodayIsPayload original{ 42, DayOfTheWeek::SATURDAY };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = TodayIsPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- randomWord signal payload ---
TEST(SignalPayloadSerialization, RandomWordRoundTrip)
{
    RandomWordPayload original{ "apples", std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = RandomWordPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- addNumbers method request arguments ---
TEST(MethodRequestSerialization, AddNumbersRoundTrip)
{
    AddNumbersRequestArguments original{ 42, 42, 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = AddNumbersRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- addNumbers method return values ---
TEST(MethodReturnSerialization, AddNumbersRoundTrip)
{
    AddNumbersReturnValues original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = AddNumbersReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- doSomething method request arguments ---
TEST(MethodRequestSerialization, DoSomethingRoundTrip)
{
    DoSomethingRequestArguments original{ "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = DoSomethingRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- doSomething method return values ---
TEST(MethodReturnSerialization, DoSomethingRoundTrip)
{
    DoSomethingReturnValues original{ "apples", 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = DoSomethingReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- what_time_is_it method request arguments ---
TEST(MethodRequestSerialization, WhatTimeIsItRoundTrip)
{
    WhatTimeIsItRequestArguments original{};
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = WhatTimeIsItRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- what_time_is_it method return values ---
TEST(MethodReturnSerialization, WhatTimeIsItRoundTrip)
{
    WhatTimeIsItReturnValues original{ std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = WhatTimeIsItReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- hold_temperature method request arguments ---
TEST(MethodRequestSerialization, HoldTemperatureRoundTrip)
{
    HoldTemperatureRequestArguments original{ 3.14 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = HoldTemperatureRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- hold_temperature method return values ---
TEST(MethodReturnSerialization, HoldTemperatureRoundTrip)
{
    HoldTemperatureReturnValues original{ true };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = HoldTemperatureReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- favorite_number property ---
TEST(PropertySerialization, FavoriteNumberRoundTrip)
{
    FavoriteNumberProperty original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = FavoriteNumberProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- favorite_foods property ---
TEST(PropertySerialization, FavoriteFoodsRoundTrip)
{
    FavoriteFoodsProperty original{ "apples", 42, std::make_optional(std::string("apples")) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = FavoriteFoodsProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- lunch_menu property ---
TEST(PropertySerialization, LunchMenuRoundTrip)
{
    LunchMenuProperty original{ Lunch{ true, "apples", 3.14, DayOfTheWeek::SATURDAY, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, Lunch{ true, "apples", 3.14, DayOfTheWeek::SATURDAY, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = LunchMenuProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- family_name property ---
TEST(PropertySerialization, FamilyNameRoundTrip)
{
    FamilyNameProperty original{ "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = FamilyNameProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- last_breakfast_time property ---
TEST(PropertySerialization, LastBreakfastTimeRoundTrip)
{
    LastBreakfastTimeProperty original{ std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = LastBreakfastTimeProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- last_birthdays property ---
TEST(PropertySerialization, LastBirthdaysRoundTrip)
{
    LastBirthdaysProperty original{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now(), 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = LastBirthdaysProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}
