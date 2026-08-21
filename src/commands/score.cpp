#include "forge/commands/score.hpp"
#include "forge/core/detector.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>

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

void printScoreHelp() {
    std::cout
        << "\n"
        << "Forge Workspace Health & Readiness Evaluator\n\n"

        << "Usage:\n"
        << "  forge score [options]\n\n"

        << "Options:\n"
        << "  -h, --help              Show this help message\n";
}

std::string getGrade(int score) {
    if (score >= 95) return "A+";
    if (score >= 90) return "A";
    if (score >= 80) return "B";
    if (score >= 70) return "C";
    if (score >= 60) return "D";
    return "F";
}

} // anonymous namespace

int runScore(int argc, char* argv[]) {
    enableConsoleEncoding();

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printScoreHelp();
            return 0;
        }
    }

    std::cout << "\nForge Repository Health Score\n";
    std::cout << "--------------------------------------------\n\n";

    int score = 0;
    int maxScore = 100;
    std::vector<std::string> passed;
    std::vector<std::string> warnings;

    // 1. Version Control Check (+15 pts)
    if (fs::exists(".git")) {
        score += 15;
        passed.push_back("Git repository initialized (+15)");
    } else {
        warnings.push_back("Missing .git directory (-15)");
    }

    // 2. License Check (+15 pts)
    if (fs::exists("LICENSE") || fs::exists("LICENSE.md") || fs::exists("LICENSE.txt")) {
        score += 15;
        passed.push_back("License file present (+15)");
    } else {
        warnings.push_back("No LICENSE file found (-15)");
    }

    // 3. Documentation Check (+15 pts)
    if (fs::exists("README.md") || fs::exists("README.txt") || fs::exists("README")) {
        score += 15;
        passed.push_back("README file present (+15)");
    } else {
        warnings.push_back("No README.md documentation found (-15)");
    }

    // 4. Build Configuration Check (+20 pts)
    bool hasManifest = fs::exists("CMakeLists.txt") || 
                       fs::exists("package.json") || 
                       fs::exists("Cargo.toml") || 
                       fs::exists("Makefile") || 
                       fs::exists("pyproject.toml");

    if (hasManifest) {
        score += 20;
        passed.push_back("Valid project build manifest detected (+20)");
    } else {
        warnings.push_back("No recognizable build system manifest (CMake, package.json, etc.) (-20)");
    }

    // 5. Source Structure & Organization (+15 pts)
    if (fs::exists("src") || fs::exists("include") || fs::exists("lib")) {
        score += 15;
        passed.push_back("Standard source directory structure present (+15)");
    } else {
        warnings.push_back("Missing standard src/ or include/ structure (-15)");
    }

    // 6. Ignore Rules Check (+10 pts)
    if (fs::exists(".gitignore")) {
        score += 10;
        passed.push_back(".gitignore configuration active (+10)");
    } else {
        warnings.push_back("Missing .gitignore file (-10)");
    }

    // 7. Testing Rig Check (+10 pts)
    if (fs::exists("tests") || fs::exists("test") || fs::exists("spec")) {
        score += 10;
        passed.push_back("Test suite directory present (+10)");
    } else {
        warnings.push_back("No tests/ or test/ directory found (-10)");
    }

    std::cout << "  Passed Checks:\n";
    for (const auto& item : passed) {
        std::cout << "    [+] " << item << "\n";
    }

    if (!warnings.empty()) {
        std::cout << "\n  Areas for Improvement:\n";
        for (const auto& warn : warnings) {
            std::cout << "    [-] " << warn << "\n";
        }
    }

    std::string grade = getGrade(score);

    std::cout << "\n--------------------------------------------\n";
    std::cout << "  Overall Score: " << score << " / " << maxScore << " (" << (score * 100 / maxScore) << "%)\n";
    std::cout << "  Grade:         " << grade << "\n";
    std::cout << "--------------------------------------------\n\n";

    return 0;
}

} // namespace forge