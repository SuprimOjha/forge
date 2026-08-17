#include "forge/commands/fix.hpp"
#include "forge/core/project_detector.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace forge {

int runFix() {

    const ProjectInfo project = detectProject();

    std::cout << "\n";
    std::cout << "Forge Fix\n";
    std::cout
        << "--------------------------------------------\n\n";

    std::cout
        << "Project: "
        << project.name
        << "\n\n";

    if (project.issues.empty()) {

        std::cout
            << "[OK] No fixes required\n";

        std::cout
            << "Project is healthy.\n\n";

        return 0;
    }

    std::cout
        << "Issues detected:\n\n";

    for (const auto& issue :
         project.issues) {

        if (issue.severity ==
            ProjectIssue::Severity::Error) {

            std::cout
                << "[ERROR] "
                << issue.message
                << "\n";

        } else {

            std::cout
                << "[WARNING] "
                << issue.message
                << "\n";
        }

        if (!issue.suggestion.empty()) {

            std::cout
                << "  Suggestion: "
                << issue.suggestion
                << "\n";
        }

        std::cout << "\n";
    }

    bool missingNodeModules = false;

    for (const auto& issue :
         project.issues) {

        if (issue.message ==
            "node_modules not found") {

            missingNodeModules = true;
            break;
        }
    }

    if (missingNodeModules &&
        project.packageManager == "npm") {

        std::cout
            << "Fix available:\n";

        std::cout
            << "  [1] Install Node.js dependencies\n";

        std::cout
            << "      Command: npm install\n\n";

        std::cout
            << "Run npm install now? [y/N]: ";

        char answer;
        std::cin >> answer;

        if (answer == 'y' ||
            answer == 'Y') {

            std::cout
                << "\nRunning npm install...\n\n";

            const int result =
                std::system("npm install");

            if (result == 0) {

                std::cout
                    << "\n[OK] Dependencies installed successfully.\n";

            } else {

                std::cout
                    << "\n[ERROR] npm install failed.\n";

                return 1;
            }

        } else {

            std::cout
                << "\n[INFO] No changes made.\n";
        }
    }

    if (!project.gitRepository) {

        std::cout
            << "\nGit:\n";

        std::cout
            << "  [INFO] Git repository not detected.\n";

        std::cout
            << "  Suggestion: git init\n";

        std::cout
            << "  Forge will not initialize Git automatically.\n";
    }

    std::cout << "\n";

    return 0;
}

}