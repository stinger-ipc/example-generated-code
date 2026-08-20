/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

It contains enumerations used by the testable interface.


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
namespace testable {

struct EmptyPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static EmptyPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
};

struct SingleIntPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleIntPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    int value;
};

struct JsonSchemaValidatedIntPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static JsonSchemaValidatedIntPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    int value;
};

struct SingleOptionalIntPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleOptionalIntPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::optional<int> value;
};

struct ThreeIntegersPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static ThreeIntegersPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    int first;
    int second;
    std::optional<int> third;
};

struct SingleStringPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleStringPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::string value;
};

struct JsonSchemaValidatedStringPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static JsonSchemaValidatedStringPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::string value;
};

struct SingleOptionalStringPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleOptionalStringPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::optional<std::string> value;
};

struct ThreeStringsPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static ThreeStringsPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::string first;
    std::string second;
    std::optional<std::string> third;
};

struct SingleEnumPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleEnumPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    Numbers value;
};

struct SingleOptionalEnumPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleOptionalEnumPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::optional<Numbers> value;
};

struct ThreeEnumsPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static ThreeEnumsPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    Numbers first;
    Numbers second;
    std::optional<Numbers> third;
};

struct SingleStructPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleStructPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    AllTypes value;
};

struct SingleOptionalStructPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleOptionalStructPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::optional<AllTypes> value;
};

struct ThreeStructsPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static ThreeStructsPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    AllTypes first;
    AllTypes second;
    std::optional<AllTypes> third;
};

struct SingleDateTimePayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleDateTimePayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::chrono::time_point<std::chrono::system_clock> value;
};

struct SingleOptionalDatetimePayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleOptionalDatetimePayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::optional<std::chrono::time_point<std::chrono::system_clock>> value;
};

struct ThreeDateTimesPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static ThreeDateTimesPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::chrono::time_point<std::chrono::system_clock> first;
    std::chrono::time_point<std::chrono::system_clock> second;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> third;
};

struct SingleDurationPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleDurationPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::chrono::duration<double> value;
};

struct SingleOptionalDurationPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleOptionalDurationPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::optional<std::chrono::duration<double>> value;
};

struct ThreeDurationsPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static ThreeDurationsPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::chrono::duration<double> first;
    std::chrono::duration<double> second;
    std::optional<std::chrono::duration<double>> third;
};

struct SingleBinaryPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleBinaryPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::vector<uint8_t> value;
};

struct SingleOptionalBinaryPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleOptionalBinaryPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::optional<std::vector<uint8_t>> value;
};

struct ThreeBinariesPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static ThreeBinariesPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::vector<uint8_t> first;
    std::vector<uint8_t> second;
    std::optional<std::vector<uint8_t>> third;
};

struct SingleArrayOfIntegersPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleArrayOfIntegersPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::vector<int> values;
};

struct SingleOptionalArrayOfStringsPayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static SingleOptionalArrayOfStringsPayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::optional<std::vector<std::string>> values;
};

struct ArrayOfEveryTypePayload {
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    static ArrayOfEveryTypePayload FromRapidJsonObject(const rapidjson::Value& jsonObj);
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    // Values...
    std::vector<int> firstOfIntegers;
    std::vector<double> secondOfFloats;
    std::vector<std::string> thirdOfStrings;
    std::vector<Numbers> fourthOfEnums;
    std::vector<Entry> fifthOfStructs;
    std::vector<std::chrono::time_point<std::chrono::system_clock>> sixthOfDatetimes;
    std::vector<std::chrono::duration<double>> seventhOfDurations;
    std::vector<std::vector<uint8_t>> eighthOfBinaries;
};

} // namespace testable

} // namespace gen

} // namespace stinger
