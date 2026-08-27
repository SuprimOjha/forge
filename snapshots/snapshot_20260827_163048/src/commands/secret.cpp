#include "forge/commands/secret.hpp"

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

fs::path getSecretFilePath() {
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
    return forgeDir / "secrets.txt";
}

void printSecretHelp() {
    std::cout
        << "\n"
        << "Forge Global Environment & Secret Vault\n\n"

        << "Usage:\n"
        << "  forge secret <command> [options]\n\n"

        << "Commands:\n"
        << "  list                    List all saved secret keys (values masked)\n"
        << "  set <key> <value>       Save or update a secret in the vault\n"
        << "  get <key>               Reveal unmasked secret value\n"
        << "  remove <key>            Delete a secret from the vault\n"
        << "  path                    Print secrets storage path\n\n"

        << "Options:\n"
        << "  -h, --help              Show this help message\n";
}

std::map<std::string, std::string> loadSecrets() {
    std::map<std::string, std::string> secrets;
    fs::path secretFile = getSecretFilePath();
    if (!fs::exists(secretFile)) return secrets;

    std::ifstream inFile(secretFile);
    std::string line;
    while (std::getline(inFile, line)) {
        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            std::string key = line.substr(0, eqPos);
            std::string val = line.substr(eqPos + 1);
            if (!key.empty() && !val.empty()) {
                secrets[key] = val;
            }
        }
    }
    return secrets;
}

void saveSecrets(const std::map<std::string, std::string>& secrets) {
    fs::path secretFile = getSecretFilePath();
    std::ofstream outFile(secretFile);
    for (const auto& [key, val] : secrets) {
        outFile << key << "=" << val << "\n";
    }
}

std::string maskSecret(const std::string& val) {
    if (val.length() <= 4) return "****";
    return val.substr(0, 2) + "...." + val.substr(val.length() - 2);
}

} // anonymous namespace

int runSecret(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printSecretHelp();
        return 0;
    }

    const std::string command = argv[2];

    if (command == "--help" || command == "-h") {
        printSecretHelp();
        return 0;
    }

    auto secrets = loadSecrets();

    /*
     * List
     */
    if (command == "list") {
        std::cout
            << "\n"
            << "Forge Configured Secrets & Vault Keys\n"
            << "--------------------------------------------\n\n";

        if (secrets.empty()) {
            std::cout << "  (No secrets stored yet)\n\n"
                      << "  Run 'forge secret set <key> <value>' to save a secret key!\n\n";
            return 0;
        }

        for (const auto& [key, val] : secrets) {
            std::cout << "  " << key << " = " << maskSecret(val) << "\n";
        }
        std::cout << "\n";
        return 0;
    }

    /*
     * Path
     */
    if (command == "path") {
        std::cout << getSecretFilePath().string() << "\n";
        return 0;
    }

    /*
     * Get
     */
    if (command == "get") {
        if (argc < 4) {
            std::cerr << "forge secret get: missing key\n\n";
            printSecretHelp();
            return 1;
        }
        const std::string key = argv[3];
        auto it = secrets.find(key);
        if (it != secrets.end()) {
            std::cout << it->second << "\n";
            return 0;
        }
        std::cerr << "forge secret get: key '" << key << "' not found in vault\n\n";
        return 1;
    }

    /*
     * Set
     */
    if (command == "set") {
        if (argc < 5) {
            std::cerr << "forge secret set: missing key or secret value\n\n";
            printSecretHelp();
            return 1;
        }

        const std::string key = argv[3];
        const std::string val = argv[4];

        secrets[key] = val;
        saveSecrets(secrets);

        std::cout << "[+] Stored secret key '" << key << "' (" << maskSecret(val) << ") in vault!\n\n";
        return 0;
    }

    /*
     * Remove
     */
    if (command == "remove") {
        if (argc < 4) {
            std::cerr << "forge secret remove: missing key\n\n";
            printSecretHelp();
            return 1;
        }

        const std::string key = argv[3];
        if (secrets.erase(key)) {
            saveSecrets(secrets);
            std::cout << "[+] Removed secret key '" << key << "' from vault\n\n";
            return 0;
        }

        std::cerr << "forge secret remove: key '" << key << "' not found\n\n";
        return 1;
    }

    std::cerr << "forge secret: unknown command '" << command << "'\n\n";
    printSecretHelp();
    return 1;
}

} // namespace forge