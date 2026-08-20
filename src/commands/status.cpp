#include "forge/commands/status.hpp"
#include "forge/core/project_detector.hpp"
#include "forge/core/process_runner.hpp"

#include <iostream>
#include <filesystem>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

namespace forge {

namespace {

void enableConsoleEncoding() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

std::string getGitBranch() {
    ProcessResult res = ProcessRunner::run("git rev-parse --abbrev-ref HEAD");
    if (res.exitCode == 0 && !res.stdOut.empty()) {
        std::string branch = res.stdOut;
        branch.erase(branch.find_last_not_of(" \n\r\t") + 1);
        return branch;
    }
    return "N/A (Not a Git repository)";
}

bool isGitClean() {
    ProcessResult res = ProcessRunner::run("git status --porcelain");
    return res.exitCode == 0 && res.stdOut.empty();
}

} // anonymous namespace

int runStatus(const std::vector<std::string>& args) {
    (void)args; // Reserved for optional --verbose flags
    enableConsoleEncoding();

    std::cout << "\nForge Workspace Status\n";
    std::cout << "--------------------------------------------\n\n";

    ProjectInfo project = detectProject();
    std::string currentDir = fs::current_path().filename().string();

    std::cout << "  Workspace Name: " << currentDir << "\n";
    std::cout << "  Project Type:   " << (project.type.empty() ? "Unknown / Uninitialized" : project.type) << "\n";

    // Manifest Status
    if (fs::exists("CMakeLists.txt")) {
        std::cout << "  Manifest:       CMakeLists.txt [Detected]\n";
    } else if (fs::exists("package.json")) {
        std::cout << "  Manifest:       package.json [Detected]\n";
    } else {
        std::cout << "  Manifest:       None [Missing]\n";
    }

    // Git Repository Metadata
    std::string branch = getGitBranch();
    std::cout << "  Git Branch:     " << branch << "\n";
    if (branch.rfind("N/A", 0) != 0) {
        std::cout << "  Git Status:     " << (isGitClean() ? "Clean (No uncommitted changes)" : "Dirty (Uncommitted changes present)") << "\n";
    }

    // Build Folder / Artifact Checks
    std::cout << "  Build Cache:    ";
    if (fs::exists("build")) {
        std::cout << "Present ('./build')\n";
    } else if (fs::exists("dist") || fs::exists("out") || fs::exists("bin")) {
        std::cout << "Present (Distribution/Output folder)\n";
    } else {
        std::cout << "Missing (Not generated yet)\n";
    }

    // Health Summary
    std::cout << "\n  Health Status:  ";
    if (project.issues.empty()) {
        std::cout << "[OK] Healthy\n\n";
    } else {
        std::cout << "[" << project.issues.size() << " Issue(s) Detected - Run 'forge doctor']\n\n";
    }

    return 0;
}

} // namespace forge