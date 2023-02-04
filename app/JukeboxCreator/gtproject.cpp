#include "gtproject.h"
#include "jsonmacros.h"

GTProject* GTProject::loadedProject = nullptr;

GTProject::GTProject()
{

}

bool GTProject::Deserialize(const rapidjson::Value& obj)
{
    homeDir = projectRoot;
    loadedProject = this;

    JSON_READ(String, name);

    auto spritesJSONArray = obj["sprites"].GetArray();
    for(rapidjson::Value::ConstValueIterator itr = spritesJSONArray.Begin(); itr != spritesJSONArray.end(); ++itr) {
        sprites.push_back(GTSprite());
        sprites.back().Deserialize((*itr));
        sprites.back().InitImageData(projectRoot);
    }

    auto behaviorsJSONArray = obj["behaviors"].GetArray();
    int behaviorIndex = 0;
    for(auto& item : behaviorsJSONArray) {
        behaviors.push_back(GTBehavior());
        behaviors.back().Deserialize(item);
        behaviors.back().cached_index = behaviorIndex++;
    }

    auto scenesJSONArray = obj["scenes"].GetArray();
    for(rapidjson::Value::ConstValueIterator itr = scenesJSONArray.Begin(); itr != scenesJSONArray.end(); ++itr) {
        scenes.push_back(GTScene());
        scenes.back().Deserialize((*itr));
    }

    return true;
}

bool GTProject::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
    writer->StartObject();

    JSON_EMIT_STRING(name);

    writer->String("sprites");
    writer->StartArray();
    for(auto itr = sprites.begin(); itr != sprites.end(); ++itr) {
        (*itr).Serialize(writer);
    }
    writer->EndArray();

    writer->String("behaviors");
    writer->StartArray();
    for(auto& behavior : behaviors) {
        behavior.Serialize(writer);
    }
    writer->EndArray();

    writer->String("scenes");
    writer->StartArray();
    for(auto itr = scenes.begin(); itr != scenes.end(); ++itr) {
        (*itr).Serialize(writer);
    }
    writer->EndArray();

    writer->EndObject();
    return true;
}

void GTProject::Save()
{
   for(int i = 0; i < behaviors.size(); ++i) {
       behaviors[i].cached_index = i;
   }

   path mainProjectJson = projectRoot / path("project.json");
   SerializeToFile(mainProjectJson.string());
}

void GTProject::EnsureSubdirs()
{
    path sub = projectRoot / path("sprites");
    if(!exists(sub)) {
        create_directory(sub);
    }

    sub = projectRoot / path("behaviors");
    if(!exists(sub)) {
        create_directory(sub);
    }

    sub = projectRoot / path("build");
    if(!exists(sub)) {
        create_directory(sub);
    }
}
