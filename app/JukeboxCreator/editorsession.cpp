#include "editorsession.h"
#include "jsonmacros.h"

using namespace std;
using namespace filesystem;

EditorSession::EditorSession()
{
    had_project_open = false;
}

bool EditorSession::Deserialize(const rapidjson::Value& obj)
{
    JSON_READ(Bool, had_project_open);
    JSON_READ_PATH(lastOpenProjectFolder);
    const auto& projArr = obj["recentProjectPaths"].GetArray();
    for(auto& elem : projArr) {
        recentProjectPaths.push_back(std::filesystem::path(elem.GetString()));
    }
    return true;
}

bool EditorSession::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
    writer->StartObject();
    JSON_EMIT(Bool, had_project_open);
    JSON_EMIT_PATH(lastOpenProjectFolder);

    writer->String("recentProjectPaths");
    writer->StartArray();
    for(auto& p : recentProjectPaths) {
        writer->String(p.string().c_str());
    }
    writer->EndArray();

    writer->EndObject();
    return true;
}

void EditorSession::UpdateRecentProjects(std::filesystem::path opened) {
    lastOpenProjectFolder = opened;
    bool found = false;
    for (auto it = recentProjectPaths.begin(), lim = recentProjectPaths.end();
         it != lim;
         ++it) {
      if ((*it) == lastOpenProjectFolder) {
        std::rotate(recentProjectPaths.begin(), it, it + 1);
        found = true;
        break;
      }
    }
    if(!found) {
        recentProjectPaths.insert(recentProjectPaths.begin(), lastOpenProjectFolder);
    }
}
