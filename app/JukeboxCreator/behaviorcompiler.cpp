#include "behaviorcompiler.h"

#include <core_symbols.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdint>
#include <QDebug>
#include <cstdlib>
#include <QProcess>

#include "whereami.h"
#include "gtproject.h"

std::filesystem::path get_assembler_path() {
    int length = wai_getExecutablePath(nullptr, 0, nullptr);
    char* exePath = (char*)malloc(length + 1);
    wai_getExecutablePath(exePath, length, nullptr);
    exePath[length] = '\0';
    return std::filesystem::path(exePath).parent_path() / std::filesystem::path("lib/ca65.exe");
}

std::filesystem::path get_linker_path() {
    int length = wai_getExecutablePath(nullptr, 0, nullptr);
    char* exePath = (char*)malloc(length + 1);
    wai_getExecutablePath(exePath, length, nullptr);
    exePath[length] = '\0';
    return std::filesystem::path(exePath).parent_path() / std::filesystem::path("lib/ld65.exe");
}



bool run_process(const std::filesystem::path &executable, const std::vector<std::string> &args) {
    QStringList qargs;
    for (const auto &arg : args) {
        qargs.push_back(arg.c_str());
    }

    QProcess proc;
    proc.start(executable.string().c_str(), qargs);
    if(!proc.waitForStarted()) {
        return false;
    }

    if(!proc.waitForFinished()) {
        return false;
    }

    qDebug() << proc.readAllStandardError();

    return !proc.exitCode();
}

BehaviorCompiler::BehaviorCompiler()
{
}

void BehaviorCompiler::PrepareToBuild()
{
    int length = wai_getExecutablePath(nullptr, 0, nullptr);
    char* exePath = (char*)malloc(length + 1);
    wai_getExecutablePath(exePath, length, nullptr);
    exePath[length] = '\0';
    std::filesystem::path buildPath = GTProject::loadedProject->projectRoot / path("build");
    if(std::filesystem::exists(buildPath)) {
        std::filesystem::remove_all(buildPath);
        std::filesystem::create_directory(buildPath);
    }
    std::filesystem::path asmInc = std::filesystem::path(exePath).parent_path() / std::filesystem::path("lib/engine.inc");
    std::filesystem::copy_file(asmInc, GTProject::loadedProject->projectRoot / path("build") / asmInc.filename());
}

bool BehaviorCompiler::RunAssembler(std::filesystem::path asmFile)
{
    std::vector<std::string> args;
    args.push_back(asmFile.string());
    args.push_back("-o");
    args.push_back((GTProject::loadedProject->projectRoot / path("build") / asmFile.filename().replace_extension(".o")).string());
    return run_process(get_assembler_path(), args);
}

std::filesystem::path BehaviorCompiler::GetBinPath()
{
    return (GTProject::loadedProject->projectRoot / path("build/behaviors.bank"));
}

bool BehaviorCompiler::RunLinker()
{
    int length = wai_getExecutablePath(nullptr, 0, nullptr);
    char* exePath = (char*)malloc(length + 1);
    wai_getExecutablePath(exePath, length, nullptr);
    exePath[length] = '\0';

    std::vector<std::string> args;
    args.push_back((GTProject::loadedProject->projectRoot / path("build") / path("*.o")).string());
    args.push_back("-m");
    args.push_back((GTProject::loadedProject->projectRoot / path("build/out.map")).string());
    args.push_back("-vm");
    args.push_back("-C");
    args.push_back((std::filesystem::path(exePath).parent_path() / std::filesystem::path("resource/behaviors.cfg")).string());
    args.push_back(("-o"));
    args.push_back(GetBinPath().string());
    return run_process(get_linker_path(), args);
}

std::map<std::string, int> BehaviorCompiler::ParseMap()
{
    std::string fileContent;
    std::ifstream fileStream(GTProject::loadedProject->projectRoot / path("build/out.map"));
    if (fileStream.is_open()) {
        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        fileContent = buffer.str();
        fileStream.close();
    }

    std::size_t start = fileContent.find("Exports list by name:");
    std::size_t end = fileContent.find("Exports list by value:");
    if (start != std::string::npos && end != std::string::npos) {
        std::string exportsList = fileContent.substr(start, end - start);
        std::istringstream iss(exportsList);
        std::string line;
        std::vector<std::string> tokens;
        while(iss >> line) {
            tokens.push_back(line);
        }
        auto tokenIter = tokens.begin();
        tokenIter += 5;
        map<string, int> parsedMap;
        while(tokenIter != tokens.end()) {
            std::string expName = *tokenIter;
            tokenIter++;
            std::string expAddr = *tokenIter;
            tokenIter++;
            tokenIter++;

            parsedMap[expName] = std::stoi(expAddr, nullptr, 16);
            qDebug() << expName.c_str() << "at" << expAddr.c_str();
        }
        return parsedMap;
    } else {
        return map<std::string, int>();
    }
}
