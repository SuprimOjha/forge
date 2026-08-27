#include "forge/commands/env.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

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

void printEnvHelp() {
    std::cout
        << "\n"
        << "Forge Environment & Config Validator\n\n"

        << "Usage:\n"
        << "  forge env [options]\n\n"

        << "Options:\n"
        << "  -e, --env-file <file>   Specify primary env file (default: .env)\n"
        << "  -x, --example <file>    Specify example template env file (default: .env.example)\n"
        << "  -h, --help              Show this help message\n";
}

std::map<std::string, std::string> parseEnvFile(const fs::path& filePath) {
    std::map<std::string, std::string> envMap;
    std::ifstream file(filePath);
    if (!file.is_open()) return envMap;

    std::string line;
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty() || line[0] == '#') continue;

        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            std::string key = line.substr(0, eqPos);
            std::string val = line.substr(eqPos + 1);
            
            key.erase(key.find_last_not_of(" \t") + 1);
            val.erase(0, val.find_first_not_of(" \t"));
            
            envMap[key] = val;
        }
    }
    return envMap;
}

bool looksLikeSecretKey(const std::string& key) {
    std::string lower = key;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower.find("secret") != std::string::npos ||
           lower.find("token") != std::string::npos ||
           lower.find("password") != std::string::npos ||
           lower.find("key") != std::string::npos ||
           lower.find("auth") != std::string::npos;
}

} // anonymous namespace

int runEnv(int argc, char* argv[]) {
    enableConsoleEncoding();

    std::string envFile = ".env";
    std::string exampleFile = ".env.example";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-e" || arg == "--env-file") && i + 1 < argc) {
            envFile = argv[++i];
        } else if ((arg == "-x" || arg == "--example") && i + 1 < argc) {
            exampleFile = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            printEnvHelp();
            return 0;
        }
    }

    std::cout << "\nForge Environment & Config Validator\n";
    std::cout << "--------------------------------------------\n\n";

    bool hasEnv = fs::exists(envFile);
    bool hasExample = fs::exists(exampleFile);

    if (!hasEnv && !hasExample) {
        std::cout << "  [!] Neither '" << envFile << "' nor '" << exampleFile << "' was found in workspace root.\n\n";
        return 0;
    }

    std::map<std::string, std::string> envVars = parseEnvFile(envFile);
    std::map<std::string, std::string> exampleVars = parseEnvFile(exampleFile);

    std::cout << "  Loaded Environment File : " << (hasEnv ? envFile : "[MISSING]") << " (" << envVars.size() << " keys)\n";
    std::cout << "  Loaded Template File    : " << (hasExample ? exampleFile : "[MISSING]") << " (" << exampleVars.size() << " keys)\n\n";

    int issuesFound = 0;

    // Check for missing keys defined in template
    if (hasExample) {
        std::vector<std::string> missingKeys;
        for (const auto& [key, _] : exampleVars) {
            if (envVars.find(key) == envVars.end()) {
                missingKeys.push_back(key);
            }
        }

        if (!missingKeys.empty()) {
            std::cout << "  [!] Missing Required Environment Variables (" << missingKeys.size() << "):\n";
            for (const auto& key : missingKeys) {
                std::cout << "      - " << key << "\n";
                issuesFound++;
            }
            std::cout << "\n";
        }
    }

    // Inspect sensitive unmasked values
    int unmaskedSecrets = 0;
    for (const auto& [key, val] : envVars) {
        if (looksLikeSecretKey(key) && !val.empty() && val != "change_me" && val != "your_key_here") {
            unmaskedSecrets++;
        }
    }

    if (unmaskedSecrets > 0) {
        std::cout << "  [*] Detected " << unmaskedSecrets << " sensitive configuration key(s) with non-default values.\n";
    }

    if (issuesFound == 0) {
        std::cout << "  [OK] Environment configuration is valid and matches template.\n\n";
        return 0;
    }

    std::cout << "  [!] Environment validation completed with " << issuesFound << " issue(s).\n\n";
    return 1;
}

} // namespace forge