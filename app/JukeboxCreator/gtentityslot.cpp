#include "gtentityslot.h"
#include "jsonmacros.h"

GTEntitySlot::GTEntitySlot()
{

}

bool GTEntitySlot::Deserialize(const rapidjson::Value& obj)
{
    JSON_READ(String, sprite_name);
    JSON_READ(Int, behavior_id);
    return true;
}

bool GTEntitySlot::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
    writer->StartObject();
    JSON_EMIT_STRING(sprite_name);
    JSON_EMIT(Int, behavior_id);
    writer->EndObject();
    return true;
}
