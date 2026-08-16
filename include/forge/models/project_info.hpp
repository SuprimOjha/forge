#pragma once

#include <string>
#include <vector>

namespace forge {

struct DependencyInfo {

    std::string name;
    std::string version;
    bool development = false;
};

struct ProjectInfo {

    std::string name;
    std::string path;
    std::string type;
    std::string gitRoot;
    std::string packageManager;
    std::string requiredNodeVersion;

    bool nodeAvailable = false;
    bool npmAvailable = false;

    bool nodeModulesExists = false;

    int installedDependencies = 0;
    int missingDependencies = 0;

    std::vector<std::string> detectedFiles;
    std::vector<std::string> frameworks;
    std::vector<DependencyInfo> dependencies;

    bool gitRepository = false;
};

}