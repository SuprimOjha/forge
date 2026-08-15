
#include "forge/commands/project.hpp"
#include "forge/core/project_detector.hpp"

#include <iostream>

namespace forge {

int runProject() {

    const ProjectInfo project = detectProject();

    std::cout << "\n";
    std::cout << "Forge Project\n";
    std::cout << "────────────────────────────\n\n";

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
        std::cout << "✓ Git repository\n";
    }

    for (const auto& file : project.detectedFiles) {

        if (file == ".git") {
            continue;
        }

        std::cout
            << "✓ "
            << file
            << "\n";
    }


    /*
     * Project type
     */

    std::cout << "\nProject Type:\n";

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
                << "  ✓ "
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
                << "  ✓ "
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


    std::cout << "\n";

    return 0;
}

}