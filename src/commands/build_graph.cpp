#include "forge/commands/build_graph.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <thread>

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

void printBuildGraphHelp() {
    std::cout
        << "\n"
        << "Forge Build Speed & Dependency Bottleneck Analyzer\n\n"

        << "Usage:\n"
        << "  forge build-graph [options]\n\n"

        << "Options:\n"
        << "  -d, --dir <path>    Target source directory [default: src/]\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge build-graph\n"
        << "  forge build-graph -d src/\n";
}

bool isSourceFile(const fs::path& p) {
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext == ".cpp" || ext == ".c");
}

size_t countIncludes(const fs::path& p) {
    std::ifstream file(p);
    if (!file.is_open()) return 0;

    size_t count = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("#include") != std::string::npos) {
            count++;
        }
    }
    return count;
}

} // anonymous namespace

int runBuildGraph(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path targetDir = "src";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printBuildGraphHelp();
            return 0;
        } else if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            targetDir = argv[++i];
        }
    }

    if (!fs::exists(targetDir)) {
        targetDir = ".";
    }

    std::cout << "\nForge Build Graph & Bottleneck Analyzer\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Analyzing Source Path : " << fs::absolute(targetDir).string() << "\n\n";

    size_t totalTranslationUnits = 0;
    size_t totalHeaderRefs = 0;
    std::vector<std::pair<std::string, size_t>> heavyUnits;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(targetDir, fs::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file() && isSourceFile(entry.path())) {
                totalTranslationUnits++;
                size_t includes = countIncludes(entry.path());
                totalHeaderRefs += includes;
                heavyUnits.push_back({entry.path().filename().string(), includes});
            }
        }
    } catch (...) {}

    std::sort(heavyUnits.begin(), heavyUnits.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    unsigned int hwCores = std::thread::hardware_concurrency();
    if (hwCores == 0) hwCores = 4;

    std::cout << "  Build Topology Stats:\n";
    std::cout << "    • Translation Units (TUs) : " << totalTranslationUnits << "\n";
    std::cout << "    • Total Includes Parsed   : " << totalHeaderRefs << "\n";
    std::cout << "    • Available CPU Cores     : " << hwCores << "\n\n";

    std::cout << "  Top Heavy Compilation Units (Header Fan-out):\n";
    size_t topLimit = std::min(heavyUnits.size(), size_t(5));
    for (size_t i = 0; i < topLimit; ++i) {
        std::cout << "    • " << heavyUnits[i].first << " (" << heavyUnits[i].second << " includes)\n";
    }

    std::cout << "\n--------------------------------------------\n";
    std::cout << "  Recommended Parallel Flags:\n";
    std::cout << "    • MSVC / NMake : /MP" << hwCores << "\n";
    std::cout << "    • Make / CMake : cmake --build build -j" << hwCores << "\n\n";

    return 0;
}

} // namespace forge