/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

It contains enumerations used by the prop-only interface.

LICENSE: This generated code is not subject to any license restrictions from the generator itself.
TODO: Get license text from stinger file
*/

#pragma once
#include <string>
#include <rapidjson/document.h>
#include "enums.hpp"
#include "structs.hpp"

namespace stinger {

namespace gen {
namespace prop_only {

/**
 * The current home address.
 */
struct HomeAddressProperty {
    static HomeAddressProperty FromRapidJsonObject(const rapidjson::Value& jsonObj);
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    Address address;
};

/**
 * The user's favorite country.
 */
struct FavoriteCountryProperty {
    static FavoriteCountryProperty FromRapidJsonObject(const rapidjson::Value& jsonObj);
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    Country country;
};

} // namespace prop_only

} // namespace gen

} // namespace stinger
