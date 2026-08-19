#include "forge/commands/project.hpp"
#include "forge/core/project_detector.hpp"

#include <iostream>

namespace forge {

int runProject() {
    ProjectInfo project = detectProject();

    std::cout << "\nForge Project\n";
    std::cout << "--------------------------------------------\n\n";

    std::cout << "Project: " << project.name << "\n";
    std::cout << "Path:    " << project.path << "\n\n";

    std::cout << "Detected:\n";
    for (const auto& file : project.detectedFiles) {
        std::cout << "  [OK] " << file << "\n";
    }
    std::cout << "\n";

    if (project.gitRepository) {
        std::cout << "Git:\n";
        std::cout << "  [OK] Repository detected\n";
        std::cout << "  Root: " << project.gitRoot << "\n\n";
    }

    std::cout << "Project Type:\n";
    std::cout << "  " << project.type << "\n\n";

    if (project.type == "C++ / CMake") {
        std::cout << "CMake Metadata:\n";
        if (!project.cxxStandard.empty()) {
            std::cout << "  Standard: C++" << project.cxxStandard << "\n";
        }
        if (!project.cmakeMinimumVersion.empty()) {
            std::cout << "  Minimum Version: CMake " << project.cmakeMinimumVersion << "\n";
        }
        if (project.cmakeConfigured) {
            std::cout << "  Status: Configured\n";
            if (!project.cmakeGenerator.empty()) {
                std::cout << "  Generator: " << project.cmakeGenerator << "\n";
            }
        } else {
            std::cout << "  Status: Not Configured (missing build/ directory or CMakeCache.txt)\n";
        }
        std::cout << "\n";
    }

    if (!project.frameworks.empty()) {
        std::cout << "Frameworks:\n";
        for (const auto& fw : project.frameworks) {
            std::cout << "  - " << fw << "\n";
        }
        std::cout << "\n";
    }

    std::cout << "Environment:\n";
    if (project.type == "C++ / CMake") {
        std::cout << "  CMake: " << (project.cmakeAvailable ? "[OK] Available" : "[ERROR] Missing") << "\n";
    }
    if (project.type == "Node.js" || project.type == "TypeScript") {
        std::cout << "  Node.js: " << (project.nodeAvailable ? "[OK] Available" : "[ERROR] Missing") << "\n";
        std::cout << "  npm:     " << (project.npmAvailable ? "[OK] Available" : "[ERROR] Missing") << "\n";
    }
    std::cout << "\n";

    if (!project.recommendations.empty()) {
        std::cout << "Recommendations:\n";
        for (const auto& rec : project.recommendations) {
            std::cout << "  [!] " << rec << "\n";
        }
        std::cout << "\n";
    }

    std::cout << "Health:\n";
    if (project.issues.empty()) {
        std::cout << "  [OK] Project is fully configured and healthy\n";
    } else {
        for (const auto& issue : project.issues) {
            if (issue.severity == ProjectIssue::Severity::Error) {
                std::cout << "  [ERROR] " << issue.message << "\n";
            } else {
                std::cout << "  [WARNING] " << issue.message << "\n";
            }
        }
    }
    std::cout << "\n";

    return 0;
}

} // namespace forge