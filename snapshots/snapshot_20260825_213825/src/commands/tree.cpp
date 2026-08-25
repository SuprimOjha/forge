#include "forge/commands/tree.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
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

void printTreeHelp() {
    std::cout
<< "\n"
<< "Forge Dependency Tree & Graph Visualizer\n\n"

<< "Usage:\n"
<< "  forge tree [options]\n\n"

<< "Options:\n"
<< "  -d, --dir <path>    Target directory to map [default: src/]\n"
<< "  -h, --help          Show this help message\n\n"

<< "Examples:\n"
<< "  forge tree\n"
<< "  forge tree -d include/\n";
}

bool isSourceFile(const fs::path& p) {
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext == ".cpp" || ext == ".hpp" || ext == ".h" || ext == ".c");
}

} // anonymous namespace

int runTree(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path targetDir = "src";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printTreeHelp();
            return 0;
        } else if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            targetDir = argv[++i];
        }
    }

 if (!fs::exists(targetDir)) {
        targetDir = ".";
    }

std::cout << "\nForge Workspace Module Dependency Tree\n";
std::cout << "--------------------------------------------\n";
std::cout << "  Root Path : " << fs::absolute(targetDir).string() << "\n\n";
std::map<std::string, std::vector<std::string>> depGraph;

for (const auto& entry : fs::recursive_directory_iterator(targetDir, fs::directory_options::skip_permission_denied)) {
        if (entry.is_regular_file() && isSourceFile(entry.path())) {
            std::ifstream file(entry.path());
            if (!file.is_open()) continue;

            std::string line;
            std::string relPath = fs::relative(entry.path(), targetDir).string();
            std::replace(relPath.begin(), relPath.end(), '\\', '/');

            while (std::getline(file, line)) {
                size_t incPos = line.find("#include");
                if (incPos != std::string::npos) {
                    size_t firstQuote = line.find('"', incPos);
                    size_t secondQuote = line.find('"', firstQuote + 1);
                    if (firstQuote != std::string::npos && secondQuote != std::string::npos) {
                        std::string includeHeader = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                        depGraph[relPath].push_back(includeHeader);
                    }
                }
            }
        }
    }

    if (depGraph.empty()) {
        std::cout << "  [i] No source dependency directives (#include \"...\") identified.\n\n";
        return 0;
    }

    for (const auto& [file, deps] : depGraph) {
        std::cout << "  📦 " << file << "\n";
        for (size_t i = 0; i < deps.size(); ++i) {
            bool isLast = (i == deps.size() - 1);
            std::cout << "      " << (isLast ? "└── " : "├── ") << deps[i] << "\n";
        }
        std::cout << "\n";
    }

 std::cout << "--------------------------------------------\n";
std::cout << "  Mapped Modules: " << depGraph.size() << "\n\n";

    return 0;
}

} // namespace forge