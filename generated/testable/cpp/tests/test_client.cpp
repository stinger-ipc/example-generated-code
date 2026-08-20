/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the testable client.  These use the MockConnection from
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

using namespace stinger::gen::testable;

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

class TestableClientTest: public ::testing::Test {
protected:
    std::shared_ptr<stinger::utils::MockConnection> _conn;
    std::unique_ptr<TestableClient> _client;

    void SetUp() override
    {
        _conn = std::make_shared<stinger::utils::MockConnection>(kClientId);

        InstanceInfo info;
        info.serviceId = kServiceId;
        info.prefix = "test-prefix";

        _client = std::make_unique<TestableClient>(_conn, info);
    }

    // Mirrors the topic parameters the client uses internally so tests can
    // reconstruct the exact concrete topic strings.
    std::map<std::string, std::string> topicParams() const
    {
        std::map<std::string, std::string> params;
        params["client_id"] = _conn->GetClientId();
        params["service_id"] = kServiceId;
        params["interface_name"] = TestableClient::NAME;
        params["prefix"] = "test-prefix";
        return params;
    }
};

} // anonymous namespace

// The client should subscribe to one topic per signal, method response,
// and property value, plus the aggregate property-update response topic.
TEST_F(TestableClientTest, SubscribesToExpectedTopics)
{
    const std::size_t expected = 79;
    EXPECT_EQ(_conn->GetSubscriptions().size(), expected);
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/empty", topicParams()))) << "Not subscribed to empty signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleInt", topicParams()))) << "Not subscribed to singleInt signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/jsonSchemaValidatedInt", topicParams()))) << "Not subscribed to jsonSchemaValidatedInt signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalInt", topicParams()))) << "Not subscribed to singleOptionalInt signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/threeIntegers", topicParams()))) << "Not subscribed to threeIntegers signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleString", topicParams()))) << "Not subscribed to singleString signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/jsonSchemaValidatedString", topicParams()))) << "Not subscribed to jsonSchemaValidatedString signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalString", topicParams()))) << "Not subscribed to singleOptionalString signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/threeStrings", topicParams()))) << "Not subscribed to threeStrings signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleEnum", topicParams()))) << "Not subscribed to singleEnum signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalEnum", topicParams()))) << "Not subscribed to singleOptionalEnum signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/threeEnums", topicParams()))) << "Not subscribed to threeEnums signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleStruct", topicParams()))) << "Not subscribed to singleStruct signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalStruct", topicParams()))) << "Not subscribed to singleOptionalStruct signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/threeStructs", topicParams()))) << "Not subscribed to threeStructs signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleDateTime", topicParams()))) << "Not subscribed to singleDateTime signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalDatetime", topicParams()))) << "Not subscribed to singleOptionalDatetime signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/threeDateTimes", topicParams()))) << "Not subscribed to threeDateTimes signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleDuration", topicParams()))) << "Not subscribed to singleDuration signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalDuration", topicParams()))) << "Not subscribed to singleOptionalDuration signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/threeDurations", topicParams()))) << "Not subscribed to threeDurations signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleBinary", topicParams()))) << "Not subscribed to singleBinary signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalBinary", topicParams()))) << "Not subscribed to singleOptionalBinary signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/threeBinaries", topicParams()))) << "Not subscribed to threeBinaries signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleArrayOfIntegers", topicParams()))) << "Not subscribed to singleArrayOfIntegers signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalArrayOfStrings", topicParams()))) << "Not subscribed to singleOptionalArrayOfStrings signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/signal/arrayOfEveryType", topicParams()))) << "Not subscribed to arrayOfEveryType signal topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callWithNothing/response", topicParams()))) << "Not subscribed to callWithNothing response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOneInteger/response", topicParams()))) << "Not subscribed to callOneInteger response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOptionalInteger/response", topicParams()))) << "Not subscribed to callOptionalInteger response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callThreeIntegers/response", topicParams()))) << "Not subscribed to callThreeIntegers response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOneString/response", topicParams()))) << "Not subscribed to callOneString response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOptionalString/response", topicParams()))) << "Not subscribed to callOptionalString response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callThreeStrings/response", topicParams()))) << "Not subscribed to callThreeStrings response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOneEnum/response", topicParams()))) << "Not subscribed to callOneEnum response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOptionalEnum/response", topicParams()))) << "Not subscribed to callOptionalEnum response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callThreeEnums/response", topicParams()))) << "Not subscribed to callThreeEnums response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOneStruct/response", topicParams()))) << "Not subscribed to callOneStruct response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOptionalStruct/response", topicParams()))) << "Not subscribed to callOptionalStruct response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callThreeStructs/response", topicParams()))) << "Not subscribed to callThreeStructs response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOneDateTime/response", topicParams()))) << "Not subscribed to callOneDateTime response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOptionalDateTime/response", topicParams()))) << "Not subscribed to callOptionalDateTime response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callThreeDateTimes/response", topicParams()))) << "Not subscribed to callThreeDateTimes response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOneDuration/response", topicParams()))) << "Not subscribed to callOneDuration response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOptionalDuration/response", topicParams()))) << "Not subscribed to callOptionalDuration response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callThreeDurations/response", topicParams()))) << "Not subscribed to callThreeDurations response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOneBinary/response", topicParams()))) << "Not subscribed to callOneBinary response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOptionalBinary/response", topicParams()))) << "Not subscribed to callOptionalBinary response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callThreeBinaries/response", topicParams()))) << "Not subscribed to callThreeBinaries response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOneListOfIntegers/response", topicParams()))) << "Not subscribed to callOneListOfIntegers response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callOptionalListOfFloats/response", topicParams()))) << "Not subscribed to callOptionalListOfFloats response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("client/{client_id}/testable/method/callTwoLists/response", topicParams()))) << "Not subscribed to callTwoLists response topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_integer/value", topicParams()))) << "Not subscribed to read_write_integer property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_integer/value", topicParams()))) << "Not subscribed to read_only_integer property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_integer/value", topicParams()))) << "Not subscribed to read_write_optional_integer property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_integers/value", topicParams()))) << "Not subscribed to read_write_two_integers property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_string/value", topicParams()))) << "Not subscribed to read_only_string property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_string/value", topicParams()))) << "Not subscribed to read_write_string property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_string/value", topicParams()))) << "Not subscribed to read_write_optional_string property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_strings/value", topicParams()))) << "Not subscribed to read_write_two_strings property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_struct/value", topicParams()))) << "Not subscribed to read_write_struct property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_struct/value", topicParams()))) << "Not subscribed to read_write_optional_struct property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_structs/value", topicParams()))) << "Not subscribed to read_write_two_structs property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_enum/value", topicParams()))) << "Not subscribed to read_only_enum property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_enum/value", topicParams()))) << "Not subscribed to read_write_enum property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_enum/value", topicParams()))) << "Not subscribed to read_write_optional_enum property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_enums/value", topicParams()))) << "Not subscribed to read_write_two_enums property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_datetime/value", topicParams()))) << "Not subscribed to read_write_datetime property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_datetime/value", topicParams()))) << "Not subscribed to read_write_optional_datetime property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_datetimes/value", topicParams()))) << "Not subscribed to read_write_two_datetimes property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_duration/value", topicParams()))) << "Not subscribed to read_write_duration property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_duration/value", topicParams()))) << "Not subscribed to read_write_optional_duration property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_durations/value", topicParams()))) << "Not subscribed to read_write_two_durations property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_binary/value", topicParams()))) << "Not subscribed to read_write_binary property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_binary/value", topicParams()))) << "Not subscribed to read_write_optional_binary property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_binaries/value", topicParams()))) << "Not subscribed to read_write_two_binaries property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_list_of_strings/value", topicParams()))) << "Not subscribed to read_write_list_of_strings property value topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_lists/value", topicParams()))) << "Not subscribed to read_write_lists property value topic";
}

// A simulated incoming `empty` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, EmptySignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerEmptyCallback([&callbackInvoked]()
                                   {
                                       callbackInvoked = true;
                                   });

    EmptyPayload payload{};
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/empty", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleInt` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleIntSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleIntCallback([&callbackInvoked](int)
                                       {
                                           callbackInvoked = true;
                                       });

    SingleIntPayload payload{ 42 };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleInt", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `jsonSchemaValidatedInt` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, JsonSchemaValidatedIntSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerJsonSchemaValidatedIntCallback([&callbackInvoked](int)
                                                    {
                                                        callbackInvoked = true;
                                                    });

    JsonSchemaValidatedIntPayload payload{ 42 };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/jsonSchemaValidatedInt", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    // This signal declares a JSON-schema constraint; whether the example payload
    // passes validation (and therefore reaches the callback) depends on the value,
    // so we only assert that processing the message does not crash.
}

// A simulated incoming `singleOptionalInt` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleOptionalIntSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleOptionalIntCallback([&callbackInvoked](std::optional<int>)
                                               {
                                                   callbackInvoked = true;
                                               });

    SingleOptionalIntPayload payload{ 42 };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalInt", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `threeIntegers` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, ThreeIntegersSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerThreeIntegersCallback([&callbackInvoked](int, int, std::optional<int>)
                                           {
                                               callbackInvoked = true;
                                           });

    ThreeIntegersPayload payload{ 42, 42, 42 };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeIntegers", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleString` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleStringSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleStringCallback([&callbackInvoked](std::string)
                                          {
                                              callbackInvoked = true;
                                          });

    SingleStringPayload payload{ "apples" };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleString", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `jsonSchemaValidatedString` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, JsonSchemaValidatedStringSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerJsonSchemaValidatedStringCallback([&callbackInvoked](std::string)
                                                       {
                                                           callbackInvoked = true;
                                                       });

    JsonSchemaValidatedStringPayload payload{ "apples" };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/jsonSchemaValidatedString", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    // This signal declares a JSON-schema constraint; whether the example payload
    // passes validation (and therefore reaches the callback) depends on the value,
    // so we only assert that processing the message does not crash.
}

// A simulated incoming `singleOptionalString` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleOptionalStringSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleOptionalStringCallback([&callbackInvoked](std::optional<std::string>)
                                                  {
                                                      callbackInvoked = true;
                                                  });

    SingleOptionalStringPayload payload{ std::make_optional(std::string("apples")) };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalString", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `threeStrings` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, ThreeStringsSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerThreeStringsCallback([&callbackInvoked](std::string, std::string, std::optional<std::string>)
                                          {
                                              callbackInvoked = true;
                                          });

    ThreeStringsPayload payload{ "apples", "apples", std::make_optional(std::string("apples")) };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeStrings", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleEnum` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleEnumSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleEnumCallback([&callbackInvoked](Numbers)
                                        {
                                            callbackInvoked = true;
                                        });

    SingleEnumPayload payload{ Numbers::ONE };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleEnum", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleOptionalEnum` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleOptionalEnumSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleOptionalEnumCallback([&callbackInvoked](std::optional<Numbers>)
                                                {
                                                    callbackInvoked = true;
                                                });

    SingleOptionalEnumPayload payload{ Numbers::ONE };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalEnum", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `threeEnums` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, ThreeEnumsSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerThreeEnumsCallback([&callbackInvoked](Numbers, Numbers, std::optional<Numbers>)
                                        {
                                            callbackInvoked = true;
                                        });

    ThreeEnumsPayload payload{ Numbers::ONE, Numbers::ONE, Numbers::ONE };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeEnums", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleStruct` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleStructSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleStructCallback([&callbackInvoked](AllTypes)
                                          {
                                              callbackInvoked = true;
                                          });

    SingleStructPayload payload{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleStruct", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleOptionalStruct` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleOptionalStructSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleOptionalStructCallback([&callbackInvoked](std::optional<AllTypes>)
                                                  {
                                                      callbackInvoked = true;
                                                  });

    SingleOptionalStructPayload payload{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalStruct", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `threeStructs` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, ThreeStructsSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerThreeStructsCallback([&callbackInvoked](AllTypes, AllTypes, std::optional<AllTypes>)
                                          {
                                              callbackInvoked = true;
                                          });

    ThreeStructsPayload payload{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeStructs", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleDateTime` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleDateTimeSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleDateTimeCallback([&callbackInvoked](std::chrono::time_point<std::chrono::system_clock>)
                                            {
                                                callbackInvoked = true;
                                            });

    SingleDateTimePayload payload{ std::chrono::system_clock::now() };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleDateTime", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleOptionalDatetime` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleOptionalDatetimeSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleOptionalDatetimeCallback([&callbackInvoked](std::optional<std::chrono::time_point<std::chrono::system_clock>>)
                                                    {
                                                        callbackInvoked = true;
                                                    });

    SingleOptionalDatetimePayload payload{ std::chrono::system_clock::now() };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalDatetime", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `threeDateTimes` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, ThreeDateTimesSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerThreeDateTimesCallback([&callbackInvoked](std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>, std::optional<std::chrono::time_point<std::chrono::system_clock>>)
                                            {
                                                callbackInvoked = true;
                                            });

    ThreeDateTimesPayload payload{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeDateTimes", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleDuration` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleDurationSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleDurationCallback([&callbackInvoked](std::chrono::duration<double>)
                                            {
                                                callbackInvoked = true;
                                            });

    SingleDurationPayload payload{ std::chrono::duration<double>(3536) };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleDuration", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleOptionalDuration` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleOptionalDurationSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleOptionalDurationCallback([&callbackInvoked](std::optional<std::chrono::duration<double>>)
                                                    {
                                                        callbackInvoked = true;
                                                    });

    SingleOptionalDurationPayload payload{ std::chrono::duration<double>(3536) };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalDuration", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `threeDurations` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, ThreeDurationsSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerThreeDurationsCallback([&callbackInvoked](std::chrono::duration<double>, std::chrono::duration<double>, std::optional<std::chrono::duration<double>>)
                                            {
                                                callbackInvoked = true;
                                            });

    ThreeDurationsPayload payload{ std::chrono::duration<double>(3536), std::chrono::duration<double>(3536), std::chrono::duration<double>(3536) };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeDurations", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleBinary` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleBinarySignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleBinaryCallback([&callbackInvoked](std::vector<uint8_t>)
                                          {
                                              callbackInvoked = true;
                                          });

    SingleBinaryPayload payload{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleBinary", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleOptionalBinary` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleOptionalBinarySignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleOptionalBinaryCallback([&callbackInvoked](std::optional<std::vector<uint8_t>>)
                                                  {
                                                      callbackInvoked = true;
                                                  });

    SingleOptionalBinaryPayload payload{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalBinary", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `threeBinaries` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, ThreeBinariesSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerThreeBinariesCallback([&callbackInvoked](std::vector<uint8_t>, std::vector<uint8_t>, std::optional<std::vector<uint8_t>>)
                                           {
                                               callbackInvoked = true;
                                           });

    ThreeBinariesPayload payload{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeBinaries", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleArrayOfIntegers` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleArrayOfIntegersSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleArrayOfIntegersCallback([&callbackInvoked](std::vector<int>)
                                                   {
                                                       callbackInvoked = true;
                                                   });

    SingleArrayOfIntegersPayload payload{ std::vector<int>{ 42, 2022, 2022 } };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleArrayOfIntegers", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `singleOptionalArrayOfStrings` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, SingleOptionalArrayOfStringsSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerSingleOptionalArrayOfStringsCallback([&callbackInvoked](std::optional<std::vector<std::string>>)
                                                          {
                                                              callbackInvoked = true;
                                                          });

    SingleOptionalArrayOfStringsPayload payload{ std::vector<std::string>{ "apples", "foo", "foo" } };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalArrayOfStrings", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// A simulated incoming `arrayOfEveryType` signal should be dispatched to the registered callback.
TEST_F(TestableClientTest, ArrayOfEveryTypeSignalInvokesCallback)
{
    bool callbackInvoked = false;
    _client->registerArrayOfEveryTypeCallback([&callbackInvoked](std::vector<int>, std::vector<double>, std::vector<std::string>, std::vector<Numbers>, std::vector<Entry>, std::vector<std::chrono::time_point<std::chrono::system_clock>>, std::vector<std::chrono::duration<double>>, std::vector<std::vector<uint8_t>>)
                                              {
                                                  callbackInvoked = true;
                                              });

    ArrayOfEveryTypePayload payload{ std::vector<int>{ 42, 2022, 2022 }, std::vector<double>{ 3.14, 1.0, 1.0 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } } };
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/arrayOfEveryType", topicParams());
    // The MockConnection routes the message by topic and stamps the matching
    // subscription id, which is how the client dispatches it to the callback.
    auto msg = stinger::mqtt::Message::Signal(topic, serializeToString(payload));

    _conn->SimulateIncomingMessage(msg);
    EXPECT_TRUE(callbackInvoked);
}

// Calling `callWithNothing` should publish a request to the broker.
TEST_F(TestableClientTest, CallWithNothingPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callWithNothing();

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callWithNothing/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOneInteger` should publish a request to the broker.
TEST_F(TestableClientTest, CallOneIntegerPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOneInteger(42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOneInteger/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOptionalInteger` should publish a request to the broker.
TEST_F(TestableClientTest, CallOptionalIntegerPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOptionalInteger(42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalInteger/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callThreeIntegers` should publish a request to the broker.
TEST_F(TestableClientTest, CallThreeIntegersPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callThreeIntegers(42, 42, 42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeIntegers/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOneString` should publish a request to the broker.
TEST_F(TestableClientTest, CallOneStringPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOneString("apples");

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOneString/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOptionalString` should publish a request to the broker.
TEST_F(TestableClientTest, CallOptionalStringPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOptionalString(std::make_optional(std::string("apples")));

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalString/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callThreeStrings` should publish a request to the broker.
TEST_F(TestableClientTest, CallThreeStringsPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callThreeStrings("apples", std::make_optional(std::string("apples")), "apples");

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeStrings/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOneEnum` should publish a request to the broker.
TEST_F(TestableClientTest, CallOneEnumPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOneEnum(Numbers::ONE);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOneEnum/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOptionalEnum` should publish a request to the broker.
TEST_F(TestableClientTest, CallOptionalEnumPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOptionalEnum(Numbers::ONE);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalEnum/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callThreeEnums` should publish a request to the broker.
TEST_F(TestableClientTest, CallThreeEnumsPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callThreeEnums(Numbers::ONE, Numbers::ONE, Numbers::ONE);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeEnums/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOneStruct` should publish a request to the broker.
TEST_F(TestableClientTest, CallOneStructPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOneStruct(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOneStruct/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOptionalStruct` should publish a request to the broker.
TEST_F(TestableClientTest, CallOptionalStructPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOptionalStruct(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalStruct/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callThreeStructs` should publish a request to the broker.
TEST_F(TestableClientTest, CallThreeStructsPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callThreeStructs(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeStructs/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOneDateTime` should publish a request to the broker.
TEST_F(TestableClientTest, CallOneDateTimePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOneDateTime(std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOneDateTime/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOptionalDateTime` should publish a request to the broker.
TEST_F(TestableClientTest, CallOptionalDateTimePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOptionalDateTime(std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalDateTime/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callThreeDateTimes` should publish a request to the broker.
TEST_F(TestableClientTest, CallThreeDateTimesPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callThreeDateTimes(std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeDateTimes/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOneDuration` should publish a request to the broker.
TEST_F(TestableClientTest, CallOneDurationPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOneDuration(std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOneDuration/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOptionalDuration` should publish a request to the broker.
TEST_F(TestableClientTest, CallOptionalDurationPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOptionalDuration(std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalDuration/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callThreeDurations` should publish a request to the broker.
TEST_F(TestableClientTest, CallThreeDurationsPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callThreeDurations(std::chrono::duration<double>(3536), std::chrono::duration<double>(3536), std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeDurations/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOneBinary` should publish a request to the broker.
TEST_F(TestableClientTest, CallOneBinaryPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOneBinary(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOneBinary/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOptionalBinary` should publish a request to the broker.
TEST_F(TestableClientTest, CallOptionalBinaryPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOptionalBinary(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalBinary/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callThreeBinaries` should publish a request to the broker.
TEST_F(TestableClientTest, CallThreeBinariesPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callThreeBinaries(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeBinaries/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOneListOfIntegers` should publish a request to the broker.
TEST_F(TestableClientTest, CallOneListOfIntegersPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOneListOfIntegers(std::vector<int>{ 42, 2022, 2022 });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOneListOfIntegers/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callOptionalListOfFloats` should publish a request to the broker.
TEST_F(TestableClientTest, CallOptionalListOfFloatsPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callOptionalListOfFloats(std::vector<double>{ 3.14, 1.0, 1.0 });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalListOfFloats/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Calling `callTwoLists` should publish a request to the broker.
TEST_F(TestableClientTest, CallTwoListsPublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->callTwoLists(std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::string>{ "apples", "foo", "foo" });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/method/callTwoLists/request", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published request payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_integer` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteIntegerUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteIntegerProperty(42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_integer/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_optional_integer` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteOptionalIntegerUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteOptionalIntegerProperty(42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_integer/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_two_integers` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteTwoIntegersUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteTwoIntegersProperty(42, 42);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_integers/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_string` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteStringUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteStringProperty("apples");

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_string/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_optional_string` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteOptionalStringUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteOptionalStringProperty(std::make_optional(std::string("apples")));

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_string/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_two_strings` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteTwoStringsUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteTwoStringsProperty("apples", std::make_optional(std::string("apples")));

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_strings/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_struct` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteStructUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteStructProperty(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_struct/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_optional_struct` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteOptionalStructUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteOptionalStructProperty(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_struct/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_two_structs` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteTwoStructsUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteTwoStructsProperty(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_structs/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_enum` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteEnumUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteEnumProperty(Numbers::ONE);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_enum/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_optional_enum` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteOptionalEnumUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteOptionalEnumProperty(Numbers::ONE);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_enum/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_two_enums` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteTwoEnumsUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteTwoEnumsProperty(Numbers::ONE, Numbers::ONE);

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_enums/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_datetime` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteDatetimeUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteDatetimeProperty(std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_datetime/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_optional_datetime` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteOptionalDatetimeUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteOptionalDatetimeProperty(std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_datetime/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_two_datetimes` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteTwoDatetimesUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteTwoDatetimesProperty(std::chrono::system_clock::now(), std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_datetimes/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_duration` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteDurationUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteDurationProperty(std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_duration/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_optional_duration` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteOptionalDurationUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteOptionalDurationProperty(std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_duration/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_two_durations` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteTwoDurationsUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteTwoDurationsProperty(std::chrono::duration<double>(3536), std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_durations/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_binary` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteBinaryUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteBinaryProperty(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_binary/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_optional_binary` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteOptionalBinaryUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteOptionalBinaryProperty(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_binary/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_two_binaries` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteTwoBinariesUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteTwoBinariesProperty(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_binaries/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_list_of_strings` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteListOfStringsUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteListOfStringsProperty(std::vector<std::string>{ "apples", "foo", "foo" });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_list_of_strings/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Updating `read_write_lists` should publish an update request to the broker.
TEST_F(TestableClientTest, ReadWriteListsUpdatePublishesRequest)
{
    _conn->ClearPublishedMessages();

    auto pending = _client->updateReadWriteListsProperty(std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() });

    auto published = _conn->GetPublishedMessages();
    ASSERT_EQ(published.size(), 1u);
    EXPECT_EQ(published[0].topic, stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_lists/update", topicParams()));

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property update payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}
