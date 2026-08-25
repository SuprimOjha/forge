#include "forge/commands/deps.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <set>
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

void printDepsHelp() {
    std::cout
        << "\n"
        << "Forge Module Dependency & Coupling Analyzer\n\n"

        << "Usage:\n"
        << "  forge deps [options]\n\n"

        << "Options:\n"
        << "  -d, --dir <path>    Target directory to analyze [default: .]\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge deps\n"
        << "  forge deps -d src/\n";
}

bool isHeaderFile(const fs::path& p) {
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext == ".hpp" || ext == ".h");
}

bool isSourceFile(const fs::path& p) {
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext == ".cpp" || ext == ".c");
}

} // anonymous namespace

int runDeps(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path targetDir = ".";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printDepsHelp();
            return 0;
        } else if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            targetDir = argv[++i];
        }
    }

    std::cout << "\nForge Dependency & Module Coupling Analyzer\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Analyzing Path : " << fs::absolute(targetDir).string() << "\n\n";

    std::set<std::string> declaredHeaders;
    std::map<std::string, size_t> includeCounts;

    try {
        // Collect headers
        for (const auto& entry : fs::recursive_directory_iterator(targetDir, fs::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file() && isHeaderFile(entry.path())) {
                std::string headerName = entry.path().filename().string();
                declaredHeaders.insert(headerName);
            }
        }

        // Scan sources and headers for includes
        for (const auto& entry : fs::recursive_directory_iterator(targetDir, fs::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file() && (isSourceFile(entry.path()) || isHeaderFile(entry.path()))) {
                std::ifstream file(entry.path());
                if (!file.is_open()) continue;

                std::string line;
                while (std::getline(file, line)) {
                    size_t incPos = line.find("#include");
                    if (incPos != std::string::npos) {
                        size_t firstQuote = line.find('"', incPos);
                        size_t secondQuote = line.find('"', firstQuote + 1);
                        if (firstQuote != std::string::npos && secondQuote != std::string::npos) {
                            std::string incPath = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                            fs::path p(incPath);
                            includeCounts[p.filename().string()]++;
                        }
                    }
                }
            }
        }
    } catch (...) {}

    std::cout << "  [i] Header Files Declared: " << declaredHeaders.size() << "\n";
    
    size_t referencedCount = 0;
    for (const auto& header : declaredHeaders) {
        if (includeCounts.find(header) != includeCounts.end()) {
            referencedCount++;
        }
    }

    std::cout << "  [i] Headers Referenced  : " << referencedCount << " / " << declaredHeaders.size() << "\n\n";

    std::cout << "  Top Included Headers:\n";
    std::vector<std::pair<std::string, size_t>> sortedIncludes(includeCounts.begin(), includeCounts.end());
    std::sort(sortedIncludes.begin(), sortedIncludes.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    size_t limit = std::min<size_t>(5, sortedIncludes.size());
    for (size_t i = 0; i < limit; ++i) {
        std::cout << "      • " << sortedIncludes[i].first << " (" << sortedIncludes[i].second << " references)\n";
    }

    std::cout << "\n--------------------------------------------\n";
    std::cout << "  Module Coupling Analysis Complete.\n\n";

    return 0;
}

} // namespace forge