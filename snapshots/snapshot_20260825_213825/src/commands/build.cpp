#include "forge/commands/build.hpp"
#include "forge/core/project_detector.hpp"
#include "forge/core/process_runner.hpp"

#include <iostream>
#include <algorithm>

namespace forge {

namespace {

bool askUserConfirmation(const std::string& prompt) {
    std::cout << "  [?] " << prompt << " (y/N): ";
    char response = 'n';
    std::cin >> response;
    return (response == 'y' || response == 'Y');
}

std::string formatCommand(const std::string& base, const std::vector<std::string>& extraArgs) {
    std::string cmd = base;
    for (const auto& arg : extraArgs) {
        cmd += " " + arg;
    }
    return cmd;
}

}

int runBuild(const std::vector<std::string>& extraArgs) {
    const ProjectInfo project = detectProject();

    if (project.type == "C++ / CMake") {
        if (!project.cmakeConfigured) {
            std::cout << "\n  [ERROR] CMake build cache is missing in './build'.\n";
            if (!askUserConfirmation("Would you like Forge to run 'cmake -B build' automatically?")) {
                std::cout << "  [!] Build aborted.\n\n";
                return 1;
            }

            const ProcessResult configRes = ProcessRunner::run("cmake -B build");
            if (configRes.exitCode != 0) {
                std::cout << "  [ERROR] CMake configuration failed.\n\n";
                return configRes.exitCode;
            }
        }

        const std::string cmd = formatCommand("cmake --build build", extraArgs);
        std::cout << "  [>] Executing: " << cmd << "\n\n";
        
        const ProcessResult res = ProcessRunner::run(cmd);
        if (res.exitCode != 0) {
            std::cout << "\n  [ERROR] Build failed with exit code " << res.exitCode << "\n";
            if (askUserConfirmation("Do you want to run 'forge fix' to repair your workspace?")) {
                return ProcessRunner::run("forge fix").exitCode;
            }
        }
        return res.exitCode;
    }
    else if (project.type == "Node.js" || project.type == "TypeScript") {
        const bool hasBuild = std::any_of(
            project.scripts.begin(), 
            project.scripts.end(),
            [](const auto& s) { return s.name == "build"; }
        );

        if (!hasBuild) {
            std::cout << "\n  [ERROR] No 'build' script defined in package.json.\n\n";
            return 1;
        }

#ifdef _WIN32
        const std::string baseCmd = "cmd /c \"set CI=true && " + project.packageManager + " run build\"";
#else
        const std::string baseCmd = "CI=true " + project.packageManager + " run build";
#endif

        const std::string cmd = formatCommand(baseCmd, extraArgs);
        std::cout << "  [>] Executing: " << cmd << "\n\n";

        return ProcessRunner::run(cmd).exitCode;
    }

    std::cout << "  [ERROR] Unsupported project context: " << project.type << "\n\n";
    return 1;
}

}