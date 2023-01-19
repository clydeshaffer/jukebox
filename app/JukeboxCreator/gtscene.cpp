#include "gtscene.h"

GTScene::GTScene()
{

}

bool GTScene::Deserialize(const rapidjson::Value& obj)
{
    auto entitySlotsJSONArray = obj["entitySlots"].GetArray();
    for(rapidjson::Value::ConstValueIterator itr = entitySlotsJSONArray.Begin(); itr != entitySlotsJSONArray.end(); ++itr) {
        entitySlots.push_back(GTEntitySlot());
        entitySlots.back().Deserialize((*itr));
    }

    auto entitiesJSONArray = obj["entities"].GetArray();
    for(rapidjson::Value::ConstValueIterator itr = entitiesJSONArray.Begin(); itr != entitiesJSONArray.end(); ++itr) {
        entities.push_back(GTEntity());
        entities.back().Deserialize((*itr));
    }

    return true;
}

bool GTScene::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
    writer->StartObject();

    writer->String("entitySlots");
    writer->StartArray();
    for(auto itr = entitySlots.begin(); itr != entitySlots.end(); ++itr) {
        (*itr).Serialize(writer);
    }
    writer->EndArray();

    writer->String("entities");
    writer->StartArray();
    for(auto itr = entities.begin(); itr != entities.end(); ++itr) {
        (*itr).Serialize(writer);
    }
    writer->EndArray();

    writer->EndObject();
    return true;
}
