

#include "property_structs.hpp"
#include <rapidjson/document.h>

namespace stinger {

namespace gen {
namespace prop_only {

HomeAddressProperty HomeAddressProperty::FromRapidJsonObject(const rapidjson::Value& jsonObj)
{
    HomeAddressProperty homeAddress;

    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = jsonObj.FindMember("address");
        if (itr != jsonObj.MemberEnd() && itr->value.IsObject()) {
            homeAddress.address = Address::FromRapidJsonObject(itr->value);

        } else {
            throw std::runtime_error("Received payload for the 'address' argument doesn't have required value/type");
        }
    }

    return homeAddress;
};

void HomeAddressProperty::AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const
{
    { // Restrict Scope for struct serialization
        rapidjson::Value tempStructValue;

        tempStructValue.SetObject();
        address.AddToRapidJsonObject(tempStructValue, allocator);

        parent.AddMember("address", tempStructValue, allocator);
    }
}

FavoriteCountryProperty FavoriteCountryProperty::FromRapidJsonObject(const rapidjson::Value& jsonObj)
{
    FavoriteCountryProperty favoriteCountry;

    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = jsonObj.FindMember("country");
        if (itr != jsonObj.MemberEnd() && itr->value.IsInt()) {
            favoriteCountry.country = static_cast<Country>(itr->value.GetInt());

        } else {
            throw std::runtime_error("Received payload for the 'country' argument doesn't have required value/type");
        }
    }

    return favoriteCountry;
};

void FavoriteCountryProperty::AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const
{
    parent.AddMember("country", static_cast<int>(country), allocator);
}

} // namespace prop_only

} // namespace gen

} // namespace stinger
