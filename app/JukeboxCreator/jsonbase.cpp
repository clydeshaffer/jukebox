#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <string>
#include "jsonbase.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

std::filesystem::path JSONBase::homeDir;

std::string JSONBase::NormalizePathString(std::string rawPath)
{
    std::string s = rawPath;
    #if !((defined (_WIN32) || defined (_WIN64)))
    std::replace(s.begin(), s.end(), '\\', '/');
    #endif
    return s;
}

std::string JSONBase::Serialize() const
{
    rapidjson::StringBuffer ss;
    rapidjson::Writer<rapidjson::StringBuffer> writer(ss);		// Can also use Writer for condensed formatting
    if (Serialize(&writer))
        return ss.GetString();
    return "";
}

bool JSONBase::Deserialize(const std::string& s)
{
    rapidjson::Document doc;
    if (!InitDocument(s, doc))
        return false;

    Deserialize(doc);

    return true;
}

bool JSONBase::DeserializeFromFile(const std::string& filePath)
{
    std::ifstream f(filePath);
    std::stringstream buffer;
    buffer << f.rdbuf();
    f.close();

    return Deserialize(buffer.str());
}

bool JSONBase::SerializeToFile(const std::string& filePath)
{
    std::ofstream f(filePath);
    std::string s = Serialize();
    f << s;
    f.flush();
    f.close();

    return true;
}

bool JSONBase::InitDocument(const std::string& s, rapidjson::Document& doc)
{
    if (s.empty())
        return false;

    std::string validJson(s);

    return !doc.Parse(validJson.c_str()).HasParseError() ? true : false;
}
