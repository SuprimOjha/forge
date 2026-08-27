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
        << "  --dot <path>       Export graph in Graphviz DOT format [default: dependency_graph.dot]\n"
        << "  -d, --dir <path>   Target source directory to analyze [default: src/]\n"
        << "  -h, --help         Show this help message\n\n"

        << "Examples:\n"
        << "  forge graph\n"
        << "  forge graph --dot graph.dot\n";
}

bool isSourceFile(const fs::path& p) {
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext == ".cpp" || ext == ".hpp" || ext == ".h");
}

std::string extractIncludeName(const std::string& line) {
    size_t start = line.find('"');
    if (start != std::string::npos) {
        size_t end = line.find('"', start + 1);
        if (end != std::string::npos) {
            return line.substr(start + 1, end - start - 1);
        }
    }
    start = line.find('<');
    if (start != std::string::npos) {
        size_t end = line.find('>', start + 1);
        if (end != std::string::npos) {
            return line.substr(start + 1, end - start - 1);
        }
    }
    return "";
}

} // anonymous namespace

int runGraph(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path targetDir = "src";
    fs::path dotPath = "dependency_graph.dot";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printGraphHelp();
            return 0;
        } else if (arg == "--dot" && i + 1 < argc) {
            dotPath = argv[++i];
        } else if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            targetDir = argv[++i];
        }
    }

    if (!fs::exists(targetDir)) {
        targetDir = ".";
    }

    std::cout << "\nForge Dependency Graph Visualizer\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Analyzing Path : " << fs::absolute(targetDir).string() << "\n\n";

    std::map<std::string, std::set<std::string>> graph;
    size_t edgeCount = 0;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(targetDir, fs::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file() && isSourceFile(entry.path())) {
                std::string filename = entry.path().filename().string();
                std::ifstream file(entry.path());
                if (!file.is_open()) continue;

                std::string line;
                while (std::getline(file, line)) {
                    if (line.find("#include") != std::string::npos) {
                        std::string inc = extractIncludeName(line);
                        if (!inc.empty() && inc.find("forge/") != std::string::npos) {
                            fs::path incPath(inc);
                            graph[filename].insert(incPath.filename().string());
                            edgeCount++;
                        }
                    }
                }
            }
        }
    } catch (...) {}

    // Terminal Tree Output
    std::cout << "  Module Inclusions:\n";
    for (const auto& [node, edges] : graph) {
        std::cout << "  📄 " << node << "\n";
        for (const auto& target : edges) {
            std::cout << "      └──> " << target << "\n";
        }
    }

    // Generate Graphviz DOT file
    std::ofstream dotFile(dotPath);
    if (dotFile.is_open()) {
        dotFile << "digraph ForgeDependencies {\n";
        dotFile << "  rankdir=LR;\n";
        dotFile << "  node [shape=box, style=rounded, fontname=\"Courier\"];\n";
        for (const auto& [node, edges] : graph) {
            for (const auto& target : edges) {
                dotFile << "  \"" << node << "\" -> \"" << target << "\";\n";
            }
        }
        dotFile << "}\n";
        dotFile.close();
    }

    std::cout << "\n--------------------------------------------\n";
    std::cout << "  Graph Analysis Complete: 🟢 SUCCESS\n";
    std::cout << "  Total Edges       : " << edgeCount << " inclusion link(s)\n";
    std::cout << "  DOT Export Path   : " << fs::absolute(dotPath).string() << "\n\n";

    return 0;
}

} // namespace forge