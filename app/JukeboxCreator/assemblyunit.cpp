#include <fstream>
#include "assemblyunit.h"
#include "behaviorcompiler.h"

using namespace std;
using namespace filesystem;

bool AssemblyUnit::Compile(std::filesystem::path root)
{
    path genFile = root / path("build") / path(GenName() + ".asm");
    std::ofstream genCode(genFile);
    std::string code = GetCode(root);
    genCode << code;
    genCode.close();
    return BehaviorCompiler::RunAssembler(genFile);
}
