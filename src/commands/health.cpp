#include "forge/commands/health.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

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

void printHealthHelp() {
    std::cout
        << "\n"
        << "Forge Workspace Health & Readiness Scorecard\n\n"

        << "Usage:\n"
        << "  forge health [options]\n\n"

        << "Options:\n"
        << "  -d, --dir <path>    Target workspace directory [default: .]\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge health\n"
        << "  forge health -d D:\\Projects\\forge\n";
}

bool hasLicense(const fs::path& dir) {
    return fs::exists(dir / "LICENSE") || fs::exists(dir / "LICENSE.txt") || fs::exists(dir / "LICENSE.md");
}

bool hasGit(const fs::path& dir) {
    return fs::exists(dir / ".git");
}

bool hasConfigFile(const fs::path& dir) {
    return fs::exists(dir / ".forgerc") || fs::exists(dir / "CMakeLists.txt") || fs::exists(dir / "package.json");
}

size_t countSourceFiles(const fs::path& dir) {
    size_t count = 0;
    try {
        for (const auto& entry : fs::recursive_directory_iterator(dir, fs::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".cpp" || ext == ".hpp" || ext == ".h" || ext == ".c" || ext == ".py" || ext == ".js") {
                    count++;
                }
            }
        }
    } catch (...) {}
    return count;
}

} // anonymous namespace

int runHealth(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path targetDir = ".";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printHealthHelp();
            return 0;
        } else if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            targetDir = argv[++i];
        }
    }

    std::cout << "\nForge Workspace Health Scorecard\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Target Workspace : " << fs::absolute(targetDir).string() << "\n\n";

    if (!fs::exists(targetDir)) {
        std::cerr << "  [!] Error: Directory not found: " << targetDir.string() << "\n\n";
        return 1;
    }

    int score = 0;
    int totalChecks = 5;

    // Check 1: Git repository initialized
    bool gitCheck = hasGit(targetDir);
    if (gitCheck) score += 20;
    std::cout << "  [" << (gitCheck ? "OK" : "WARN") << "] Git Repository Status  : " << (gitCheck ? "Initialized" : "Missing .git folder") << "\n";

    // Check 2: License presence
    bool licenseCheck = hasLicense(targetDir);
    if (licenseCheck) score += 20;
    std::cout << "  [" << (licenseCheck ? "OK" : "WARN") << "] License Audit        : " << (licenseCheck ? "Present" : "Missing LICENSE file") << "\n";

    // Check 3: Build configuration present
    bool configCheck = hasConfigFile(targetDir);
    if (configCheck) score += 20;
    std::cout << "  [" << (configCheck ? "OK" : "WARN") << "] Project Configuration: " << (configCheck ? "Configured" : "No project descriptor found") << "\n";

    // Check 4: Compiled build artifact presence
    bool buildCheck = fs::exists(targetDir / "build");
    if (buildCheck) score += 20;
    std::cout << "  [" << (buildCheck ? "OK" : "INFO") << "] Build Artifacts      : " << (buildCheck ? "build/ directory ready" : "Not built yet") << "\n";

    // Check 5: Source files present
    size_t srcCount = countSourceFiles(targetDir);
    bool srcCheck = srcCount > 0;
    if (srcCheck) score += 20;
    std::cout << "  [" << (srcCheck ? "OK" : "WARN") << "] Source Code Coverage : " << srcCount << " source files detected\n";

    std::cout << "--------------------------------------------\n";
    std::cout << "  Overall Health Score : " << score << "% (" << score / 20 << "/" << totalChecks << " checks passed)\n";
    
    if (score == 100) {
        std::cout << "  Status               : 🟢 EXCELLENT (Production Ready)\n\n";
    } else if (score >= 60) {
        std::cout << "  Status               : 🟡 GOOD (Minor items pending)\n\n";
    } else {
        std::cout << "  Status               : 🔴 NEEDS ATTENTION\n\n";
    }

    return 0;
}

} // namespace forge