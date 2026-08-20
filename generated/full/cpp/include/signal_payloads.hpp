/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

It contains enumerations used by the Full interface.


*/

#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <optional>
#include "enums.hpp"
#include "structs.hpp"

namespace stinger {

namespace gen {
namespace full {

struct TodayIsPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static TodayIsPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    int dayOfMonth;
    DayOfTheWeek dayOfWeek;
};

struct RandomWordPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static RandomWordPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::string word;
    std::chrono::time_point<std::chrono::system_clock> time;
};

} // namespace full

} // namespace gen

} // namespace stinger
