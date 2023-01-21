#include "gtentity.h"
#include "jsonmacros.h"

GTEntity::GTEntity()
{
    pixItem = nullptr;
}

bool GTEntity::Deserialize(const rapidjson::Value& obj)
{

    JSON_READ(Int, vx)
    JSON_READ(Int, vy)
    JSON_READ(Int, frame)
    JSON_READ(Int, slot)
    JSON_READ(Int, hp)
    JSON_READ(Int, state)
    return true;
}

bool GTEntity::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
    writer->StartObject();
    JSON_EMIT(Int, vx);
    JSON_EMIT(Int, vy);
    JSON_EMIT(Int, frame);
    JSON_EMIT(Int, slot);
    JSON_EMIT(Int, hp);
    JSON_EMIT(Int, state);
    writer->EndObject();
    return true;
}
