#include "gtentityslot.h"
#include "jsonmacros.h"

GTEntitySlot::GTEntitySlot()
{
    behaviors = new BehaviorsListModel();
}

bool GTEntitySlot::Deserialize(const rapidjson::Value& obj)
{
    JSON_READ(String, sprite_name);

    behaviors->Deserialize(obj["behaviors"]);

    JSON_READ(Int, type);
    return true;
}

bool GTEntitySlot::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
    writer->StartObject();
    JSON_EMIT_STRING(sprite_name);

    writer->String("behaviors");
    behaviors->Serialize(writer);

    JSON_EMIT(Int, type);
    writer->EndObject();
    return true;
}
