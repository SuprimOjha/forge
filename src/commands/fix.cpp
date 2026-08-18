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

    FixAction action;

    action.title =
        "Install Node.js dependencies";

    action.description =
        "Install dependencies declared in package.json.";

    action.command =
        "npm install";

    std::cout
        << "Fix available:\n";

    std::cout
        << "  [1] "
        << action.title
        << "\n";

    std::cout
        << "      "
        << action.description
        << "\n";

    std::cout
        << "      Command: "
        << action.command
        << "\n\n";

    std::cout
        << "Run this fix now? [y/N]: ";

    char answer;
    std::cin >> answer;

    if (answer == 'y' ||
        answer == 'Y') {

        std::cout
            << "\nRunning: "
            << action.command
            << "\n\n";

        const int result =
            std::system(
                action.command.c_str()
            );

        if (result == 0) {

            std::cout
                << "\n[OK] "
                << action.title
                << " completed successfully.\n";

        } else {

            std::cout
                << "\n[ERROR] "
                << action.title
                << " failed.\n";

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