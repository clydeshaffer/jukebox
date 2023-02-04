#ifndef ASSEMBLYUNIT_H
#define ASSEMBLYUNIT_H

#include <string>
#include <filesystem>

class AssemblyUnit
{
public:

    bool Compile(std::filesystem::path root);

    virtual std::string GenName() = 0;
    virtual std::string GetCode(std::filesystem::path root) = 0;
};

#endif // ASSEMBLYUNIT_H
