#include "forge/commands/graph.hpp"

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

void printGraphHelp() {
    std::cout
        << "\n"
        << "Forge Dependency Tree & Graph Visualizer\n\n"

        << "Usage:\n"
        << "  forge graph [options]\n\n"

        << "Options:\n"
        << "  --dot                   Output graph in Graphviz DOT format\n"
        << "  -dir, --directory <dir> Target source directory [default: include/]\n"
        << "  -h, --help              Show this help message\n";
}

std::set<std::string> extractIncludes(const fs::path& filePath) {
    std::set<std::string> includes;
    std::ifstream file(filePath);
    if (!file.is_open()) return includes;

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find("#include");
        if (pos != std::string::npos) {
            size_t startQuote = line.find_first_of("\"<", pos);
            size_t endQuote = line.find_last_of("\">");
            if (startQuote != std::string::npos && endQuote != std::string::npos && endQuote > startQuote) {
                std::string inc = line.substr(startQuote + 1, endQuote - startQuote - 1);
                includes.insert(inc);
            }
        }
    }
    return includes;
}

} // anonymous namespace

int runGraph(int argc, char* argv[]) {
    enableConsoleEncoding();

    bool dotFormat = false;
    fs::path targetDir = "include";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--dot") {
            dotFormat = true;
        } else if ((arg == "-dir" || arg == "--directory") && i + 1 < argc) {
            targetDir = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            printGraphHelp();
            return 0;
        }
    }

    if (!fs::exists(targetDir)) {
        targetDir = "src"; // fallback to src if include does not exist
    }

    if (!fs::exists(targetDir)) {
        std::cout << "\n  [!] Neither 'include' nor 'src' directory found in workspace.\n\n";
        return 1;
    }

    std::map<std::string, std::set<std::string>> graph;

    for (const auto& entry : fs::recursive_directory_iterator(targetDir)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".hpp" || ext == ".h" || ext == ".cpp") {
                std::string relPath = fs::relative(entry.path(), targetDir).string();
                std::replace(relPath.begin(), relPath.end(), '\\', '/');
                graph[relPath] = extractIncludes(entry.path());
            }
        }
    }

    std::cout << "\nForge Dependency Graph (" << targetDir.string() << ")\n";
    std::cout << "--------------------------------------------\n\n";

    if (dotFormat) {
        std::cout << "digraph ForgeDependencies {\n";
        std::cout << "  node [shape=box, fontname=\"Courier\"];\n";
        for (const auto& [node, edges] : graph) {
            for (const auto& edge : edges) {
                std::cout << "  \"" << node << "\" -> \"" << edge << "\";\n";
            }
        }
        std::cout << "}\n\n";
    } else {
        for (const auto& [node, edges] : graph) {
            std::cout << "  [" << node << "]\n";
            if (edges.empty()) {
                std::cout << "      └── (no local includes)\n";
            } else {
                size_t count = 0;
                for (const auto& edge : edges) {
                    count++;
                    bool isLast = (count == edges.size());
                    std::cout << "      " << (isLast ? "└── " : "├── ") << edge << "\n";
                }
            }
            std::cout << "\n";
        }
    }

    return 0;
}

} // namespace forge