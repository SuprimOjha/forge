#include "forge/commands/project.hpp"
#include "forge/core/project_detector.hpp"

#include <iostream>

namespace forge {

int runProject() {

    const ProjectInfo project = detectProject();

    std::cout << "\n";
    std::cout << "Forge Project\n";
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

    std::cout << "Detected:\n";

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
                << "  [OK] "
                << dependency.name
                << " "
                << dependency.version;

            if (dependency.development) {

                std::cout
                    << " (dev)";
            }

            std::cout << "\n";
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
            << "  Suggestion: run npm install\n";

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
                << "  Suggestion: run npm install\n";

        } else {

            std::cout
                << "  [OK] All detected dependencies installed\n";
        }
    }
}

    /*
     * Environment
     */

    std::cout
        << "\nEnvironment:\n";


    /*
     * Node.js
     */

    if (project.type == "Node.js" ||
        project.type == "TypeScript") {

        if (project.nodeAvailable) {

            std::cout
                << "  [OK] Node.js detected\n";

        } else {

            std::cout
                << "  [ERROR] Node.js not found\n";
        }


        /*
         * npm
         */

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
     * Project Health
     */

    int healthChecks = 0;
    int passedChecks = 0;

    std::cout
        << "\nHealth:\n";


    /*
     * Dependencies check
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
     * Package manager check
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
     * Node.js check
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
     * npm check
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
     * Git check
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
     * Health summary
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


    std::cout << "\n";

    return 0;
}

}
