/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the prop-only interface.  These exercise the (de)serialization
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

using namespace stinger::gen::prop_only;

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

// --- home_address property ---
TEST(PropertySerialization, HomeAddressRoundTrip)
{
    HomeAddressProperty original{ Address{ "apples", "apples", "apples", "apples", Country::USA } };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = HomeAddressProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}

// --- favorite_country property ---
TEST(PropertySerialization, FavoriteCountryRoundTrip)
{
    FavoriteCountryProperty original{ Country::USA };
    const std::string firstJson = serializeToString(original);

    rapidjson::Document doc;
    ASSERT_FALSE(doc.Parse(firstJson.c_str()).HasParseError()) << "Serialized property is not valid JSON: " << firstJson;
    ASSERT_TRUE(doc.IsObject());

    auto roundTripped = FavoriteCountryProperty::FromRapidJsonObject(doc);
    const std::string secondJson = serializeToString(roundTripped);

    EXPECT_EQ(firstJson, secondJson);
    (void)roundTripped.ValidateSchema();
}
