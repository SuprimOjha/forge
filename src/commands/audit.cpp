#include "forge/commands/audit.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <regex>

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

void auditCppProject() {
    std::cout << "  Manifest detected: CMakeLists.txt\n\n";

    std::ifstream file("CMakeLists.txt");
    if (!file.is_open()) {
        std::cout << "  [ERROR] Unable to open CMakeLists.txt for reading.\n\n";
        return;
    }

    std::string line;
    std::vector<std::string> findPackages;
    bool cxxStandardFound = false;

    std::regex packageRegex(R"(find_package\s*\(\s*([A-Za-z0-9_]+))");
    std::regex stdRegex(R"(CMAKE_CXX_STANDARD)");

    while (std::getline(file, line)) {
        std::smatch match;
        if (std::regex_search(line, match, packageRegex)) {
            findPackages.push_back(match[1].str());
        }
        if (std::regex_search(line, stdRegex)) {
            cxxStandardFound = true;
        }
    }
    file.close();

    std::cout << "  Declared Dependencies (find_package):\n";
    if (findPackages.empty()) {
        std::cout << "    - None explicit\n";
    } else {
        for (const auto& pkg : findPackages) {
            std::cout << "    - " << pkg << "\n";
        }
    }

    std::cout << "\n  Auditing Configurations:\n";
    if (cxxStandardFound) {
        std::cout << "    [OK] C++ standard specification present\n";
    } else {
        std::cout << "    [WARNING] CMAKE_CXX_STANDARD is missing\n";
    }

    // Source audit check
    if (fs::exists("src")) {
        int cppCount = 0;
        for (const auto& entry : fs::recursive_directory_iterator("src")) {
            if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
                ++cppCount;
            }
        }
        std::cout << "    [OK] Found " << cppCount << " source file(s) in 'src/'\n";
    } else {
        std::cout << "    [WARNING] 'src/' directory not found\n";
    }
    std::cout << "\n";
}

void auditNodeProject() {
    std::cout << "  Manifest detected: package.json\n\n";

    std::ifstream file("package.json");
    if (!file.is_open()) {
        std::cout << "  [ERROR] Unable to open package.json for reading.\n\n";
        return;
    }

    std::cout << "  Auditing Environment:\n";
    if (fs::exists("node_modules")) {
        std::cout << "    [OK] 'node_modules/' directory present\n";
    } else {
        std::cout << "    [WARNING] 'node_modules/' missing (run 'npm install')\n";
    }

    if (fs::exists("package-lock.json")) {
        std::cout << "    [OK] Lockfile present (package-lock.json)\n";
    } else {
        std::cout << "    [WARNING] Lockfile missing\n";
    }
    std::cout << "\n";
}

} // anonymous namespace

int runAudit(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    enableConsoleEncoding();

    std::cout << "\nForge Dependency & Health Auditor\n";
    std::cout << "--------------------------------------------\n\n";

    if (fs::exists("CMakeLists.txt")) {
        auditCppProject();
    } else if (fs::exists("package.json")) {
        auditNodeProject();
    } else {
        std::cout << "  [ERROR] No supported manifest (CMakeLists.txt / package.json) found.\n";
        std::cout << "  Run 'forge init' to setup a new workspace.\n\n";
        return 1;
    }

    std::cout << "  [OK] Audit check complete.\n\n";
    return 0;
}

} // namespace forge