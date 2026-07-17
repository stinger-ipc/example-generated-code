/*
DO NOT MODIFY THIS FILE.  It is automatically generated and changes will be over-written
on the next generation.

It contains enumerations used by the prop-only interface.

LICENSE: This generated code is not subject to any license restrictions from the generator itself.
TODO: Get license text from stinger file
*/

#pragma once

#include <map>
#include <string>

namespace stinger {

namespace gen {
namespace prop_only {

/** 
 * @enum Country
 * @brief A small set of supported countries.
 */
enum class Country {
    USA = 1, // United States of America.
    CANADA = 2,
    MEXICO = 3
};
static const std::map<int, std::string> countryStrings = {
    { 1, "USA" },

    { 2, "Canada" },

    { 3, "Mexico" }
};

} // namespace prop_only

} // namespace gen

} // namespace stinger
