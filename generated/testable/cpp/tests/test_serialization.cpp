/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the testable interface.  These exercise the (de)serialization
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

using namespace stinger::gen::testable;

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

// --- empty signal payload ---
TEST(SignalPayloadSerialization, EmptyRoundTrip)
{
    EmptyPayload original{};
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = EmptyPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleInt signal payload ---
TEST(SignalPayloadSerialization, SingleIntRoundTrip)
{
    SingleIntPayload original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleIntPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- jsonSchemaValidatedInt signal payload ---
TEST(SignalPayloadSerialization, JsonSchemaValidatedIntRoundTrip)
{
    JsonSchemaValidatedIntPayload original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = JsonSchemaValidatedIntPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleOptionalInt signal payload ---
TEST(SignalPayloadSerialization, SingleOptionalIntRoundTrip)
{
    SingleOptionalIntPayload original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleOptionalIntPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- threeIntegers signal payload ---
TEST(SignalPayloadSerialization, ThreeIntegersRoundTrip)
{
    ThreeIntegersPayload original{ 42, 42, 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ThreeIntegersPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleString signal payload ---
TEST(SignalPayloadSerialization, SingleStringRoundTrip)
{
    SingleStringPayload original{ "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleStringPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- jsonSchemaValidatedString signal payload ---
TEST(SignalPayloadSerialization, JsonSchemaValidatedStringRoundTrip)
{
    JsonSchemaValidatedStringPayload original{ "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = JsonSchemaValidatedStringPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleOptionalString signal payload ---
TEST(SignalPayloadSerialization, SingleOptionalStringRoundTrip)
{
    SingleOptionalStringPayload original{ std::make_optional(std::string("apples")) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleOptionalStringPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- threeStrings signal payload ---
TEST(SignalPayloadSerialization, ThreeStringsRoundTrip)
{
    ThreeStringsPayload original{ "apples", "apples", std::make_optional(std::string("apples")) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ThreeStringsPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleEnum signal payload ---
TEST(SignalPayloadSerialization, SingleEnumRoundTrip)
{
    SingleEnumPayload original{ Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleEnumPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleOptionalEnum signal payload ---
TEST(SignalPayloadSerialization, SingleOptionalEnumRoundTrip)
{
    SingleOptionalEnumPayload original{ Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleOptionalEnumPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- threeEnums signal payload ---
TEST(SignalPayloadSerialization, ThreeEnumsRoundTrip)
{
    ThreeEnumsPayload original{ Numbers::ONE, Numbers::ONE, Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ThreeEnumsPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleStruct signal payload ---
TEST(SignalPayloadSerialization, SingleStructRoundTrip)
{
    SingleStructPayload original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleStructPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleOptionalStruct signal payload ---
TEST(SignalPayloadSerialization, SingleOptionalStructRoundTrip)
{
    SingleOptionalStructPayload original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleOptionalStructPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- threeStructs signal payload ---
TEST(SignalPayloadSerialization, ThreeStructsRoundTrip)
{
    ThreeStructsPayload original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ThreeStructsPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleDateTime signal payload ---
TEST(SignalPayloadSerialization, SingleDateTimeRoundTrip)
{
    SingleDateTimePayload original{ std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleDateTimePayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleOptionalDatetime signal payload ---
TEST(SignalPayloadSerialization, SingleOptionalDatetimeRoundTrip)
{
    SingleOptionalDatetimePayload original{ std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleOptionalDatetimePayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- threeDateTimes signal payload ---
TEST(SignalPayloadSerialization, ThreeDateTimesRoundTrip)
{
    ThreeDateTimesPayload original{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ThreeDateTimesPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleDuration signal payload ---
TEST(SignalPayloadSerialization, SingleDurationRoundTrip)
{
    SingleDurationPayload original{ std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleDurationPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleOptionalDuration signal payload ---
TEST(SignalPayloadSerialization, SingleOptionalDurationRoundTrip)
{
    SingleOptionalDurationPayload original{ std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleOptionalDurationPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- threeDurations signal payload ---
TEST(SignalPayloadSerialization, ThreeDurationsRoundTrip)
{
    ThreeDurationsPayload original{ std::chrono::duration<double>(3536), std::chrono::duration<double>(3536), std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ThreeDurationsPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleBinary signal payload ---
TEST(SignalPayloadSerialization, SingleBinaryRoundTrip)
{
    SingleBinaryPayload original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleBinaryPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleOptionalBinary signal payload ---
TEST(SignalPayloadSerialization, SingleOptionalBinaryRoundTrip)
{
    SingleOptionalBinaryPayload original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleOptionalBinaryPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- threeBinaries signal payload ---
TEST(SignalPayloadSerialization, ThreeBinariesRoundTrip)
{
    ThreeBinariesPayload original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ThreeBinariesPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleArrayOfIntegers signal payload ---
TEST(SignalPayloadSerialization, SingleArrayOfIntegersRoundTrip)
{
    SingleArrayOfIntegersPayload original{ std::vector<int>{ 42, 2022, 2022 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleArrayOfIntegersPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- singleOptionalArrayOfStrings signal payload ---
TEST(SignalPayloadSerialization, SingleOptionalArrayOfStringsRoundTrip)
{
    SingleOptionalArrayOfStringsPayload original{ std::vector<std::string>{ "apples", "foo", "foo" } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = SingleOptionalArrayOfStringsPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- arrayOfEveryType signal payload ---
TEST(SignalPayloadSerialization, ArrayOfEveryTypeRoundTrip)
{
    ArrayOfEveryTypePayload original{ std::vector<int>{ 42, 2022, 2022 }, std::vector<double>{ 3.14, 1.0, 1.0 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ArrayOfEveryTypePayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- callWithNothing method request arguments ---
TEST(MethodRequestSerialization, CallWithNothingRoundTrip)
{
    CallWithNothingRequestArguments original{};
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallWithNothingRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callWithNothing method return values ---
TEST(MethodReturnSerialization, CallWithNothingRoundTrip)
{
    CallWithNothingReturnValues original{};
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallWithNothingReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneInteger method request arguments ---
TEST(MethodRequestSerialization, CallOneIntegerRoundTrip)
{
    CallOneIntegerRequestArguments original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneIntegerRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneInteger method return values ---
TEST(MethodReturnSerialization, CallOneIntegerRoundTrip)
{
    CallOneIntegerReturnValues original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneIntegerReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalInteger method request arguments ---
TEST(MethodRequestSerialization, CallOptionalIntegerRoundTrip)
{
    CallOptionalIntegerRequestArguments original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalIntegerRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalInteger method return values ---
TEST(MethodReturnSerialization, CallOptionalIntegerRoundTrip)
{
    CallOptionalIntegerReturnValues original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalIntegerReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeIntegers method request arguments ---
TEST(MethodRequestSerialization, CallThreeIntegersRoundTrip)
{
    CallThreeIntegersRequestArguments original{ 42, 42, 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeIntegersRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeIntegers method return values ---
TEST(MethodReturnSerialization, CallThreeIntegersRoundTrip)
{
    CallThreeIntegersReturnValues original{ 42, 42, 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeIntegersReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneString method request arguments ---
TEST(MethodRequestSerialization, CallOneStringRoundTrip)
{
    CallOneStringRequestArguments original{ "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneStringRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneString method return values ---
TEST(MethodReturnSerialization, CallOneStringRoundTrip)
{
    CallOneStringReturnValues original{ "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneStringReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalString method request arguments ---
TEST(MethodRequestSerialization, CallOptionalStringRoundTrip)
{
    CallOptionalStringRequestArguments original{ std::make_optional(std::string("apples")) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalStringRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalString method return values ---
TEST(MethodReturnSerialization, CallOptionalStringRoundTrip)
{
    CallOptionalStringReturnValues original{ std::make_optional(std::string("apples")) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalStringReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeStrings method request arguments ---
TEST(MethodRequestSerialization, CallThreeStringsRoundTrip)
{
    CallThreeStringsRequestArguments original{ "apples", std::make_optional(std::string("apples")), "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeStringsRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeStrings method return values ---
TEST(MethodReturnSerialization, CallThreeStringsRoundTrip)
{
    CallThreeStringsReturnValues original{ "apples", std::make_optional(std::string("apples")), "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeStringsReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneEnum method request arguments ---
TEST(MethodRequestSerialization, CallOneEnumRoundTrip)
{
    CallOneEnumRequestArguments original{ Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneEnumRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneEnum method return values ---
TEST(MethodReturnSerialization, CallOneEnumRoundTrip)
{
    CallOneEnumReturnValues original{ Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneEnumReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalEnum method request arguments ---
TEST(MethodRequestSerialization, CallOptionalEnumRoundTrip)
{
    CallOptionalEnumRequestArguments original{ Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalEnumRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalEnum method return values ---
TEST(MethodReturnSerialization, CallOptionalEnumRoundTrip)
{
    CallOptionalEnumReturnValues original{ Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalEnumReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeEnums method request arguments ---
TEST(MethodRequestSerialization, CallThreeEnumsRoundTrip)
{
    CallThreeEnumsRequestArguments original{ Numbers::ONE, Numbers::ONE, Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeEnumsRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeEnums method return values ---
TEST(MethodReturnSerialization, CallThreeEnumsRoundTrip)
{
    CallThreeEnumsReturnValues original{ Numbers::ONE, Numbers::ONE, Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeEnumsReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneStruct method request arguments ---
TEST(MethodRequestSerialization, CallOneStructRoundTrip)
{
    CallOneStructRequestArguments original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneStructRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneStruct method return values ---
TEST(MethodReturnSerialization, CallOneStructRoundTrip)
{
    CallOneStructReturnValues original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneStructReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalStruct method request arguments ---
TEST(MethodRequestSerialization, CallOptionalStructRoundTrip)
{
    CallOptionalStructRequestArguments original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalStructRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalStruct method return values ---
TEST(MethodReturnSerialization, CallOptionalStructRoundTrip)
{
    CallOptionalStructReturnValues original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalStructReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeStructs method request arguments ---
TEST(MethodRequestSerialization, CallThreeStructsRoundTrip)
{
    CallThreeStructsRequestArguments original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeStructsRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeStructs method return values ---
TEST(MethodReturnSerialization, CallThreeStructsRoundTrip)
{
    CallThreeStructsReturnValues original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeStructsReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneDateTime method request arguments ---
TEST(MethodRequestSerialization, CallOneDateTimeRoundTrip)
{
    CallOneDateTimeRequestArguments original{ std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneDateTimeRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneDateTime method return values ---
TEST(MethodReturnSerialization, CallOneDateTimeRoundTrip)
{
    CallOneDateTimeReturnValues original{ std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneDateTimeReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalDateTime method request arguments ---
TEST(MethodRequestSerialization, CallOptionalDateTimeRoundTrip)
{
    CallOptionalDateTimeRequestArguments original{ std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalDateTimeRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalDateTime method return values ---
TEST(MethodReturnSerialization, CallOptionalDateTimeRoundTrip)
{
    CallOptionalDateTimeReturnValues original{ std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalDateTimeReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeDateTimes method request arguments ---
TEST(MethodRequestSerialization, CallThreeDateTimesRoundTrip)
{
    CallThreeDateTimesRequestArguments original{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeDateTimesRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeDateTimes method return values ---
TEST(MethodReturnSerialization, CallThreeDateTimesRoundTrip)
{
    CallThreeDateTimesReturnValues original{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeDateTimesReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneDuration method request arguments ---
TEST(MethodRequestSerialization, CallOneDurationRoundTrip)
{
    CallOneDurationRequestArguments original{ std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneDurationRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneDuration method return values ---
TEST(MethodReturnSerialization, CallOneDurationRoundTrip)
{
    CallOneDurationReturnValues original{ std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneDurationReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalDuration method request arguments ---
TEST(MethodRequestSerialization, CallOptionalDurationRoundTrip)
{
    CallOptionalDurationRequestArguments original{ std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalDurationRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalDuration method return values ---
TEST(MethodReturnSerialization, CallOptionalDurationRoundTrip)
{
    CallOptionalDurationReturnValues original{ std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalDurationReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeDurations method request arguments ---
TEST(MethodRequestSerialization, CallThreeDurationsRoundTrip)
{
    CallThreeDurationsRequestArguments original{ std::chrono::duration<double>(3536), std::chrono::duration<double>(3536), std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeDurationsRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeDurations method return values ---
TEST(MethodReturnSerialization, CallThreeDurationsRoundTrip)
{
    CallThreeDurationsReturnValues original{ std::chrono::duration<double>(3536), std::chrono::duration<double>(3536), std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeDurationsReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneBinary method request arguments ---
TEST(MethodRequestSerialization, CallOneBinaryRoundTrip)
{
    CallOneBinaryRequestArguments original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneBinaryRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneBinary method return values ---
TEST(MethodReturnSerialization, CallOneBinaryRoundTrip)
{
    CallOneBinaryReturnValues original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneBinaryReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalBinary method request arguments ---
TEST(MethodRequestSerialization, CallOptionalBinaryRoundTrip)
{
    CallOptionalBinaryRequestArguments original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalBinaryRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalBinary method return values ---
TEST(MethodReturnSerialization, CallOptionalBinaryRoundTrip)
{
    CallOptionalBinaryReturnValues original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalBinaryReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeBinaries method request arguments ---
TEST(MethodRequestSerialization, CallThreeBinariesRoundTrip)
{
    CallThreeBinariesRequestArguments original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeBinariesRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callThreeBinaries method return values ---
TEST(MethodReturnSerialization, CallThreeBinariesRoundTrip)
{
    CallThreeBinariesReturnValues original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallThreeBinariesReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneListOfIntegers method request arguments ---
TEST(MethodRequestSerialization, CallOneListOfIntegersRoundTrip)
{
    CallOneListOfIntegersRequestArguments original{ std::vector<int>{ 42, 2022, 2022 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneListOfIntegersRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOneListOfIntegers method return values ---
TEST(MethodReturnSerialization, CallOneListOfIntegersRoundTrip)
{
    CallOneListOfIntegersReturnValues original{ std::vector<int>{ 42, 2022, 2022 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOneListOfIntegersReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalListOfFloats method request arguments ---
TEST(MethodRequestSerialization, CallOptionalListOfFloatsRoundTrip)
{
    CallOptionalListOfFloatsRequestArguments original{ std::vector<double>{ 3.14, 1.0, 1.0 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalListOfFloatsRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callOptionalListOfFloats method return values ---
TEST(MethodReturnSerialization, CallOptionalListOfFloatsRoundTrip)
{
    CallOptionalListOfFloatsReturnValues original{ std::vector<double>{ 3.14, 1.0, 1.0 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallOptionalListOfFloatsReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callTwoLists method request arguments ---
TEST(MethodRequestSerialization, CallTwoListsRoundTrip)
{
    CallTwoListsRequestArguments original{ std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::string>{ "apples", "foo", "foo" } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized request is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallTwoListsRequestArguments::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- callTwoLists method return values ---
TEST(MethodReturnSerialization, CallTwoListsRoundTrip)
{
    CallTwoListsReturnValues original{ std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::string>{ "apples", "foo", "foo" } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized return values are not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = CallTwoListsReturnValues::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_integer property ---
TEST(PropertySerialization, ReadWriteIntegerRoundTrip)
{
    ReadWriteIntegerProperty original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteIntegerProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_only_integer property ---
TEST(PropertySerialization, ReadOnlyIntegerRoundTrip)
{
    ReadOnlyIntegerProperty original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadOnlyIntegerProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_optional_integer property ---
TEST(PropertySerialization, ReadWriteOptionalIntegerRoundTrip)
{
    ReadWriteOptionalIntegerProperty original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteOptionalIntegerProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_two_integers property ---
TEST(PropertySerialization, ReadWriteTwoIntegersRoundTrip)
{
    ReadWriteTwoIntegersProperty original{ 42, 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteTwoIntegersProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_only_string property ---
TEST(PropertySerialization, ReadOnlyStringRoundTrip)
{
    ReadOnlyStringProperty original{ "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadOnlyStringProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_string property ---
TEST(PropertySerialization, ReadWriteStringRoundTrip)
{
    ReadWriteStringProperty original{ "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteStringProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_optional_string property ---
TEST(PropertySerialization, ReadWriteOptionalStringRoundTrip)
{
    ReadWriteOptionalStringProperty original{ std::make_optional(std::string("apples")) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteOptionalStringProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_two_strings property ---
TEST(PropertySerialization, ReadWriteTwoStringsRoundTrip)
{
    ReadWriteTwoStringsProperty original{ "apples", std::make_optional(std::string("apples")) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteTwoStringsProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_struct property ---
TEST(PropertySerialization, ReadWriteStructRoundTrip)
{
    ReadWriteStructProperty original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteStructProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_optional_struct property ---
TEST(PropertySerialization, ReadWriteOptionalStructRoundTrip)
{
    ReadWriteOptionalStructProperty original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteOptionalStructProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_two_structs property ---
TEST(PropertySerialization, ReadWriteTwoStructsRoundTrip)
{
    ReadWriteTwoStructsProperty original{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteTwoStructsProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_only_enum property ---
TEST(PropertySerialization, ReadOnlyEnumRoundTrip)
{
    ReadOnlyEnumProperty original{ Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadOnlyEnumProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_enum property ---
TEST(PropertySerialization, ReadWriteEnumRoundTrip)
{
    ReadWriteEnumProperty original{ Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteEnumProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_optional_enum property ---
TEST(PropertySerialization, ReadWriteOptionalEnumRoundTrip)
{
    ReadWriteOptionalEnumProperty original{ Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteOptionalEnumProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_two_enums property ---
TEST(PropertySerialization, ReadWriteTwoEnumsRoundTrip)
{
    ReadWriteTwoEnumsProperty original{ Numbers::ONE, Numbers::ONE };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteTwoEnumsProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_datetime property ---
TEST(PropertySerialization, ReadWriteDatetimeRoundTrip)
{
    ReadWriteDatetimeProperty original{ std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteDatetimeProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_optional_datetime property ---
TEST(PropertySerialization, ReadWriteOptionalDatetimeRoundTrip)
{
    ReadWriteOptionalDatetimeProperty original{ std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteOptionalDatetimeProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_two_datetimes property ---
TEST(PropertySerialization, ReadWriteTwoDatetimesRoundTrip)
{
    ReadWriteTwoDatetimesProperty original{ std::chrono::system_clock::now(), std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteTwoDatetimesProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_duration property ---
TEST(PropertySerialization, ReadWriteDurationRoundTrip)
{
    ReadWriteDurationProperty original{ std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteDurationProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_optional_duration property ---
TEST(PropertySerialization, ReadWriteOptionalDurationRoundTrip)
{
    ReadWriteOptionalDurationProperty original{ std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteOptionalDurationProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_two_durations property ---
TEST(PropertySerialization, ReadWriteTwoDurationsRoundTrip)
{
    ReadWriteTwoDurationsProperty original{ std::chrono::duration<double>(3536), std::chrono::duration<double>(3536) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteTwoDurationsProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_binary property ---
TEST(PropertySerialization, ReadWriteBinaryRoundTrip)
{
    ReadWriteBinaryProperty original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteBinaryProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_optional_binary property ---
TEST(PropertySerialization, ReadWriteOptionalBinaryRoundTrip)
{
    ReadWriteOptionalBinaryProperty original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteOptionalBinaryProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_two_binaries property ---
TEST(PropertySerialization, ReadWriteTwoBinariesRoundTrip)
{
    ReadWriteTwoBinariesProperty original{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteTwoBinariesProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_list_of_strings property ---
TEST(PropertySerialization, ReadWriteListOfStringsRoundTrip)
{
    ReadWriteListOfStringsProperty original{ std::vector<std::string>{ "apples", "foo", "foo" } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteListOfStringsProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- read_write_lists property ---
TEST(PropertySerialization, ReadWriteListsRoundTrip)
{
    ReadWriteListsProperty original{ std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = ReadWriteListsProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}
