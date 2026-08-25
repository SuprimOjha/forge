#include "forge/commands/template.hpp"

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

fs::path getTemplateDirectory() {
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

    fs::path templateDir = homeDir / ".forge" / "templates";
    if (!fs::exists(templateDir)) {
        fs::create_directories(templateDir);
    }
    return templateDir;
}

void printTemplateHelp() {
    std::cout
        << "\n"
        << "Forge Template Engine\n\n"

        << "Usage:\n"
        << "  forge template <command> [options]\n\n"

        << "Commands:\n"
        << "  list                    List available built-in and user templates\n"
        << "  path                    Print custom template directory path\n"
        << "  save <name>             Save current directory as a custom template\n"
        << "  use <name> <target>     Generate a new project from a saved template\n\n"

        << "Options:\n"
        << "  -h, --help              Show this help message\n";
}

bool saveCurrentAsTemplate(const std::string& name) {
    fs::path tplDir = getTemplateDirectory() / name;
    if (fs::exists(tplDir)) {
        std::cerr << "  [ERROR] Template '" << name << "' already exists at " << tplDir.string() << "\n\n";
        return false;
    }

    std::error_code ec;
    fs::create_directories(tplDir, ec);

    // Copy src, include, CMakeLists.txt if present
    std::vector<std::string> copyTargets = {"src", "include", "CMakeLists.txt", "package.json", "README.md"};
    for (const auto& item : copyTargets) {
        if (fs::exists(item)) {
            fs::copy(item, tplDir / item, fs::copy_options::recursive, ec);
        }
    }

    return true;
}

bool instantiateTemplate(const std::string& templateName, const std::string& targetDir) {
    fs::path tplDir = getTemplateDirectory() / templateName;
    if (!fs::exists(tplDir)) {
        std::cerr << "  [ERROR] Template '" << templateName << "' not found.\n\n";
        return false;
    }

    fs::path destination = fs::current_path() / targetDir;
    if (fs::exists(destination)) {
        std::cerr << "  [ERROR] Target directory '" << targetDir << "' already exists.\n\n";
        return false;
    }

    std::error_code ec;
    fs::copy(tplDir, destination, fs::copy_options::recursive, ec);
    return !ec;
}

} // anonymous namespace

int runTemplate(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printTemplateHelp();
        return 0;
    }

    const std::string command = argv[2];

    if (command == "--help" || command == "-h") {
        printTemplateHelp();
        return 0;
    }

    fs::path templateDir = getTemplateDirectory();

    /*
     * List
     */
    if (command == "list") {
        std::cout
            << "\n"
            << "Forge Workspace Templates\n"
            << "--------------------------------------------\n\n"
            << "Built-in Presets:\n"
            << "  - cpp-cli               Modern C++ CLI Application Scaffold\n"
            << "  - node-express          Lightweight Node.js REST API Scaffold\n\n"
            << "Custom Templates (" << templateDir.string() << "):\n";

        int customCount = 0;
        for (const auto& entry : fs::directory_iterator(templateDir)) {
            if (entry.is_directory()) {
                std::cout << "  - " << entry.path().filename().string() << "\n";
                ++customCount;
            }
        }

        if (customCount == 0) {
            std::cout << "  (No custom user templates saved yet)\n\n"
                      << "  Run 'forge template save <name>' to store your current setup!\n\n";
        } else {
            std::cout << "\n  Total Custom: " << customCount << "\n\n";
        }

        return 0;
    }

    /*
     * Path
     */
    if (command == "path") {
        std::cout << templateDir.string() << "\n";
        return 0;
    }

    /*
     * Save
     */
    if (command == "save") {
        if (argc < 4) {
            std::cerr << "forge template save: missing template name\n\n";
            printTemplateHelp();
            return 1;
        }

        const std::string tplName = argv[3];
        if (saveCurrentAsTemplate(tplName)) {
            std::cout << "[+] Saved current project as template '" << tplName << "'!\n\n";
            return 0;
        }
        return 1;
    }

    /*
     * Use
     */
    if (command == "use") {
        if (argc < 5) {
            std::cerr << "forge template use: missing template name or target directory\n\n";
            printTemplateHelp();
            return 1;
        }

        const std::string tplName = argv[3];
        const std::string targetName = argv[4];

        if (instantiateTemplate(tplName, targetName)) {
            std::cout << "[+] Successfully created project '" << targetName << "' from template '" << tplName << "'!\n\n";
            return 0;
        }
        return 1;
    }

    std::cerr << "forge template: unknown command '" << command << "'\n\n";
    printTemplateHelp();
    return 1;
}

} // namespace forge