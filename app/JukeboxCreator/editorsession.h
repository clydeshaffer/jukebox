#ifndef EDITORSESSION_H
#define EDITORSESSION_H

#include <filesystem>
#include "jsonbase.h"

class EditorSession : public JSONBase
{
public:
    EditorSession();
    virtual bool Deserialize(const rapidjson::Value& obj);
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;

    bool had_project_open;
    std::filesystem::path lastOpenProjectFolder;
};

#endif // EDITORSESSION_H
