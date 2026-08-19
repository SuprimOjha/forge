#include "forge/commands/fix.hpp"
#include "forge/core/project_detector.hpp"
#include "forge/core/fix_engine.hpp"

#include <iostream>

namespace forge {

int runFix() {
    ProjectInfo project = detectProject();

    std::cout << "\nForge Fix\n";
    std::cout << "--------------------------------------------\n\n";
    std::cout << "Project: " << project.name << "\n\n";

    if (project.issues.empty()) {
        std::cout << "[OK] No fixes required\n";
        std::cout << "Project is healthy.\n\n";
        return 0;
    }

    std::cout << "Issues detected:\n\n";
    for (const auto& issue : project.issues) {
        if (issue.severity == ProjectIssue::Severity::Error) {
            std::cout << "[ERROR] " << issue.message << "\n";
        } else {
            std::cout << "[WARNING] " << issue.message << "\n";
        }

        if (!issue.suggestion.empty()) {
            std::cout << "  Suggestion: " << issue.suggestion << "\n";
        }
        std::cout << "\n";
    }

    std::vector<FixAction> availableFixes = FixEngine::resolveFixes(project);

    if (availableFixes.empty()) {
        std::cout << "[INFO] No automated fixes available for detected issues.\n\n";
        return 0;
    }

    std::cout << "Available Fixes:\n";
    for (size_t i = 0; i < availableFixes.size(); ++i) {
        const auto& fix = availableFixes[i];
        std::cout << "  [" << (i + 1) << "] " << fix.title << "\n";
        std::cout << "      " << fix.description << "\n";
        std::cout << "      Command: " << fix.command << "\n\n";
    }

    std::cout << "Run fix [1] now? [y/N]: ";
    char answer;
    std::cin >> answer;

    if (answer == 'y' || answer == 'Y') {
        const FixAction& selectedFix = availableFixes[0];
        std::cout << "\nExecuting: " << selectedFix.command << "...\n\n";

        ProcessResult result;
        bool status = FixEngine::executeFix(selectedFix, project.path, result);

        if (!result.stdOut.empty()) {
            std::cout << result.stdOut << "\n";
        }

        if (status) {
            std::cout << "[OK] Command executed successfully. Verifying...\n";
            ProjectInfo recheckedProject = detectProject();

            if (FixEngine::verifyFix(selectedFix, recheckedProject)) {
                std::cout << "[OK] Fix verified: Issue successfully resolved.\n\n";
            } else {
                std::cout << "[WARNING] Command succeeded, but verification failed.\n\n";
            }
        } else {
            std::cout << "[ERROR] Fix failed with exit code " << result.exitCode << "\n";
            if (!result.stdErr.empty()) {
                std::cout << "Error Output:\n" << result.stdErr << "\n";
            }
            return 1;
        }
    } else {
        std::cout << "\n[INFO] No changes made.\n\n";
    }

    return 0;
}

} // namespace forge