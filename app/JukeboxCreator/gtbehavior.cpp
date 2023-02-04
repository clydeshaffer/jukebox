#include <fstream>
#include <string>
#include "gtproject.h"
#include "gtbehavior.h"
#include "jsonmacros.h"
#include <regex>
#include "behaviorcompiler.h"
#include "core_symbols.h"

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

std::string remove_params(const std::string &str) {
    std::string start_regex_str = ";PARAMS START";
    std::string end_regex_str = ";PARAMS END";
    std::regex query(start_regex_str + ".*" + end_regex_str, std::regex::extended);
    return std::regex_replace(str, query, "");
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
    if(GTProject::loadedProject != nullptr) {
        string paramsSection = readParamsSection(GTProject::loadedProject->projectRoot / source);
        params = parseParamsSection(paramsSection);
    }
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

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::string GTBehavior::GenName()
{
    std::string genName = name;

    for(auto& param : params) {
        genName += "_" + std::to_string(param.second);
    }
    return genName;
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

    std::string genName = GenName();
    std::string genParams = "";

    for(auto& param : params) {
        genParams += param.first + " = " + std::to_string(param.second) + "\r\n";
    }

    replaceAll(fileContent, "UPDATE_START", genName);

    fileContent = genParams + remove_params(fileContent);

    return fileContent;
}
