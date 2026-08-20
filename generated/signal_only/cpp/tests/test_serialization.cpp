/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the SignalOnly interface.  These exercise the (de)serialization
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

using namespace stinger::gen::signal_only;

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

// --- anotherSignal signal payload ---
TEST(SignalPayloadSerialization, AnotherSignalRoundTrip)
{
    AnotherSignalPayload original{ 3.14, true, "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = AnotherSignalPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- bark signal payload ---
TEST(SignalPayloadSerialization, BarkRoundTrip)
{
    BarkPayload original{ "apples" };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = BarkPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- maybe_number signal payload ---
TEST(SignalPayloadSerialization, MaybeNumberRoundTrip)
{
    MaybeNumberPayload original{ 42 };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = MaybeNumberPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- maybe_name signal payload ---
TEST(SignalPayloadSerialization, MaybeNameRoundTrip)
{
    MaybeNamePayload original{ std::make_optional(std::string("apples")) };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = MaybeNamePayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}

// --- now signal payload ---
TEST(SignalPayloadSerialization, NowRoundTrip)
{
    NowPayload original{ std::chrono::system_clock::now() };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized payload is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = NowPayload::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    // Exercise the schema-validation code path.
    (void)roundTripped.ValidateSchema();
}
