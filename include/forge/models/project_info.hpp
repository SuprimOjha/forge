#pragma once

#include <string>
#include <vector>

namespace forge {

struct DependencyInfo {

    std::string name;
    std::string version;
    bool development = false;

    std::string status;
};


struct ScriptInfo {

    std::string name;
    std::string command;
};

struct ProjectIssue {
    enum class Severity {
        Warning,
        Error
    };

    Severity severity;

    std::string message;
    std::string suggestion;
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

    std::vector<ProjectIssue> issues;

    bool gitRepository = false;
};

}