/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

It contains enumerations used by the Simple interface.


*/

#pragma once
#include <string>
#include <chrono>
#include <vector>
#include "enums.hpp"
#include "structs.hpp"

namespace stinger {

namespace gen {
namespace simple {

struct TradeNumbersRequestArguments {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static TradeNumbersRequestArguments FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    int yourNumber;
};

struct TradeNumbersReturnValues {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static TradeNumbersReturnValues FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    int myNumber;
};

} // namespace simple

} // namespace gen

} // namespace stinger
