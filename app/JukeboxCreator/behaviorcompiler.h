#ifndef BEHAVIORCOMPILER_H
#define BEHAVIORCOMPILER_H

#include <filesystem>
#include <map>

class BehaviorCompiler
{
public:
    BehaviorCompiler();

    static void PrepareToBuild();

    static bool RunAssembler(std::filesystem::path asmFile);

    static bool RunLinker();

    static std::map<std::string, int> ParseMap();

    static std::filesystem::path GetBinPath();
};

#endif // BEHAVIORCOMPILER_H
