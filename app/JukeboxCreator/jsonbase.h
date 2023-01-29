#ifndef JSONBASE_H
#define JSONBASE_H

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <filesystem>

class JSONBase
{
public:
    bool DeserializeFromFile(const std::string& filePath);
    bool SerializeToFile(const std::string& filePath);

    virtual std::string Serialize() const;
    virtual bool Deserialize(const std::string& s);
    virtual bool Deserialize(const rapidjson::Value& obj) = 0;
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const = 0;

    static std::filesystem::path homeDir;
protected:
    bool InitDocument(const std::string & s, rapidjson::Document &doc);
};

#endif // JSONBASE_H
