
#include "method_payloads.hpp"

#include <rapidjson/document.h>
#include <rapidjson/schema.h>
#include <stinger/utils/conversions.hpp>

namespace stinger {

namespace gen {
namespace simple {

// --- (De-)Serialization for trade_numbers method request arguments ---
TradeNumbersRequestArguments TradeNumbersRequestArguments::FromRapidJsonObject(const rapidjson::Value& jsonObj)
{
    TradeNumbersRequestArguments tradeNumbersArgs;

    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = jsonObj.FindMember("your_number");
        if (itr != jsonObj.MemberEnd() && itr->value.IsInt()) {
            tradeNumbersArgs.yourNumber = itr->value.GetInt();

        } else {
            throw std::runtime_error("Received payload for the 'your_number' argument doesn't have required value/type");
        }
    }

    return tradeNumbersArgs;
};

void TradeNumbersRequestArguments::AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const
{
    parent.AddMember("your_number", yourNumber, allocator);
}

bool TradeNumbersRequestArguments::ValidateSchema() const
{
    rapidjson::Document doc;
    doc.SetObject();
    AddToRapidJsonObject(doc, doc.GetAllocator());
    {
        rapidjson::Document schemaDoc;
        if (schemaDoc.Parse(R"stingerschema({"maximum": 10000, "minimum": 0})stingerschema").HasParseError()) {
            return false;
        }
        rapidjson::SchemaDocument schema(schemaDoc);
        rapidjson::SchemaValidator validator(schema);
        auto itr = doc.FindMember("your_number");
        if (itr != doc.MemberEnd() && !itr->value.Accept(validator)) {
            return false;
        }
    }
    return true;
}

// --- (De-)Serialization for trade_numbers method return type ---
TradeNumbersReturnValues TradeNumbersReturnValues::FromRapidJsonObject(const rapidjson::Value& jsonObj)
{
    TradeNumbersReturnValues tradeNumbersRc;

    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = jsonObj.FindMember("my_number");
        if (itr != jsonObj.MemberEnd() && itr->value.IsInt()) {
            tradeNumbersRc.myNumber = itr->value.GetInt();

        } else {
            throw std::runtime_error("Received payload for the 'my_number' argument doesn't have required value/type");
        }
    }

    return tradeNumbersRc;
};

void TradeNumbersReturnValues::AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const
{
    parent.AddMember("my_number", myNumber, allocator);
}

bool TradeNumbersReturnValues::ValidateSchema() const
{
    return true;
}

} // namespace simple

} // namespace gen

} // namespace stinger
