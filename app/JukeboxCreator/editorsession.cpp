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
}

bool EditorSession::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
    writer->StartObject();
    JSON_EMIT(Bool, had_project_open);
    JSON_EMIT_PATH(lastOpenProjectFolder);
    writer->EndObject();
}
