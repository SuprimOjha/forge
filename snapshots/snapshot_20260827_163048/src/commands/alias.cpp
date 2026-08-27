#include "forge/commands/alias.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
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

void printAliasHelp() {
    std::cout
        << "\n"
        << "Forge Command Alias & Shortcut Manager\n\n"

        << "Usage:\n"
        << "  forge alias [options]\n"
        << "  forge alias <name>=<command>\n\n"

        << "Options:\n"
        << "  -l, --list          List all configured aliases\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge alias b=\"build --config Release\"\n"
        << "  forge alias -l\n";
}

std::map<std::string, std::string> loadAliases(const fs::path& configPath) {
    std::map<std::string, std::string> aliases;
    if (!fs::exists(configPath)) return aliases;

    std::ifstream file(configPath);
    std::string line;
    bool inAliasSection = false;

    while (std::getline(file, line)) {
        if (line == "[alias]") {
            inAliasSection = true;
            continue;
        } else if (!line.empty() && line[0] == '[') {
            inAliasSection = false;
            continue;
        }

        if (inAliasSection) {
            size_t eqPos = line.find('=');
            if (eqPos != std::string::npos) {
                std::string key = line.substr(0, eqPos);
                std::string val = line.substr(eqPos + 1);
                aliases[key] = val;
            }
        }
    }
    return aliases;
}

} // anonymous namespace

int runAlias(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path configPath = ".forgerc";

    if (argc < 3) {
        // Default behavior: list aliases
        auto aliases = loadAliases(configPath);
        std::cout << "\nForge Configured Aliases (.forgerc)\n";
        std::cout << "--------------------------------------------\n";
        
        if (aliases.empty()) {
            std::cout << "  (No custom aliases configured)\n\n";
            return 0;
        }

        for (const auto& [name, target] : aliases) {
            std::cout << "  forge " << std::left << std::setw(12) << name << " -> forge " << target << "\n";
        }
        std::cout << "\n";
        return 0;
    }

    std::string arg = argv[2];
    if (arg == "-h" || arg == "--help") {
        printAliasHelp();
        return 0;
    } else if (arg == "-l" || arg == "--list") {
        auto aliases = loadAliases(configPath);
        std::cout << "\nForge Configured Aliases (.forgerc)\n";
        std::cout << "--------------------------------------------\n";
        if (aliases.empty()) {
            std::cout << "  (No custom aliases configured)\n\n";
        } else {
            for (const auto& [name, target] : aliases) {
                std::cout << "  forge " << std::left << std::setw(12) << name << " -> forge " << target << "\n";
            }
            std::cout << "\n";
        }
        return 0;
    }

    // Save alias assignment format: alias_name=command
    size_t eqPos = arg.find('=');
    if (eqPos != std::string::npos) {
        std::string aliasName = arg.substr(0, eqPos);
        std::string targetCmd = arg.substr(eqPos + 1);

        std::ofstream file(configPath, std::ios::app);
        if (file.is_open()) {
            file << "\n[alias]\n" << aliasName << "=" << targetCmd << "\n";
            std::cout << "\n  [+] Saved Alias: forge " << aliasName << " -> forge " << targetCmd << "\n\n";
            return 0;
        } else {
            std::cerr << "  [!] Error writing to .forgerc configuration file.\n\n";
            return 1;
        }
    }

    printAliasHelp();
    return 1;
}

} // namespace forge