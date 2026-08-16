#pragma once

#include <string>
#include <vector>

namespace forge {

struct DependencyInfo {

    std::string name;
    std::string version;
    bool development = false;
};

struct ScriptInfo {

    std::string name;
    std::string command;
};

struct ProjectInfo {

    std::string name;
    std::string path;
    std::string type;

    std::string packageManager;
    std::string requiredNodeVersion;
    std::string gitRoot;

    bool nodeAvailable = false;
    bool npmAvailable = false;
    bool nodeModulesExists = false;

    int installedDependencies = 0;
    int missingDependencies = 0;

    std::vector<std::string> detectedFiles;
    std::vector<std::string> frameworks;
    std::vector<DependencyInfo> dependencies;
    std::vector<ScriptInfo> scripts;

    bool gitRepository = false;
};

}