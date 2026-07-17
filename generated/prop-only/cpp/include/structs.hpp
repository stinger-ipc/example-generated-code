/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

It contains enumerations used by the prop-only interface.

LICENSE: This generated code is not subject to any license restrictions from the generator itself.
TODO: Get license text from stinger file
*/

#pragma once
#include <string>
#include <cstddef>
#include <chrono>
#include <vector>
#include <optional>
#include <rapidjson/document.h>
#include "enums.hpp"
#include <stinger/utils/conversions.hpp>

namespace stinger {

namespace gen {
namespace prop_only {

struct Address {
    static Address FromRapidJsonObject(const rapidjson::Value& jsonObj);
    void AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const;
    std::string street;
    std::string city;
    std::string state;
    std::string postalCode;
    Country country;
};

} // namespace prop_only

} // namespace gen

} // namespace stinger
