#include <fstream>
#include <string>
#include "gtbehavior.h"
#include "jsonmacros.h"

using namespace std;
using namespace filesystem;

map<string, int> parseParamsSection(const string& input) {
    map<string, int> result;
    stringstream ss(input);
    string line;
    while (getline(ss, line)) {
        size_t pos = line.find("=");
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            int value = stoi(line.substr(pos + 1));
            result[key] = value;
        }
    }
    return result;
}

string readParamsSection(const filesystem::path& filePath) {
    string fileContent,sectionContent;
    bool found_start=false;
    ifstream fileStream(filePath);
    if (fileStream.is_open()) {
        string line;
        while (getline(fileStream, line)) {
            if(line==";PARAMS START")
            {
                found_start=true;
                continue;
            }
            if(line==";PARAMS END")
            {
                break;
            }
            if(found_start)
            {
                sectionContent+=line+'\n';
            }
        }
        fileStream.close();
    }
    return sectionContent;
}

GTBehavior::GTBehavior()
{

}

GTBehavior::GTBehavior(std::string name, std::filesystem::path source) : name(name), source(source)
{

}

bool GTBehavior::Deserialize(const rapidjson::Value& obj)
{
    JSON_READ(String, name);
    JSON_READ_PATH(source);

    string paramsSection = readParamsSection(homeDir / source);
    params = parseParamsSection(paramsSection);

    for(auto& member : obj["params"].GetObject()) {
        params[member.name.GetString()] = member.value.GetInt();
    }
    return true;
}

bool GTBehavior::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
    writer->StartObject();
    JSON_EMIT_STRING(name);
    JSON_EMIT_PATH(source);

    writer->String("params");
    writer->StartObject();
    for(auto& paramPair : params) {
        writer->String(paramPair.first.c_str());
        writer->Int(paramPair.second);
    }
    writer->EndObject();

    writer->EndObject();
    return true;
}

std::string GTBehavior::GetCode(path root)
{
    std::string fileContent;
    std::ifstream fileStream(root / source);
    if (fileStream.is_open()) {
        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        fileContent = buffer.str();
        fileStream.close();
    }
    return fileContent;
}
