#include "forge/commands/env_check.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <set>
#include <regex>
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

void printEnvCheckHelp() {
    std::cout
        << "\n"
        << "Forge Environment Variable & Security Linter\n\n"

        << "Usage:\n"
        << "  forge env-check [options]\n\n"

        << "Options:\n"
        << "  -d, --dir <path>    Target directory to scan [default: .]\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge env-check\n"
        << "  forge env-check -d src/\n";
}

bool isSourceFile(const fs::path& p) {
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext == ".cpp" || ext == ".hpp" || ext == ".h" || ext == ".c" || ext == ".js" || ext == ".py");
}

std::set<std::string> loadEnvTemplateKeys(const fs::path& envFile) {
    std::set<std::string> keys;
    if (!fs::exists(envFile)) return keys;

    std::ifstream file(envFile);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            std::string key = line.substr(0, eqPos);
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            if (!key.empty()) keys.insert(key);
        }
    }
    return keys;
}

} // anonymous namespace

int runEnvCheck(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path targetDir = ".";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printEnvCheckHelp();
            return 0;
        } else if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            targetDir = argv[++i];
        }
    }

    std::cout << "\nForge Environment Variable Linter\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Scanning Path : " << fs::absolute(targetDir).string() << "\n\n";

    fs::path exampleEnv = targetDir / ".env.example";
    fs::path localEnv = targetDir / ".env";

    std::set<std::string> templateKeys = loadEnvTemplateKeys(exampleEnv);
    if (templateKeys.empty()) {
        templateKeys = loadEnvTemplateKeys(localEnv);
    }

    std::set<std::string> detectedGetenvKeys;
    std::regex getenvRegex(R"raw(getenv\s*\(\s*"([^"]+)"\s*\))raw");

    try {
        for (const auto& entry : fs::recursive_directory_iterator(targetDir, fs::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file() && isSourceFile(entry.path())) {
                std::ifstream file(entry.path());
                if (!file.is_open()) continue;

                std::string line;
                while (std::getline(file, line)) {
                    std::smatch match;
                    std::string::const_iterator searchStart(line.cbegin());
                    while (std::regex_search(searchStart, line.cend(), match, getenvRegex)) {
                        detectedGetenvKeys.insert(match[1]);
                        searchStart = match.suffix().first;
                    }
                }
            }
        }
    } catch (...) {}

    std::cout << "  [i] Source Code getenv() References Found: " << detectedGetenvKeys.size() << "\n";
    for (const auto& key : detectedGetenvKeys) {
        std::cout << "      • " << key << "\n";
    }

    std::cout << "\n  [i] Configured Environment Keys (.env / .env.example): " << templateKeys.size() << "\n";
    for (const auto& key : templateKeys) {
        std::cout << "      • " << key << "\n";
    }

    std::cout << "\n--------------------------------------------\n";
    std::cout << "  Environment Audit Complete.\n\n";

    return 0;
}

} // namespace forge