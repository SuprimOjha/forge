#include "forge/commands/arch.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <set>
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

void printArchHelp() {
    std::cout
        << "\n"
        << "Forge Architecture & Layer Isolation Inspector\n\n"

        << "Usage:\n"
        << "  forge arch [options]\n\n"

        << "Options:\n"
        << "  -d, --dir <path>    Target root directory [default: .]\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge arch\n"
        << "  forge arch -d src/\n";
}

bool isSourceFile(const fs::path& p) {
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext == ".cpp" || ext == ".hpp" || ext == ".h");
}

} // anonymous namespace

int runArch(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path targetDir = ".";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printArchHelp();
            return 0;
        } else if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            targetDir = argv[++i];
        }
    }

    std::cout << "\nForge Workspace Architecture & Layer Inspector\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Root Path : " << fs::absolute(targetDir).string() << "\n\n";

    std::map<std::string, size_t> layerFileCount;
    size_t violationCount = 0;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(targetDir, fs::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file() && isSourceFile(entry.path())) {
                fs::path rel = fs::relative(entry.path(), targetDir);
                std::string parentDir = rel.parent_path().string();
                if (parentDir.empty()) parentDir = "root";
                std::replace(parentDir.begin(), parentDir.end(), '\\', '/');

                layerFileCount[parentDir]++;

                // Architectural Rule Check: Core components shouldn't depend on Command layer
                if (parentDir.find("core") != std::string::npos || parentDir.find("ui") != std::string::npos) {
                    std::ifstream file(entry.path());
                    if (file.is_open()) {
                        std::string line;
                        while (std::getline(file, line)) {
                            if (line.find("#include") != std::string::npos && line.find("commands/") != std::string::npos) {
                                std::cout << "  [!] Architectural Violation: Core/UI component includes Command header!\n";
                                std::cout << "      File: " << rel.string() << "\n";
                                std::cout << "      Line: " << line << "\n\n";
                                violationCount++;
                            }
                        }
                    }
                }
            }
        }
    } catch (...) {}

    std::cout << "  Layer Breakdown:\n";
    for (const auto& [layer, count] : layerFileCount) {
        std::cout << "    • " << layer << " : " << count << " module file(s)\n";
    }

    std::cout << "\n--------------------------------------------\n";
    if (violationCount == 0) {
        std::cout << "  Layer Isolation Integrity: 🟢 PASSED (0 violations)\n\n";
    } else {
        std::cout << "  Layer Isolation Integrity: 🔴 WARN (" << violationCount << " violation(s) detected)\n\n";
    }

    return 0;
}

} // namespace forge