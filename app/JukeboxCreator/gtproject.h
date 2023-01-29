#ifndef GTPROJECT_H
#define GTPROJECT_H

#include <filesystem>
#include <vector>
#include "gtscene.h"
#include "gtsprite.h"
#include "gtbehavior.h"
#include "jsonbase.h"

using namespace std;
using namespace filesystem;

class GTProject : public JSONBase
{
public:
    GTProject();

    virtual bool Deserialize(const rapidjson::Value& obj);
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;

    string name;
    std::vector<GTSprite> sprites;
    std::vector<GTScene> scenes;
    std::vector<GTBehavior> behaviors;

    path projectRoot;
    void Save();
};

#endif // GTPROJECT_H
