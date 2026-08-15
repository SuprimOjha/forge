#include "forge/commands/project.hpp"
#include "forge/core/project_detector.hpp"

#include <iostream>

namespace forge {

int runProject() {

    const ProjectInfo project = detectProject();

    std::cout << "\n";
    std::cout << "Forge Project\n";
    std::cout << "────────────────────────────\n\n";

    std::cout << "Project: " << project.name << "\n";
    std::cout << "Path:    " << project.path << "\n\n";

    std::cout << "Detected:\n";

    if (project.gitRepository) {
        std::cout << "✓ Git repository\n";
    }

    for (const auto& file : project.detectedFiles) {

        if (file == ".git") {
            continue;
        }

        std::cout << "✓ " << file << "\n";
    }

    std::cout << "\n";

    std::cout << "Project Type:\n";
    std::cout << "  " << project.type << "\n";

    if (!project.frameworks.empty()) {

        std::cout << "\nFrameworks:\n";

        for (const auto& framework : project.frameworks) {
            std::cout << "  ✓ " << framework << "\n";
        }
    }

    std::cout << "\n";

    return 0;
}

}