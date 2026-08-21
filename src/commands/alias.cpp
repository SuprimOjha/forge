#include "forge/commands/alias.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>
#include <cstdlib>

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

fs::path getAliasFilePath() {
    fs::path homeDir;
#ifdef _WIN32
    const char* userProfile = std::getenv("USERPROFILE");
    if (userProfile) {
        homeDir = fs::path(userProfile);
    } else {
        const char* drive = std::getenv("HOMEDRIVE");
        const char* path = std::getenv("HOMEPATH");
        if (drive && path) homeDir = fs::path(std::string(drive) + std::string(path));
    }
#else
    const char* home = std::getenv("HOME");
    if (home) homeDir = fs::path(home);
#endif
    if (homeDir.empty()) homeDir = fs::current_path();

    fs::path forgeDir = homeDir / ".forge";
    if (!fs::exists(forgeDir)) {
        fs::create_directories(forgeDir);
    }
    return forgeDir / "aliases.txt";
}

void printAliasHelp() {
    std::cout
        << "\n"
        << "Forge Command Alias Manager\n\n"

        << "Usage:\n"
        << "  forge alias <command> [options]\n\n"

        << "Commands:\n"
        << "  list                    List all configured command shortcuts\n"
        << "  set <key> <command>     Create or update a command shortcut\n"
        << "  remove <key>            Delete a command shortcut\n"
        << "  path                    Print alias configuration file path\n\n"

        << "Options:\n"
        << "  -h, --help              Show this help message\n";
}

std::map<std::string, std::string> loadAliases() {
    std::map<std::string, std::string> aliases;
    fs::path aliasFile = getAliasFilePath();
    if (!fs::exists(aliasFile)) return aliases;

    std::ifstream inFile(aliasFile);
    std::string line;
    while (std::getline(inFile, line)) {
        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            std::string key = line.substr(0, eqPos);
            std::string val = line.substr(eqPos + 1);
            if (!key.empty() && !val.empty()) {
                aliases[key] = val;
            }
        }
    }
    return aliases;
}

void saveAliases(const std::map<std::string, std::string>& aliases) {
    fs::path aliasFile = getAliasFilePath();
    std::ofstream outFile(aliasFile);
    for (const auto& [key, val] : aliases) {
        outFile << key << "=" << val << "\n";
    }
}

} // anonymous namespace

int runAlias(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printAliasHelp();
        return 0;
    }

    const std::string command = argv[2];

    if (command == "--help" || command == "-h") {
        printAliasHelp();
        return 0;
    }

    auto aliases = loadAliases();

    /*
     * List
     */
    if (command == "list") {
        std::cout
            << "\n"
            << "Forge Configured Command Aliases\n"
            << "--------------------------------------------\n\n";

        if (aliases.empty()) {
            std::cout << "  (No aliases created yet)\n\n"
                      << "  Run 'forge alias set <key> <command>' to add your first alias!\n\n";
            return 0;
        }

        for (const auto& [key, val] : aliases) {
            std::cout << "  " << key << " -> " << val << "\n";
        }
        std::cout << "\n";
        return 0;
    }

    /*
     * Path
     */
    if (command == "path") {
        std::cout << getAliasFilePath().string() << "\n";
        return 0;
    }

    /*
     * Set
     */
    if (command == "set") {
        if (argc < 5) {
            std::cerr << "forge alias set: missing key or command target\n\n";
            printAliasHelp();
            return 1;
        }

        const std::string key = argv[3];
        std::string targetCmd = argv[4];
        for (int i = 5; i < argc; ++i) {
            targetCmd += " ";
            targetCmd += argv[i];
        }

        aliases[key] = targetCmd;
        saveAliases(aliases);

        std::cout << "[+] Set alias '" << key << "' -> '" << targetCmd << "'\n\n";
        return 0;
    }

    /*
     * Remove
     */
    if (command == "remove") {
        if (argc < 4) {
            std::cerr << "forge alias remove: missing key\n\n";
            printAliasHelp();
            return 1;
        }

        const std::string key = argv[3];
        if (aliases.erase(key)) {
            saveAliases(aliases);
            std::cout << "[+] Removed alias '" << key << "'\n\n";
            return 0;
        }

        std::cerr << "forge alias remove: alias '" << key << "' not found\n\n";
        return 1;
    }

    std::cerr << "forge alias: unknown command '" << command << "'\n\n";
    printAliasHelp();
    return 1;
}

} // namespace forge