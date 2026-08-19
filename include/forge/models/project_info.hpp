#pragma once

#include <string>
#include <vector>

namespace forge {

struct DependencyInfo {
    std::string name;
    std::string version;
    bool development = false;

    bool warning = false;
    std::string warningMessage;
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

enum class RiskLevel {
    Safe,
    Moderate,
    High,
    Destructive
};

struct FixAction {
    std::string id;
    std::string title;
    std::string description;
    std::string command;
    RiskLevel risk = RiskLevel::Safe;
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

    // CMake / C++ metadata
    bool cmakeAvailable = false;
    bool cmakeConfigured = false;
    std::string cmakeGenerator;
    std::string cxxStandard;
    std::string cmakeMinimumVersion;

    int installedDependencies = 0;
    int missingDependencies = 0;

    std::vector<std::string> detectedFiles;
    std::vector<std::string> frameworks;
    std::vector<DependencyInfo> dependencies;
    std::vector<ScriptInfo> scripts;

    std::vector<ProjectIssue> issues;
    std::vector<std::string> recommendations;
    bool gitRepository = false;
};

} // namespace forge