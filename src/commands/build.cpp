#include "forge/commands/build.hpp"
#include "forge/core/project_detector.hpp"
#include "forge/core/process_runner.hpp"

#include <iostream>

namespace forge {

int runBuild(
    const std::vector<std::string>& extraArgs
) {

    const ProjectInfo project =
        detectProject();

    std::cout << "\n";

    std::cout
        << "Forge Build\n";

    std::cout
        << "--------------------------------------------\n\n";


    /*
     * C++ / CMake projects
     */

    if (project.type == "C++ / CMake") {

        if (!project.cmakeConfigured) {

            std::cout
                << "  [ERROR] CMake build directory is not configured\n";

            std::cout
                << "  Suggestion: Run 'forge fix' or 'cmake -B build' first\n\n";

            return 1;
        }

        std::string fullCommand =
            "cmake --build build";

        for (const auto& arg :
             extraArgs) {

            fullCommand += " " + arg;
        }

        std::cout
            << "  [INFO] Running: "
            << fullCommand
            << "\n\n";

        const ProcessResult result =
            ProcessRunner::run(fullCommand);

        if (result.exitCode == 0) {

            std::cout
                << "\n  [OK] Build succeeded\n\n";

            return 0;

        } else {

            std::cout
                << "\n  [ERROR] Build failed with exit code "
                << result.exitCode
                << "\n\n";

            return result.exitCode;
        }

    }


    /*
     * Node.js / TypeScript projects
     */

    else if (project.type == "Node.js" ||
             project.type == "TypeScript") {

        bool hasBuildScript = false;

        for (const auto& script :
             project.scripts) {

            if (script.name == "build") {
                hasBuildScript = true;
                break;
            }
        }

        if (!hasBuildScript) {

            std::cout
                << "  [ERROR] No 'build' script found in package.json\n";

            std::cout
                << "  Suggestion: Add a \"build\" script under \"scripts\" in package.json\n\n";

            return 1;
        }

        std::string fullCommand =
            project.packageManager + " run build";

        for (const auto& arg :
             extraArgs) {

            fullCommand += " " + arg;
        }

        std::cout
            << "  [INFO] Running: "
            << fullCommand
            << "\n\n";

        const ProcessResult result =
            ProcessRunner::run(fullCommand);

        if (result.exitCode == 0) {

            std::cout
                << "\n  [OK] Build succeeded\n\n";

            return 0;

        } else {

            std::cout
                << "\n  [ERROR] Build failed with exit code "
                << result.exitCode
                << "\n\n";

            return result.exitCode;
        }

    }


    /*
     * Unsupported project type
     */

    else {

        std::cout
            << "  [ERROR] Unsupported project type for build: "
            << project.type
            << "\n\n";

        return 1;
    }
}

} // namespace forge