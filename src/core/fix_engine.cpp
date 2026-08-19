#include "forge/core/fix_engine.hpp"

namespace forge {

std::vector<FixAction> FixEngine::resolveFixes(const ProjectInfo& project) {
    std::vector<FixAction> actions;

    for (const auto& issue : project.issues) {

        if (issue.message == "node_modules not found" && project.packageManager == "npm") {
            FixAction action;
            action.id = "npm.install_missing";
            action.title = "Install Node.js dependencies";
            action.description = "Runs `npm install` to populate node_modules based on package.json.";
            action.command = "npm install";
            action.risk = RiskLevel::Safe;

            actions.push_back(action);
        }

        if (issue.message == "CMake build directory not configured") {
            FixAction action;
            action.id = "cmake.configure";
            action.title = "Configure CMake build directory";
            action.description = "Runs `cmake -B build` to generate the CMake build cache and build files.";
            action.command = "cmake -B build";
            action.risk = RiskLevel::Safe;

            actions.push_back(action);
        }
    }

    return actions;
}

bool FixEngine::executeFix(const FixAction& fix, const std::string& projectPath, ProcessResult& resultOut) {
    resultOut = ProcessRunner::run(fix.command, projectPath);
    return resultOut.success;
}

bool FixEngine::verifyFix(const FixAction& fix, const ProjectInfo& updatedProject) {
    if (fix.id == "npm.install_missing") {
        return updatedProject.nodeModulesExists;
    }

    if (fix.id == "cmake.configure") {
        return updatedProject.cmakeConfigured;
    }

    return true;
}

} // namespace forge