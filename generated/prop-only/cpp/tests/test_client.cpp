/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the prop-only client.  These use the MockConnection from
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

using namespace stinger::gen::prop_only;

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

class PropOnlyClientTest: public ::testing::Test {
protected:
    std::shared_ptr<stinger::utils::MockConnection> _conn;
    std::unique_ptr<PropOnlyClient> _client;

    void SetUp() override
    {
        _conn = std::make_shared<stinger::utils::MockConnection>(kClientId);

        InstanceInfo info;
        info.serviceId = kServiceId;
        info.prefix = "test-prefix";

        _client = std::make_unique<PropOnlyClient>(_conn, info);
    }

    // Mirrors the topic parameters the client uses internally so tests can
    // reconstruct the exact concrete topic strings.
    std::map<std::string, std::string> topicParams() const
    {
        std::map<std::string, std::string> params;
        params["client_id"] = _conn->GetClientId();
        params["service_id"] = kServiceId;
        params["interface_name"] = PropOnlyClient::NAME;
        params["prefix"] = "test-prefix";
        return params;
    }
};

} // anonymous namespace

// The client should subscribe to one topic per signal, method response,
// and property value, plus the aggregate property-update response topic.
TEST_F(PropOnlyClientTest, SubscribesToExpectedTopics)
{
    const std::size_t expected = 3;
    EXPECT_EQ(_conn->GetSubscriptions().size(), expected);
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/prop-only/{service_id}/property/home_address/value", topicParams()))) << "Not subscribed to home_address property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/prop-only/{service_id}/property/favorite_country/value", topicParams()))) << "Not subscribed to favorite_country property value topic";
}

// Updating `home_address` should publish an update request to the broker.
TEST_F(PropOnlyClientTest, HomeAddressUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateHomeAddressProperty(Address{ "apples", "apples", "apples", "apples", Country::USA });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/prop-only/{service_id}/property/home_address/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `favorite_country` should publish an update request to the broker.
TEST_F(PropOnlyClientTest, FavoriteCountryUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateFavoriteCountryProperty(Country::USA);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/prop-only/{service_id}/property/favorite_country/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}
