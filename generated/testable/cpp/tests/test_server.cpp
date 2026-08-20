/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

Unit tests for the testable server.  These use the MockConnection from
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

using namespace stinger::gen::testable;

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
class TestableServerTest: public ::testing::Test {
protected:
    static std::shared_ptr<stinger::utils::MockConnection> _conn;
    static std::unique_ptr<TestableServer> _server;

    static void SetUpTestSuite()
    {
        _conn = std::make_shared<stinger::utils::MockConnection>(kClientId);
        _server = std::make_unique<TestableServer>(_conn, kServiceId, "test-prefix");
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
        params["interface_name"] = TestableServer::NAME;
        params["client_id"] = _conn->GetClientId();
        params["prefix"] = "test-prefix";
        return params;
    }
};

std::shared_ptr<stinger::utils::MockConnection> TestableServerTest::_conn;
std::unique_ptr<TestableServer> TestableServerTest::_server;

} // anonymous namespace

// The server should subscribe to one topic per method request and per property update.
TEST_F(TestableServerTest, SubscribesToExpectedTopics)
{
    const std::size_t expected = 51;
    EXPECT_EQ(_conn->GetSubscriptions().size(), expected);
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callWithNothing/request", topicArgs()))) << "Not subscribed to callWithNothing request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOneInteger/request", topicArgs()))) << "Not subscribed to callOneInteger request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalInteger/request", topicArgs()))) << "Not subscribed to callOptionalInteger request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeIntegers/request", topicArgs()))) << "Not subscribed to callThreeIntegers request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOneString/request", topicArgs()))) << "Not subscribed to callOneString request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalString/request", topicArgs()))) << "Not subscribed to callOptionalString request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeStrings/request", topicArgs()))) << "Not subscribed to callThreeStrings request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOneEnum/request", topicArgs()))) << "Not subscribed to callOneEnum request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalEnum/request", topicArgs()))) << "Not subscribed to callOptionalEnum request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeEnums/request", topicArgs()))) << "Not subscribed to callThreeEnums request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOneStruct/request", topicArgs()))) << "Not subscribed to callOneStruct request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalStruct/request", topicArgs()))) << "Not subscribed to callOptionalStruct request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeStructs/request", topicArgs()))) << "Not subscribed to callThreeStructs request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOneDateTime/request", topicArgs()))) << "Not subscribed to callOneDateTime request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalDateTime/request", topicArgs()))) << "Not subscribed to callOptionalDateTime request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeDateTimes/request", topicArgs()))) << "Not subscribed to callThreeDateTimes request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOneDuration/request", topicArgs()))) << "Not subscribed to callOneDuration request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalDuration/request", topicArgs()))) << "Not subscribed to callOptionalDuration request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeDurations/request", topicArgs()))) << "Not subscribed to callThreeDurations request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOneBinary/request", topicArgs()))) << "Not subscribed to callOneBinary request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalBinary/request", topicArgs()))) << "Not subscribed to callOptionalBinary request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeBinaries/request", topicArgs()))) << "Not subscribed to callThreeBinaries request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOneListOfIntegers/request", topicArgs()))) << "Not subscribed to callOneListOfIntegers request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalListOfFloats/request", topicArgs()))) << "Not subscribed to callOptionalListOfFloats request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/method/callTwoLists/request", topicArgs()))) << "Not subscribed to callTwoLists request topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_integer/update", topicArgs()))) << "Not subscribed to read_write_integer update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_integer/update", topicArgs()))) << "Not subscribed to read_only_integer update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_integer/update", topicArgs()))) << "Not subscribed to read_write_optional_integer update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_integers/update", topicArgs()))) << "Not subscribed to read_write_two_integers update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_string/update", topicArgs()))) << "Not subscribed to read_only_string update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_string/update", topicArgs()))) << "Not subscribed to read_write_string update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_string/update", topicArgs()))) << "Not subscribed to read_write_optional_string update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_strings/update", topicArgs()))) << "Not subscribed to read_write_two_strings update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_struct/update", topicArgs()))) << "Not subscribed to read_write_struct update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_struct/update", topicArgs()))) << "Not subscribed to read_write_optional_struct update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_structs/update", topicArgs()))) << "Not subscribed to read_write_two_structs update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_enum/update", topicArgs()))) << "Not subscribed to read_only_enum update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_enum/update", topicArgs()))) << "Not subscribed to read_write_enum update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_enum/update", topicArgs()))) << "Not subscribed to read_write_optional_enum update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_enums/update", topicArgs()))) << "Not subscribed to read_write_two_enums update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_datetime/update", topicArgs()))) << "Not subscribed to read_write_datetime update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_datetime/update", topicArgs()))) << "Not subscribed to read_write_optional_datetime update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_datetimes/update", topicArgs()))) << "Not subscribed to read_write_two_datetimes update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_duration/update", topicArgs()))) << "Not subscribed to read_write_duration update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_duration/update", topicArgs()))) << "Not subscribed to read_write_optional_duration update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_durations/update", topicArgs()))) << "Not subscribed to read_write_two_durations update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_binary/update", topicArgs()))) << "Not subscribed to read_write_binary update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_binary/update", topicArgs()))) << "Not subscribed to read_write_optional_binary update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_binaries/update", topicArgs()))) << "Not subscribed to read_write_two_binaries update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_list_of_strings/update", topicArgs()))) << "Not subscribed to read_write_list_of_strings update topic";
    EXPECT_TRUE(_conn->IsSubscribed(stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_lists/update", topicArgs()))) << "Not subscribed to read_write_lists update topic";
}

// Emitting `empty` should publish the signal to the broker.
TEST_F(TestableServerTest, EmptyEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/empty", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitEmptySignal();

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleInt` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleIntEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleInt", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleIntSignal(42);

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleOptionalInt` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleOptionalIntEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalInt", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleOptionalIntSignal(42);

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `threeIntegers` should publish the signal to the broker.
TEST_F(TestableServerTest, ThreeIntegersEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeIntegers", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitThreeIntegersSignal(42, 42, 42);

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleString` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleStringEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleString", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleStringSignal("apples");

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleOptionalString` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleOptionalStringEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalString", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleOptionalStringSignal(std::make_optional(std::string("apples")));

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `threeStrings` should publish the signal to the broker.
TEST_F(TestableServerTest, ThreeStringsEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeStrings", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitThreeStringsSignal("apples", "apples", std::make_optional(std::string("apples")));

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleEnum` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleEnumEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleEnum", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleEnumSignal(Numbers::ONE);

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleOptionalEnum` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleOptionalEnumEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalEnum", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleOptionalEnumSignal(Numbers::ONE);

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `threeEnums` should publish the signal to the broker.
TEST_F(TestableServerTest, ThreeEnumsEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeEnums", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitThreeEnumsSignal(Numbers::ONE, Numbers::ONE, Numbers::ONE);

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleStruct` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleStructEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleStruct", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleStructSignal(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleOptionalStruct` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleOptionalStructEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalStruct", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleOptionalStructSignal(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `threeStructs` should publish the signal to the broker.
TEST_F(TestableServerTest, ThreeStructsEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeStructs", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitThreeStructsSignal(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleDateTime` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleDateTimeEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleDateTime", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleDateTimeSignal(std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleOptionalDatetime` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleOptionalDatetimeEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalDatetime", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleOptionalDatetimeSignal(std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `threeDateTimes` should publish the signal to the broker.
TEST_F(TestableServerTest, ThreeDateTimesEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeDateTimes", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitThreeDateTimesSignal(std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleDuration` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleDurationEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleDuration", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleDurationSignal(std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleOptionalDuration` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleOptionalDurationEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalDuration", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleOptionalDurationSignal(std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `threeDurations` should publish the signal to the broker.
TEST_F(TestableServerTest, ThreeDurationsEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeDurations", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitThreeDurationsSignal(std::chrono::duration<double>(3536), std::chrono::duration<double>(3536), std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleBinary` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleBinaryEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleBinary", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleBinarySignal(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleOptionalBinary` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleOptionalBinaryEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalBinary", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleOptionalBinarySignal(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `threeBinaries` should publish the signal to the broker.
TEST_F(TestableServerTest, ThreeBinariesEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/threeBinaries", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitThreeBinariesSignal(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleArrayOfIntegers` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleArrayOfIntegersEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleArrayOfIntegers", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleArrayOfIntegersSignal(std::vector<int>{ 42, 2022, 2022 });

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `singleOptionalArrayOfStrings` should publish the signal to the broker.
TEST_F(TestableServerTest, SingleOptionalArrayOfStringsEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/singleOptionalArrayOfStrings", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitSingleOptionalArrayOfStringsSignal(std::vector<std::string>{ "apples", "foo", "foo" });

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// Emitting `arrayOfEveryType` should publish the signal to the broker.
TEST_F(TestableServerTest, ArrayOfEveryTypeEmitPublishes)
{
    const std::string topic = stinger::utils::format("{prefix}/testable/{service_id}/signal/arrayOfEveryType", topicArgs());
    _conn->ClearPublishedMessages();

    auto pending = _server->emitArrayOfEveryTypeSignal(std::vector<int>{ 42, 2022, 2022 }, std::vector<double>{ 3.14, 1.0, 1.0 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } });

    auto published = _conn->GetPublishedMessages(topic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published signal payload is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `callWithNothing` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallWithNothingRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallWithNothingHandler([handlerCalled]() -> void
                                            {
                                                *handlerCalled = true;
                                            });

    const std::string payload = "{}";

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callWithNothing/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callWithNothing/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOneInteger` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOneIntegerRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOneIntegerHandler([handlerCalled](int) -> int
                                           {
                                               *handlerCalled = true;
                                               return 42;
                                           });

    CallOneIntegerRequestArguments requestArgs{ 42 };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOneInteger/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOneInteger/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOptionalInteger` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOptionalIntegerRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOptionalIntegerHandler([handlerCalled](std::optional<int>) -> std::optional<int>
                                                {
                                                    *handlerCalled = true;
                                                    return 42;
                                                });

    CallOptionalIntegerRequestArguments requestArgs{ 42 };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalInteger/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOptionalInteger/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callThreeIntegers` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallThreeIntegersRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallThreeIntegersHandler([handlerCalled](int, int, std::optional<int>) -> CallThreeIntegersReturnValues
                                              {
                                                  *handlerCalled = true;
                                                  return CallThreeIntegersReturnValues{ 42, 42, 42 };
                                              });

    CallThreeIntegersRequestArguments requestArgs{ 42, 42, 42 };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeIntegers/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callThreeIntegers/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOneString` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOneStringRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOneStringHandler([handlerCalled](std::string) -> std::string
                                          {
                                              *handlerCalled = true;
                                              return "apples";
                                          });

    CallOneStringRequestArguments requestArgs{ "apples" };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOneString/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOneString/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOptionalString` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOptionalStringRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOptionalStringHandler([handlerCalled](std::optional<std::string>) -> std::optional<std::string>
                                               {
                                                   *handlerCalled = true;
                                                   return std::make_optional(std::string("apples"));
                                               });

    CallOptionalStringRequestArguments requestArgs{ std::make_optional(std::string("apples")) };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalString/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOptionalString/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callThreeStrings` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallThreeStringsRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallThreeStringsHandler([handlerCalled](std::string, std::optional<std::string>, std::string) -> CallThreeStringsReturnValues
                                             {
                                                 *handlerCalled = true;
                                                 return CallThreeStringsReturnValues{ "apples", std::make_optional(std::string("apples")), "apples" };
                                             });

    CallThreeStringsRequestArguments requestArgs{ "apples", std::make_optional(std::string("apples")), "apples" };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeStrings/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callThreeStrings/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOneEnum` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOneEnumRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOneEnumHandler([handlerCalled](Numbers) -> Numbers
                                        {
                                            *handlerCalled = true;
                                            return Numbers::ONE;
                                        });

    CallOneEnumRequestArguments requestArgs{ Numbers::ONE };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOneEnum/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOneEnum/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOptionalEnum` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOptionalEnumRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOptionalEnumHandler([handlerCalled](std::optional<Numbers>) -> std::optional<Numbers>
                                             {
                                                 *handlerCalled = true;
                                                 return Numbers::ONE;
                                             });

    CallOptionalEnumRequestArguments requestArgs{ Numbers::ONE };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalEnum/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOptionalEnum/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callThreeEnums` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallThreeEnumsRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallThreeEnumsHandler([handlerCalled](Numbers, Numbers, std::optional<Numbers>) -> CallThreeEnumsReturnValues
                                           {
                                               *handlerCalled = true;
                                               return CallThreeEnumsReturnValues{ Numbers::ONE, Numbers::ONE, Numbers::ONE };
                                           });

    CallThreeEnumsRequestArguments requestArgs{ Numbers::ONE, Numbers::ONE, Numbers::ONE };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeEnums/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callThreeEnums/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOneStruct` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOneStructRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOneStructHandler([handlerCalled](AllTypes) -> AllTypes
                                          {
                                              *handlerCalled = true;
                                              return AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } };
                                          });

    CallOneStructRequestArguments requestArgs{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOneStruct/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOneStruct/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOptionalStruct` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOptionalStructRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOptionalStructHandler([handlerCalled](std::optional<AllTypes>) -> std::optional<AllTypes>
                                               {
                                                   *handlerCalled = true;
                                                   return AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } };
                                               });

    CallOptionalStructRequestArguments requestArgs{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalStruct/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOptionalStruct/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callThreeStructs` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallThreeStructsRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallThreeStructsHandler([handlerCalled](std::optional<AllTypes>, AllTypes, AllTypes) -> CallThreeStructsReturnValues
                                             {
                                                 *handlerCalled = true;
                                                 return CallThreeStructsReturnValues{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
                                             });

    CallThreeStructsRequestArguments requestArgs{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeStructs/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callThreeStructs/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOneDateTime` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOneDateTimeRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOneDateTimeHandler([handlerCalled](std::chrono::time_point<std::chrono::system_clock>) -> std::chrono::time_point<std::chrono::system_clock>
                                            {
                                                *handlerCalled = true;
                                                return std::chrono::system_clock::now();
                                            });

    CallOneDateTimeRequestArguments requestArgs{ std::chrono::system_clock::now() };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOneDateTime/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOneDateTime/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOptionalDateTime` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOptionalDateTimeRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOptionalDateTimeHandler([handlerCalled](std::optional<std::chrono::time_point<std::chrono::system_clock>>) -> std::optional<std::chrono::time_point<std::chrono::system_clock>>
                                                 {
                                                     *handlerCalled = true;
                                                     return std::chrono::system_clock::now();
                                                 });

    CallOptionalDateTimeRequestArguments requestArgs{ std::chrono::system_clock::now() };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalDateTime/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOptionalDateTime/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callThreeDateTimes` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallThreeDateTimesRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallThreeDateTimesHandler([handlerCalled](std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>, std::optional<std::chrono::time_point<std::chrono::system_clock>>) -> CallThreeDateTimesReturnValues
                                               {
                                                   *handlerCalled = true;
                                                   return CallThreeDateTimesReturnValues{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() };
                                               });

    CallThreeDateTimesRequestArguments requestArgs{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeDateTimes/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callThreeDateTimes/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOneDuration` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOneDurationRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOneDurationHandler([handlerCalled](std::chrono::duration<double>) -> std::chrono::duration<double>
                                            {
                                                *handlerCalled = true;
                                                return std::chrono::duration<double>(3536);
                                            });

    CallOneDurationRequestArguments requestArgs{ std::chrono::duration<double>(3536) };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOneDuration/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOneDuration/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOptionalDuration` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOptionalDurationRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOptionalDurationHandler([handlerCalled](std::optional<std::chrono::duration<double>>) -> std::optional<std::chrono::duration<double>>
                                                 {
                                                     *handlerCalled = true;
                                                     return std::chrono::duration<double>(3536);
                                                 });

    CallOptionalDurationRequestArguments requestArgs{ std::chrono::duration<double>(3536) };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalDuration/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOptionalDuration/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callThreeDurations` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallThreeDurationsRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallThreeDurationsHandler([handlerCalled](std::chrono::duration<double>, std::chrono::duration<double>, std::optional<std::chrono::duration<double>>) -> CallThreeDurationsReturnValues
                                               {
                                                   *handlerCalled = true;
                                                   return CallThreeDurationsReturnValues{ std::chrono::duration<double>(3536), std::chrono::duration<double>(3536), std::chrono::duration<double>(3536) };
                                               });

    CallThreeDurationsRequestArguments requestArgs{ std::chrono::duration<double>(3536), std::chrono::duration<double>(3536), std::chrono::duration<double>(3536) };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeDurations/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callThreeDurations/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOneBinary` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOneBinaryRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOneBinaryHandler([handlerCalled](std::vector<uint8_t>) -> std::vector<uint8_t>
                                          {
                                              *handlerCalled = true;
                                              return std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 };
                                          });

    CallOneBinaryRequestArguments requestArgs{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOneBinary/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOneBinary/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOptionalBinary` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOptionalBinaryRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOptionalBinaryHandler([handlerCalled](std::optional<std::vector<uint8_t>>) -> std::optional<std::vector<uint8_t>>
                                               {
                                                   *handlerCalled = true;
                                                   return std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 };
                                               });

    CallOptionalBinaryRequestArguments requestArgs{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalBinary/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOptionalBinary/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callThreeBinaries` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallThreeBinariesRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallThreeBinariesHandler([handlerCalled](std::vector<uint8_t>, std::vector<uint8_t>, std::optional<std::vector<uint8_t>>) -> CallThreeBinariesReturnValues
                                              {
                                                  *handlerCalled = true;
                                                  return CallThreeBinariesReturnValues{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
                                              });

    CallThreeBinariesRequestArguments requestArgs{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callThreeBinaries/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callThreeBinaries/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOneListOfIntegers` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOneListOfIntegersRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOneListOfIntegersHandler([handlerCalled](std::vector<int>) -> std::vector<int>
                                                  {
                                                      *handlerCalled = true;
                                                      return std::vector<int>{ 42, 2022, 2022 };
                                                  });

    CallOneListOfIntegersRequestArguments requestArgs{ std::vector<int>{ 42, 2022, 2022 } };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOneListOfIntegers/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOneListOfIntegers/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callOptionalListOfFloats` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallOptionalListOfFloatsRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallOptionalListOfFloatsHandler([handlerCalled](std::optional<std::vector<double>>) -> std::optional<std::vector<double>>
                                                     {
                                                         *handlerCalled = true;
                                                         return std::vector<double>{ 3.14, 1.0, 1.0 };
                                                     });

    CallOptionalListOfFloatsRequestArguments requestArgs{ std::vector<double>{ 3.14, 1.0, 1.0 } };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callOptionalListOfFloats/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callOptionalListOfFloats/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// A simulated `callTwoLists` request should invoke the registered handler and publish a response.
TEST_F(TestableServerTest, CallTwoListsRequestInvokesHandler)
{
    // Captured by value (shared_ptr) so the registered handler stays valid for the
    // lifetime of the shared server, even after this test returns.
    auto handlerCalled = std::make_shared<bool>(false);
    _server->registerCallTwoListsHandler([handlerCalled](std::vector<Numbers>, std::optional<std::vector<std::string>>) -> CallTwoListsReturnValues
                                         {
                                             *handlerCalled = true;
                                             return CallTwoListsReturnValues{ std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::string>{ "apples", "foo", "foo" } };
                                         });

    CallTwoListsRequestArguments requestArgs{ std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::string>{ "apples", "foo", "foo" } };
    const std::string payload = serializeToString(requestArgs);

    const std::string requestTopic = stinger::utils::format("{prefix}/testable/{service_id}/method/callTwoLists/request", topicArgs());
    const std::string responseTopic = stinger::utils::format("client/{client_id}/testable/method/callTwoLists/response", topicArgs());
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 }, std::byte{ 0x02 } };
    auto msg = stinger::mqtt::Message::MethodRequest(requestTopic, payload, correlationData, responseTopic);

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_TRUE(*handlerCalled);
    // The server publishes a response (success or error) to the response topic.
    EXPECT_EQ(_conn->GetPublishedMessages(responseTopic).size(), 1u);
}

// Setting `read_write_integer` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteIntegerSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_integer/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteIntegerProperty(42);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_integer` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteIntegerUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_integer/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_integer/value", topicArgs());

    ReadWriteIntegerProperty newValue{ 42 };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_only_integer` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadOnlyIntegerSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_integer/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadOnlyIntegerProperty(42);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_only_integer` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadOnlyIntegerUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_integer/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_integer/value", topicArgs());

    ReadOnlyIntegerProperty newValue{ 42 };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_optional_integer` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteOptionalIntegerSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_integer/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteOptionalIntegerProperty(42);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_optional_integer` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteOptionalIntegerUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_integer/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_integer/value", topicArgs());

    ReadWriteOptionalIntegerProperty newValue{ 42 };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_two_integers` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteTwoIntegersSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_integers/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteTwoIntegersProperty(42, 42);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_two_integers` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteTwoIntegersUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_integers/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_integers/value", topicArgs());

    ReadWriteTwoIntegersProperty newValue{ 42, 42 };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_only_string` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadOnlyStringSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_string/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadOnlyStringProperty("apples");

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_only_string` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadOnlyStringUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_string/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_string/value", topicArgs());

    ReadOnlyStringProperty newValue{ "apples" };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_string` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteStringSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_string/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteStringProperty("apples");

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_string` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteStringUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_string/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_string/value", topicArgs());

    ReadWriteStringProperty newValue{ "apples" };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_optional_string` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteOptionalStringSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_string/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteOptionalStringProperty(std::make_optional(std::string("apples")));

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_optional_string` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteOptionalStringUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_string/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_string/value", topicArgs());

    ReadWriteOptionalStringProperty newValue{ std::make_optional(std::string("apples")) };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_two_strings` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteTwoStringsSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_strings/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteTwoStringsProperty("apples", std::make_optional(std::string("apples")));

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_two_strings` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteTwoStringsUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_strings/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_strings/value", topicArgs());

    ReadWriteTwoStringsProperty newValue{ "apples", std::make_optional(std::string("apples")) };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_struct` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteStructSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_struct/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteStructProperty(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_struct` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteStructUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_struct/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_struct/value", topicArgs());

    ReadWriteStructProperty newValue{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_optional_struct` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteOptionalStructSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_struct/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteOptionalStructProperty(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_optional_struct` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteOptionalStructUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_struct/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_struct/value", topicArgs());

    ReadWriteOptionalStructProperty newValue{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_two_structs` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteTwoStructsSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_structs/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteTwoStructsProperty(AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } });

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_two_structs` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteTwoStructsUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_structs/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_structs/value", topicArgs());

    ReadWriteTwoStructsProperty newValue{ AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } }, AllTypes{ true, 42, 3.14, "apples", Numbers::ONE, Lunch{ true, "apples", 3.14, 42, std::chrono::system_clock::now(), std::chrono::duration<double>(3536) }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, 42, std::make_optional(std::string("apples")), Numbers::ONE, Entry{ 42, "apples" }, std::chrono::system_clock::now(), std::chrono::duration<double>(3536), std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<int>{ 42, 2022, 2022 }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<std::string>{ "apples", "foo", "foo" }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::chrono::duration<double>>{ std::chrono::duration<double>(3536), std::chrono::duration<double>(975), std::chrono::duration<double>(967) }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<std::vector<uint8_t>>{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } }, std::vector<Entry>{ Entry{ 42, "apples" }, Entry{ 2022, "foo" }, Entry{ 2022, "foo" } } } };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_only_enum` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadOnlyEnumSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_enum/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadOnlyEnumProperty(Numbers::ONE);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_only_enum` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadOnlyEnumUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_enum/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_only_enum/value", topicArgs());

    ReadOnlyEnumProperty newValue{ Numbers::ONE };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_enum` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteEnumSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_enum/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteEnumProperty(Numbers::ONE);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_enum` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteEnumUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_enum/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_enum/value", topicArgs());

    ReadWriteEnumProperty newValue{ Numbers::ONE };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_optional_enum` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteOptionalEnumSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_enum/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteOptionalEnumProperty(Numbers::ONE);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_optional_enum` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteOptionalEnumUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_enum/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_enum/value", topicArgs());

    ReadWriteOptionalEnumProperty newValue{ Numbers::ONE };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_two_enums` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteTwoEnumsSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_enums/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteTwoEnumsProperty(Numbers::ONE, Numbers::ONE);

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_two_enums` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteTwoEnumsUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_enums/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_enums/value", topicArgs());

    ReadWriteTwoEnumsProperty newValue{ Numbers::ONE, Numbers::ONE };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_datetime` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteDatetimeSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_datetime/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteDatetimeProperty(std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_datetime` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteDatetimeUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_datetime/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_datetime/value", topicArgs());

    ReadWriteDatetimeProperty newValue{ std::chrono::system_clock::now() };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_optional_datetime` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteOptionalDatetimeSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_datetime/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteOptionalDatetimeProperty(std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_optional_datetime` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteOptionalDatetimeUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_datetime/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_datetime/value", topicArgs());

    ReadWriteOptionalDatetimeProperty newValue{ std::chrono::system_clock::now() };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_two_datetimes` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteTwoDatetimesSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_datetimes/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteTwoDatetimesProperty(std::chrono::system_clock::now(), std::chrono::system_clock::now());

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_two_datetimes` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteTwoDatetimesUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_datetimes/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_datetimes/value", topicArgs());

    ReadWriteTwoDatetimesProperty newValue{ std::chrono::system_clock::now(), std::chrono::system_clock::now() };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_duration` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteDurationSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_duration/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteDurationProperty(std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_duration` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteDurationUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_duration/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_duration/value", topicArgs());

    ReadWriteDurationProperty newValue{ std::chrono::duration<double>(3536) };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_optional_duration` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteOptionalDurationSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_duration/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteOptionalDurationProperty(std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_optional_duration` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteOptionalDurationUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_duration/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_duration/value", topicArgs());

    ReadWriteOptionalDurationProperty newValue{ std::chrono::duration<double>(3536) };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_two_durations` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteTwoDurationsSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_durations/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteTwoDurationsProperty(std::chrono::duration<double>(3536), std::chrono::duration<double>(3536));

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_two_durations` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteTwoDurationsUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_durations/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_durations/value", topicArgs());

    ReadWriteTwoDurationsProperty newValue{ std::chrono::duration<double>(3536), std::chrono::duration<double>(3536) };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_binary` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteBinarySetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_binary/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteBinaryProperty(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_binary` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteBinaryUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_binary/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_binary/value", topicArgs());

    ReadWriteBinaryProperty newValue{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_optional_binary` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteOptionalBinarySetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_binary/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteOptionalBinaryProperty(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_optional_binary` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteOptionalBinaryUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_binary/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_optional_binary/value", topicArgs());

    ReadWriteOptionalBinaryProperty newValue{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_two_binaries` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteTwoBinariesSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_binaries/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteTwoBinariesProperty(std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 });

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_two_binaries` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteTwoBinariesUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_binaries/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_two_binaries/value", topicArgs());

    ReadWriteTwoBinariesProperty newValue{ std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 }, std::vector<uint8_t>{ 101, 120, 97, 109, 112, 108, 101 } };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_list_of_strings` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteListOfStringsSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_list_of_strings/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteListOfStringsProperty(std::vector<std::string>{ "apples", "foo", "foo" });

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_list_of_strings` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteListOfStringsUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_list_of_strings/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_list_of_strings/value", topicArgs());

    ReadWriteListOfStringsProperty newValue{ std::vector<std::string>{ "apples", "foo", "foo" } };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}

// Setting `read_write_lists` on the server should publish its value to the broker.
TEST_F(TestableServerTest, ReadWriteListsSetterPublishesValue)
{
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_lists/value", topicArgs());
    _conn->ClearPublishedMessages();

    _server->updateReadWriteListsProperty(std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() });

    auto published = _conn->GetPublishedMessages(valueTopic);
    ASSERT_EQ(published.size(), 1u);

    rapidjson::Document doc;
    EXPECT_FALSE(doc.Parse(published[0].payload.c_str()).HasParseError()) << "Published property value is not valid JSON";
    EXPECT_TRUE(doc.IsObject());
}

// A simulated `read_write_lists` update request should be applied and re-published as the new value.
TEST_F(TestableServerTest, ReadWriteListsUpdateRequestIsRepublished)
{
    const std::string updateTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_lists/update", topicArgs());
    const std::string valueTopic = stinger::utils::format("{prefix}/testable/{service_id}/property/read_write_lists/value", topicArgs());

    ReadWriteListsProperty newValue{ std::vector<Numbers>{ Numbers::ONE, Numbers::ONE, Numbers::ONE }, std::vector<std::chrono::time_point<std::chrono::system_clock>>{ std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() } };
    const std::string payload = serializeToString(newValue);
    const std::vector<std::byte> correlationData{ std::byte{ 0x01 } };
    auto msg = stinger::mqtt::Message::PropertyUpdateRequest(updateTopic, payload, 1, correlationData, "test/response");

    _conn->ClearPublishedMessages();
    _conn->SimulateIncomingMessage(msg);

    EXPECT_EQ(_conn->GetPublishedMessages(valueTopic).size(), 1u);
}
