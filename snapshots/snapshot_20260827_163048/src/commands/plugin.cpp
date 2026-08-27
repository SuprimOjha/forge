#include "forge/commands/plugin.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
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

fs::path getPluginDirectory() {
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

    fs::path pluginDir = homeDir / ".forge" / "plugins";
    if (!fs::exists(pluginDir)) {
        fs::create_directories(pluginDir);
    }
    return pluginDir;
}

void printPluginHelp() {
    std::cout
        << "\n"
        << "Forge Plugin Manager\n\n"

        << "Usage:\n"
        << "  forge plugin <command> [options]\n\n"

        << "Commands:\n"
        << "  list                    List all installed plugins\n"
        << "  path                    Print plugins directory path\n"
        << "  create <name>           Scaffold a new local plugin template\n"
        << "  remove <name>           Uninstall a local plugin\n\n"

        << "Options:\n"
        << "  -h, --help              Show this help message\n";
}

bool scaffoldPlugin(const std::string& name) {
    fs::path pluginDir = getPluginDirectory() / name;
    if (fs::exists(pluginDir)) {
        std::cerr << "  [ERROR] Plugin '" << name << "' already exists at " << pluginDir.string() << "\n\n";
        return false;
    }

    fs::create_directories(pluginDir);

    std::ofstream manifest(pluginDir / "plugin.json");
    if (!manifest.is_open()) return false;

    manifest << "{\n"
             << "  \"name\": \"" << name << "\",\n"
             << "  \"version\": \"0.1.0\",\n"
             << "  \"description\": \"Custom Forge extension plugin\",\n"
             << "  \"entry\": \"main.py\"\n"
             << "}\n";
    manifest.close();

    std::ofstream script(pluginDir / "main.py");
    if (!script.is_open()) return false;

    script << "#!/usr/bin/env python3\n"
           << "# Auto-generated Forge plugin script\n"
           << "print('Hello from " << name << " plugin!')\n";
    script.close();

    return true;
}

} // anonymous namespace

int runPlugin(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printPluginHelp();
        return 0;
    }

    const std::string command = argv[2];

    if (command == "--help" || command == "-h") {
        printPluginHelp();
        return 0;
    }

    fs::path pluginDir = getPluginDirectory();

    /*
     * List
     */
    if (command == "list") {
        std::cout
            << "\n"
            << "Forge Installed Plugins\n"
            << "--------------------------------------------\n\n"
            << "Location: " << pluginDir.string() << "\n\n";

        int count = 0;
        for (const auto& entry : fs::directory_iterator(pluginDir)) {
            if (entry.is_directory()) {
                std::cout << "  - " << entry.path().filename().string();
                fs::path manifestPath = entry.path() / "plugin.json";
                if (fs::exists(manifestPath)) {
                    std::cout << " [Manifest Loaded]";
                }
                std::cout << "\n";
                ++count;
            }
        }

        if (count == 0) {
            std::cout << "  (No plugins installed)\n\n"
                      << "  Run 'forge plugin create <name>' to create your first extension!\n\n";
        } else {
            std::cout << "\n  Total Installed: " << count << "\n\n";
        }

        return 0;
    }

    /*
     * Path
     */
    if (command == "path") {
        std::cout << pluginDir.string() << "\n";
        return 0;
    }

    /*
     * Create
     */
    if (command == "create") {
        if (argc < 4) {
            std::cerr << "forge plugin create: missing plugin name\n\n";
            printPluginHelp();
            return 1;
        }

        const std::string pluginName = argv[3];
        if (scaffoldPlugin(pluginName)) {
            std::cout << "[+] Scaffolded plugin '" << pluginName << "' at " << (pluginDir / pluginName).string() << "\n\n";
            return 0;
        }
        return 1;
    }

    /*
     * Remove
     */
    if (command == "remove") {
        if (argc < 4) {
            std::cerr << "forge plugin remove: missing plugin name\n\n";
            printPluginHelp();
            return 1;
        }

        const std::string pluginName = argv[3];
        fs::path target = pluginDir / pluginName;

        if (!fs::exists(target)) {
            std::cerr << "forge plugin remove: plugin '" << pluginName << "' not found\n\n";
            return 1;
        }

        std::error_code ec;
        fs::remove_all(target, ec);
        if (!ec) {
            std::cout << "[+] Removed plugin '" << pluginName << "'\n\n";
            return 0;
        }

        std::cerr << "forge plugin remove: failed to delete plugin: " << ec.message() << "\n\n";
        return 1;
    }

    std::cerr << "forge plugin: unknown command '" << command << "'\n\n";
    printPluginHelp();
    return 1;
}

} // namespace forge