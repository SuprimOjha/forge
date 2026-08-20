#include "forge/commands/release.hpp"
#include "forge/core/process_runner.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>

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

void printReleaseHelp() {
    std::cout
        << "\n"
        << "Forge Release Generator\n\n"

        << "Usage:\n"
        << "  forge release <version> [options]\n\n"

        << "Examples:\n"
        << "  forge release v1.0.0\n"
        << "  forge release v1.0.0 --write\n\n"

        << "Options:\n"
        << "  --write                 Write changes directly to CHANGELOG.md\n"
        << "  -h, --help              Show this help message\n";
}

} // anonymous namespace

int runRelease(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printReleaseHelp();
        return 1;
    }

    std::string version = argv[2];
    if (version == "--help" || version == "-h") {
        printReleaseHelp();
        return 0;
    }

    bool writeToFile = false;
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--write") {
            writeToFile = true;
        }
    }

    // Fetch commit messages using Git
    ProcessResult res = ProcessRunner::run("git log --oneline -n 30");
    if (res.exitCode != 0) {
        std::cerr << "  [ERROR] Failed to retrieve git log history.\n\n";
        return 1;
    }

    std::vector<std::string> features;
    std::vector<std::string> fixes;
    std::vector<std::string> others;

    std::stringstream ss(res.stdOut);
    std::string line;

    while (std::getline(ss, line)) {
        if (line.empty()) continue;

        // Skip the commit hash prefix
        size_t spacePos = line.find(' ');
        std::string msg = (spacePos != std::string::npos) ? line.substr(spacePos + 1) : line;

        if (msg.rfind("feat", 0) == 0 || msg.find("feat:") != std::string::npos) {
            features.push_back(msg);
        } else if (msg.rfind("fix", 0) == 0 || msg.find("fix:") != std::string::npos) {
            fixes.push_back(msg);
        } else {
            others.push_back(msg);
        }
    }

    std::stringstream changelog;
    changelog << "## Release " << version << "\n\n";

    if (!features.empty()) {
        changelog << "### Features\n";
        for (const auto& f : features) {
            changelog << "- " << f << "\n";
        }
        changelog << "\n";
    }

    if (!fixes.empty()) {
        changelog << "### Bug Fixes\n";
        for (const auto& fx : fixes) {
            changelog << "- " << fx << "\n";
        }
        changelog << "\n";
    }

    if (!others.empty()) {
        changelog << "### Other Changes\n";
        for (const auto& o : others) {
            changelog << "- " << o << "\n";
        }
        changelog << "\n";
    }

    std::cout << "\nForge Release & Changelog Generator\n";
    std::cout << "--------------------------------------------\n\n";
    std::cout << changelog.str();

    if (writeToFile) {
        std::string existingContent = "";
        if (fs::exists("CHANGELOG.md")) {
            std::ifstream inFile("CHANGELOG.md");
            std::stringstream buffer;
            buffer << inFile.rdbuf();
            existingContent = buffer.str();
            inFile.close();
        }

        std::ofstream outFile("CHANGELOG.md");
        if (outFile.is_open()) {
            outFile << changelog.str() << "\n" << existingContent;
            outFile.close();
            std::cout << "  [+] Updated CHANGELOG.md successfully.\n\n";
        } else {
            std::cerr << "  [ERROR] Failed to write to CHANGELOG.md\n\n";
            return 1;
        }
    }

    return 0;
}

} // namespace forge