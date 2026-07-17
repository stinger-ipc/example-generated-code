

#include "structs.hpp"

namespace stinger {

namespace gen {
namespace prop_only {

Address Address::FromRapidJsonObject(const rapidjson::Value& jsonObj)
{
    Address address;

    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = jsonObj.FindMember("street");
        if (itr != jsonObj.MemberEnd() && itr->value.IsString()) {
            address.street = itr->value.GetString();

        } else {
            throw std::runtime_error("Received payload for the 'street' argument doesn't have required value/type");
        }
    }
    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = jsonObj.FindMember("city");
        if (itr != jsonObj.MemberEnd() && itr->value.IsString()) {
            address.city = itr->value.GetString();

        } else {
            throw std::runtime_error("Received payload for the 'city' argument doesn't have required value/type");
        }
    }
    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = jsonObj.FindMember("state");
        if (itr != jsonObj.MemberEnd() && itr->value.IsString()) {
            address.state = itr->value.GetString();

        } else {
            throw std::runtime_error("Received payload for the 'state' argument doesn't have required value/type");
        }
    }
    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = jsonObj.FindMember("postal_code");
        if (itr != jsonObj.MemberEnd() && itr->value.IsString()) {
            address.postalCode = itr->value.GetString();

        } else {
            throw std::runtime_error("Received payload for the 'postal_code' argument doesn't have required value/type");
        }
    }
    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = jsonObj.FindMember("country");
        if (itr != jsonObj.MemberEnd() && itr->value.IsInt()) {
            address.country = static_cast<Country>(itr->value.GetInt());

        } else {
            throw std::runtime_error("Received payload for the 'country' argument doesn't have required value/type");
        }
    }

    return address;
};

void Address::AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const
{
    { // restrict scope
        rapidjson::Value tempStringValue;
        tempStringValue.SetString(street.c_str(), street.size(), allocator);
        parent.AddMember("street", tempStringValue, allocator);
    }

    { // restrict scope
        rapidjson::Value tempStringValue;
        tempStringValue.SetString(city.c_str(), city.size(), allocator);
        parent.AddMember("city", tempStringValue, allocator);
    }

    { // restrict scope
        rapidjson::Value tempStringValue;
        tempStringValue.SetString(state.c_str(), state.size(), allocator);
        parent.AddMember("state", tempStringValue, allocator);
    }

    { // restrict scope
        rapidjson::Value tempStringValue;
        tempStringValue.SetString(postalCode.c_str(), postalCode.size(), allocator);
        parent.AddMember("postal_code", tempStringValue, allocator);
    }

    parent.AddMember("country", static_cast<int>(country), allocator);
}

} // namespace prop_only

} // namespace gen

} // namespace stinger
