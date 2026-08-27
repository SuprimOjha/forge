#include "forge/commands/init.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
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

bool createCppProject(const std::string& projName) {
    std::cout << "  [*] Generating C++ (CMake) workspace structure for '" << projName << "'...\n";

    fs::create_directories("src");
    fs::create_directories("include/" + projName);

    std::ofstream mainFile("src/main.cpp");
    if (!mainFile.is_open()) return false;
    mainFile << "#include <iostream>\n\n"
             << "int main() {\n"
             << "    std::cout << \"Hello from " << projName << "!\" << std::endl;\n"
             << "    return 0;\n"
             << "}\n";
    mainFile.close();

    std::ofstream cmakeFile("CMakeLists.txt");
    if (!cmakeFile.is_open()) return false;
    cmakeFile << "cmake_minimum_required(VERSION 3.16)\n"
              << "project(" << projName << " VERSION 0.1.0 LANGUAGES CXX)\n\n"
              << "set(CMAKE_CXX_STANDARD 17)\n"
              << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n"
              << "add_executable(" << projName << " src/main.cpp)\n"
              << "target_include_directories(" << projName << " PRIVATE include)\n";
    cmakeFile.close();

    std::ofstream gitignore(".gitignore");
    if (gitignore.is_open()) {
        gitignore << "build/\nbin/\n.vscode/\n.idea/\n";
    }

    return true;
}

bool createNodeProject(const std::string& projName) {
    std::cout << "  [*] Generating Node.js workspace structure for '" << projName << "'...\n";

    fs::create_directories("src");

    std::ofstream indexFile("src/index.js");
    if (!indexFile.is_open()) return false;
    indexFile << "console.log('Hello from " << projName << "!');\n";
    indexFile.close();

    std::ofstream pkgFile("package.json");
    if (!pkgFile.is_open()) return false;
    pkgFile << "{\n"
            << "  \"name\": \"" << projName << "\",\n"
            << "  \"version\": \"0.1.0\",\n"
            << "  \"main\": \"src/index.js\",\n"
            << "  \"scripts\": {\n"
            << "    \"start\": \"node src/index.js\",\n"
            << "    \"build\": \"echo \\\"No build step required\\\"\"\n"
            << "  }\n"
            << "}\n";
    pkgFile.close();

    std::ofstream gitignore(".gitignore");
    if (gitignore.is_open()) {
        gitignore << "node_modules/\n.env\n";
    }

    return true;
}

} // anonymous namespace

int runInit(const std::vector<std::string>& args) {
    enableConsoleEncoding();

    std::cout << "\nForge Interactive Setup Wizard\n";
    std::cout << "--------------------------------------------\n\n";

    if (fs::exists("CMakeLists.txt") || fs::exists("package.json")) {
        std::cout << "  [ERROR] A project manifest already exists in this directory.\n";
        std::cout << "  Aborting initialization to prevent overwriting existing configuration.\n\n";
        return 1;
    }

    std::string currentDirName = fs::current_path().filename().string();
    std::string projectType = "";

    // Parse flags if provided
    for (size_t i = 0; i < args.size(); ++i) {
        if ((args[i] == "--type" || args[i] == "-t") && i + 1 < args.size()) {
            projectType = args[i + 1];
        }
    }

    // Interactive fallback prompt if no --type flag passed
    if (projectType.empty()) {
        std::cout << "  Select project template:\n";
        std::cout << "    [1] C++ (CMake)\n";
        std::cout << "    [2] Node.js (JavaScript)\n\n";
        std::cout << "  Choice [1-2, default: 1]: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "2" || choice == "node" || choice == "js") {
            projectType = "node";
        } else {
            projectType = "cpp";
        }
        std::cout << "\n";
    }

    std::transform(projectType.begin(), projectType.end(), projectType.begin(), ::tolower);

    bool success = false;
    if (projectType == "cpp" || projectType == "cmake" || projectType == "c++") {
        success = createCppProject(currentDirName);
    } else if (projectType == "node" || projectType == "js" || projectType == "ts") {
        success = createNodeProject(currentDirName);
    } else {
        std::cout << "  [ERROR] Unsupported template type: " << projectType << "\n\n";
        return 1;
    }

    if (success) {
        std::cout << "\n  [OK] Project '" << currentDirName << "' initialized successfully.\n";
        std::cout << "  Run 'forge check' or 'forge build' to begin.\n\n";
        return 0;
    }

    std::cout << "  [ERROR] Failed to write project template files.\n\n";
    return 1;
}

} // namespace forge