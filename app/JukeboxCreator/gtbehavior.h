#ifndef GTBEHAVIOR_H
#define GTBEHAVIOR_H

#include <map>
#include <filesystem>
#include "jsonbase.h"

class GTBehavior : public JSONBase
{
public:
    GTBehavior();
    GTBehavior(std::string name, std::filesystem::path source);
    virtual bool Deserialize(const rapidjson::Value& obj);
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;

    std::string name;
    std::filesystem::path source;
    std::map<std::string, int> params;

    std::string GetCode(std::filesystem::path root);

private:
    std::string ParamDefineSection;
};


#endif // GTBEHAVIOR_H
