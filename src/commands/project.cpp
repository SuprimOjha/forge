#include "forge/commands/project.hpp"
#include "forge/core/project_detector.hpp"

#include <iostream>

namespace forge {

namespace {

void printIssue(
    const ProjectIssue& issue
) {

    if (issue.severity ==
        ProjectIssue::Severity::Error) {

        std::cout
            << "  [ERROR] "
            << issue.message
            << "\n";

    } else {

        std::cout
            << "  [WARNING] "
            << issue.message
            << "\n";
    }

    if (!issue.suggestion.empty()) {

        std::cout
            << "  Suggestion: "
            << issue.suggestion
            << "\n";
    }
}

}


int runProject() {

    const ProjectInfo project =
        detectProject();

    std::cout << "\n";

    std::cout
        << "Forge Project\n";

    std::cout
        << "--------------------------------------------\n\n";


    /*
     * Basic project information
     */

    std::cout
        << "Project: "
        << project.name
        << "\n";

    std::cout
        << "Path:    "
        << project.path
        << "\n\n";


    /*
     * Detected files
     */

    std::cout
        << "Detected:\n";

    if (project.gitRepository) {

        std::cout
            << "  [OK] Git repository\n";
    }

    for (const auto& file :
         project.detectedFiles) {

        if (file == ".git") {
            continue;
        }

        std::cout
            << "  [OK] "
            << file
            << "\n";
    }


    /*
     * Git
     */

    if (project.gitRepository) {

        std::cout
            << "\nGit:\n";

        std::cout
            << "  [OK] Repository detected\n";

        std::cout
            << "  Root: "
            << project.gitRoot
            << "\n";
    }


    /*
     * Project type
     */

    std::cout
        << "\nProject Type:\n";

    std::cout
        << "  "
        << project.type
        << "\n";


    /*
     * Package manager
     */

    if (!project.packageManager.empty()) {

        std::cout
            << "\nPackage Manager:\n";

        std::cout
            << "  "
            << project.packageManager
            << "\n";
    }


    /*
     * Frameworks
     */

    if (!project.frameworks.empty()) {

        std::cout
            << "\nFrameworks:\n";

        for (const auto& framework :
             project.frameworks) {

            std::cout
                << "  [OK] "
                << framework
                << "\n";
        }
    }


/*
 * Dependencies
 */

if (!project.dependencies.empty()) {

    std::cout
        << "\nDependencies:\n";

    for (const auto& dependency :
         project.dependencies) {

        std::cout
            << "  ";

        if (dependency.status.rfind(
                "WARNING:",
                0
            ) == 0) {

            std::cout << "[WARNING] ";

        } else if (
            dependency.status.rfind(
                "ERROR:",
                0
            ) == 0
        ) {

            std::cout << "[ERROR] ";

        } else {

            std::cout << "[OK] ";
        }

        std::cout
            << dependency.name
            << " "
            << dependency.version;

        if (dependency.development) {

            std::cout
                << " (dev)";
        }

        std::cout << "\n";

        /*
         * Display dependency warning/error
         */

        if (dependency.status.rfind(
                "WARNING:",
                0
            ) == 0 ||
            dependency.status.rfind(
                "ERROR:",
                0
            ) == 0) {

            std::cout
                << "    "
                << dependency.status
                << "\n";
        }
    }
}

    /*
     * Dependency Health
     */

    if (project.type == "Node.js" ||
        project.type == "TypeScript") {

        std::cout
            << "\nDependency Health:\n";

        if (!project.nodeModulesExists) {

            std::cout
                << "  [ERROR] node_modules not found\n";

            std::cout
                << "  Suggestion: run "
                << project.packageManager
                << " install\n";

        } else {

            std::cout
                << "  [OK] node_modules found\n";

            std::cout
                << "  Installed: "
                << project.installedDependencies
                << "\n";

            if (project.missingDependencies > 0) {

                std::cout
                    << "  [ERROR] Missing: "
                    << project.missingDependencies
                    << "\n";

                std::cout
                    << "  Suggestion: run "
                    << project.packageManager
                    << " install\n";

            } else {

                std::cout
                    << "  [OK] All detected dependencies installed\n";
            }
        }
    }


    /*
     * Scripts
     */

    if (!project.scripts.empty()) {

        std::cout
            << "\nScripts:\n";

        for (const auto& script :
             project.scripts) {

            std::cout
                << "  [OK] "
                << script.name
                << " -> "
                << script.command
                << "\n";
        }
    }


    /*
     * Environment
     */

    std::cout
        << "\nEnvironment:\n";

    if (project.type == "Node.js" ||
        project.type == "TypeScript") {

        if (project.nodeAvailable) {

            std::cout
                << "  [OK] Node.js detected\n";

        } else {

            std::cout
                << "  [ERROR] Node.js not found\n";
        }


        if (project.packageManager == "npm") {

            if (project.npmAvailable) {

                std::cout
                    << "  [OK] npm detected\n";

            } else {

                std::cout
                    << "  [ERROR] npm not found\n";
            }
        }
    }


    /*
     * Node.js requirement
     */

    if (!project.requiredNodeVersion.empty()) {

        std::cout
            << "\nRequirements:\n";

        std::cout
            << "  Node.js "
            << project.requiredNodeVersion
            << "\n";
    }
   /*
 * Recommendations
 */

if (!project.recommendations.empty()) {

    std::cout
        << "\nRecommendations:\n";

    for (const auto& recommendation :
         project.recommendations) {

        std::cout
            << "  [!] "
            << recommendation
            << "\n";
    }
}

    /*
     * Health
     */

    int healthChecks = 0;
    int passedChecks = 0;


    std::cout
        << "\nHealth:\n";


    /*
     * Dependencies
     */

    if (project.type == "Node.js" ||
        project.type == "TypeScript") {

        healthChecks++;

        if (!project.dependencies.empty()) {

            passedChecks++;

            std::cout
                << "  [OK] Dependencies detected\n";

        } else {

            std::cout
                << "  [ERROR] No dependencies detected\n";
        }
    }


    /*
     * Package manager
     */

    if (project.type == "Node.js" ||
        project.type == "TypeScript") {

        healthChecks++;

        if (!project.packageManager.empty()) {

            passedChecks++;

            std::cout
                << "  [OK] Package manager detected\n";

        } else {

            std::cout
                << "  [ERROR] Package manager not detected\n";
        }
    }


    /*
     * Node.js
     */

    if (project.type == "Node.js" ||
        project.type == "TypeScript") {

        healthChecks++;

        if (project.nodeAvailable) {

            passedChecks++;

            std::cout
                << "  [OK] Node.js available\n";

        } else {

            std::cout
                << "  [ERROR] Node.js unavailable\n";
        }
    }


    /*
     * npm
     */

    if (project.packageManager == "npm") {

        healthChecks++;

        if (project.npmAvailable) {

            passedChecks++;

            std::cout
                << "  [OK] npm available\n";

        } else {

            std::cout
                << "  [ERROR] npm unavailable\n";
        }
    }


    /*
     * node_modules
     */

    if (project.type == "Node.js" ||
        project.type == "TypeScript") {

        healthChecks++;

        if (project.nodeModulesExists) {

            passedChecks++;

            std::cout
                << "  [OK] Dependencies installed\n";

        } else {

            std::cout
                << "  [ERROR] Dependencies not installed\n";
        }
    }


    /*
     * Git
     */

    healthChecks++;

    if (project.gitRepository) {

        passedChecks++;

        std::cout
            << "  [OK] Git repository detected\n";

    } else {

        std::cout
            << "  [WARNING] Git repository not detected\n";
    }


    /*
     * Issues
     */

    if (!project.issues.empty()) {

        std::cout
            << "\nIssues:\n";

        for (const auto& issue :
             project.issues) {

            printIssue(issue);
        }
    }


    /*
     * Status
     */

    std::cout
        << "\nStatus:\n";

    if (healthChecks == 0) {

        std::cout
            << "  [WARNING] No health checks available\n";

    } else if (passedChecks == healthChecks) {

        std::cout
            << "  [OK] Project looks healthy\n";

    } else {

        std::cout
            << "  [WARNING] Project has "
            << (healthChecks - passedChecks)
            << " issue(s)\n";
    }


    /*
     * Health score
     */

    if (healthChecks > 0) {

        const int healthPercentage =
            (passedChecks * 100) /
            healthChecks;

        std::cout
            << "  Score: "
            << healthPercentage
            << "%\n";
    }


    std::cout
        << "\n";

    return 0;
}

}