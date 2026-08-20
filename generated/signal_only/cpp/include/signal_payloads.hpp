/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

It contains enumerations used by the SignalOnly interface.


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
namespace signal_only {

struct AnotherSignalPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static AnotherSignalPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    double one;
    bool two;
    std::string three;
};

struct BarkPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static BarkPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::string word;
};

struct MaybeNumberPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static MaybeNumberPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::optional<int> number;
};

struct MaybeNamePayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static MaybeNamePayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::optional<std::string> name;
};

struct NowPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static NowPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::chrono::time_point<std::chrono::system_clock> timestamp;
};

} // namespace signal_only

} // namespace gen

} // namespace stinger
