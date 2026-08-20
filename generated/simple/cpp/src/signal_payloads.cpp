
#include "signal_payloads.hpp"
#include <rapidjson/document.h>
#include <rapidjson/schema.h>

namespace stinger {

namespace gen {
namespace simple {

// --- (De-)Serialization for person_entered signal payload ---
PersonEnteredPayload PersonEnteredPayload::FromRapidJsonObject(const rapidjson::Value& jsonObj)
{
    PersonEnteredPayload personEnteredPayload;

    { // Scoping
        rapidjson::Value::ConstMemberIterator itr = jsonObj.FindMember("person");
        if (itr != jsonObj.MemberEnd() && itr->value.IsObject()) {
            personEnteredPayload.person = Person::FromRapidJsonObject(itr->value);

        } else {
            throw std::runtime_error("Received payload for the 'person' argument doesn't have required value/type");
        }
    }

    return personEnteredPayload;
};

void PersonEnteredPayload::AddToRapidJsonObject(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator) const
{
    { // Restrict Scope for struct serialization
        rapidjson::Value tempStructValue;

        tempStructValue.SetObject();
        person.AddToRapidJsonObject(tempStructValue, allocator);

        parent.AddMember("person", tempStructValue, allocator);
    }
}

bool PersonEnteredPayload::ValidateSchema() const
{
    return true;
}

} // namespace simple

} // namespace gen

} // namespace stinger
