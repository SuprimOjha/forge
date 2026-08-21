#include "forge/commands/graph.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
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

void printGraphHelp() {
    std::cout
        << "\n"
        << "Forge Dependency Visualizer\n\n"

        << "Usage:\n"
        << "  forge graph [options]\n\n"

        << "Options:\n"
        << "  --dot                   Output in Graphviz DOT format\n"
        << "  -h, --help              Show this help message\n";
}

} // anonymous namespace

int runGraph(int argc, char* argv[]) {
    enableConsoleEncoding();

    bool dotFormat = false;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--dot") {
            dotFormat = true;
        } else if (arg == "-h" || arg == "--help") {
            printGraphHelp();
            return 0;
        }
    }

    std::map<std::string, std::vector<std::string>> deps;
    std::regex includeRegex(R"(#include\s*["<](forge/[^">]+)[">])");

    auto scanDir = [&](const std::string& dirPath) {
        if (!fs::exists(dirPath)) return;

        for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".cpp" || ext == ".hpp" || ext == ".h") {
                    std::ifstream file(entry.path());
                    std::string line;
                    std::string filename = entry.path().filename().string();

                    while (std::getline(file, line)) {
                        std::smatch match;
                        if (std::regex_search(line, match, includeRegex)) {
                            deps[filename].push_back(match[1].str());
                        }
                    }
                }
            }
        }
    };

    scanDir("src");
    scanDir("include");

    std::cout << "\nForge Module Dependency Graph\n";
    std::cout << "--------------------------------------------\n\n";

    if (dotFormat) {
        std::cout << "digraph ForgeDependencies {\n";
        std::cout << "  node [shape=box, fontname=\"Courier\"];\n";
        for (const auto& [file, includes] : deps) {
            for (const auto& inc : includes) {
                std::cout << "  \"" << file << "\" -> \"" << inc << "\";\n";
            }
        }
        std::cout << "}\n\n";
        return 0;
    }

    if (deps.empty()) {
        std::cout << "  [OK] No explicit project dependencies found in 'src/' or 'include/'.\n\n";
        return 0;
    }

    for (const auto& [file, includes] : deps) {
        std::cout << "  " << file << "\n";
        for (size_t i = 0; i < includes.size(); ++i) {
            bool isLast = (i + 1 == includes.size());
            std::cout << "    " << (isLast ? "`-- " : "|-- ") << includes[i] << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}

} // namespace forge