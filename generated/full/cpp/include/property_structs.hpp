/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

It contains enumerations used by the Full interface.


*/

#pragma once
#include <string>
#include <rapidjson/document.h>
#include "enums.hpp"
#include "structs.hpp"

namespace stinger {

namespace gen {
namespace full {

/**
 * My favorite number
 * 
 */
struct FavoriteNumberProperty {
    static FavoriteNumberProperty FromRapidJsonObject(const rapidjson::Value& jsonObj);
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    int number;
};

struct FavoriteFoodsProperty {
    static FavoriteFoodsProperty FromRapidJsonObject(const rapidjson::Value& jsonObj);
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    std::string drink;
    int slicesOfPizza;
    std::optional<std::string> breakfast;
};

struct LunchMenuProperty {
    static LunchMenuProperty FromRapidJsonObject(const rapidjson::Value& jsonObj);
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    Lunch monday;
    Lunch tuesday; ///< Tuesday's lunch menu.
};

/**
 * This is to test a property with a single string value.
 */
struct FamilyNameProperty {
    static FamilyNameProperty FromRapidJsonObject(const rapidjson::Value& jsonObj);
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    std::string familyName;
};

/**
 * This is to test a property with a single datetime value.
 */
struct LastBreakfastTimeProperty {
    static LastBreakfastTimeProperty FromRapidJsonObject(const rapidjson::Value& jsonObj);
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    std::chrono::time_point<std::chrono::system_clock> timestamp;
};

/**
 * This is to test a property with multiple datetime values.
 */
struct LastBirthdaysProperty {
    static LastBirthdaysProperty FromRapidJsonObject(const rapidjson::Value& jsonObj);
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    // Validates fields that declare a JSON schema constraint.  Returns true if all such fields conform to their schema (or if none declare one).
    bool ValidateSchema() const;
    std::chrono::time_point<std::chrono::system_clock> mom;
    std::chrono::time_point<std::chrono::system_clock> dad;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> sister;
    std::optional<int> brothersAge; ///< The age of my brothers in years.
};

} // namespace full

} // namespace gen

} // namespace stinger
