#ifndef GTBEHAVIOR_H
#define GTBEHAVIOR_H

#include <map>
#include <filesystem>
#include "jsonbase.h"
#include "assemblyunit.h"

class GTBehavior : public JSONBase, public AssemblyUnit
{
public:
    GTBehavior();
    GTBehavior(std::string name, std::filesystem::path source);
    virtual bool Deserialize(const rapidjson::Value& obj) override;
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const override;

    std::string name;
    std::string uid;
    std::filesystem::path source;
    std::map<std::string, int> params;

    void regenUID();

    virtual std::string GenName() override;
    virtual std::string GetCode(std::filesystem::path root) override;

    static GTBehavior* find(std::string uid);

};

struct GTBehaviorLink {
    std::string uid;
    GTBehavior *ptr;
};

#endif // GTBEHAVIOR_H
