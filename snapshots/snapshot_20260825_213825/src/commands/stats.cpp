#include "forge/commands/stats.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
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

struct FileMetrics {
    std::size_t totalFiles = 0;
    std::size_t totalBytes = 0;
    std::size_t totalLines = 0;
    std::size_t codeLines = 0;
    std::size_t commentLines = 0;
    std::size_t blankLines = 0;
};

void printStatsHelp() {
    std::cout
        << "\n"
        << "Forge Workspace Metrics Engine\n\n"

        << "Usage:\n"
        << "  forge stats [options]\n\n"

        << "Options:\n"
        << "  -h, --help              Show this help message\n";
}

bool isIgnoredDir(const std::string& dirName) {
    return (dirName == "build" || dirName == ".git" || dirName == "node_modules" || dirName == "dist" || dirName == ".vs");
}

} // anonymous namespace

int runStats(int argc, char* argv[]) {
    enableConsoleEncoding();

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printStatsHelp();
            return 0;
        }
    }

    std::map<std::string, FileMetrics> extStats;
    FileMetrics overall;

    std::vector<std::string> scanDirs = {"src", "include", "lib", "tests"};

    for (const auto& scanDir : scanDirs) {
        if (!fs::exists(scanDir)) continue;

        for (const auto& entry : fs::recursive_directory_iterator(scanDir)) {
            if (entry.is_directory()) {
                if (isIgnoredDir(entry.path().filename().string())) {
                    continue;
                }
            } else if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext.empty()) ext = "[no ext]";

                std::ifstream file(entry.path());
                if (!file.is_open()) continue;

                std::size_t bytes = entry.file_size();
                std::string line;
                std::size_t lines = 0;
                std::size_t code = 0;
                std::size_t comments = 0;
                std::size_t blanks = 0;

                while (std::getline(file, line)) {
                    lines++;
                    std::string trimmed = line;
                    trimmed.erase(0, trimmed.find_first_not_of(" \t\r\n"));
                    trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

                    if (trimmed.empty()) {
                        blanks++;
                    } else if (trimmed.rfind("//", 0) == 0 || trimmed.rfind("/*", 0) == 0 || trimmed.rfind("#", 0) == 0) {
                        comments++;
                    } else {
                        code++;
                    }
                }

                auto& m = extStats[ext];
                m.totalFiles++;
                m.totalBytes += bytes;
                m.totalLines += lines;
                m.codeLines += code;
                m.commentLines += comments;
                m.blankLines += blanks;

                overall.totalFiles++;
                overall.totalBytes += bytes;
                overall.totalLines += lines;
                overall.codeLines += code;
                overall.commentLines += comments;
                overall.blankLines += blanks;
            }
        }
    }

    std::cout << "\nForge Workspace Code Metrics\n";
    std::cout << "--------------------------------------------------------\n\n";

    if (overall.totalFiles == 0) {
        std::cout << "  [WARN] No source files found in 'src/', 'include/', 'lib/', or 'tests/'.\n\n";
        return 0;
    }

    std::cout << std::left 
              << std::setw(12) << "Extension"
              << std::setw(10) << "Files"
              << std::setw(12) << "Code Lines"
              << std::setw(12) << "Comments"
              << std::setw(10) << "Blanks"
              << std::setw(12) << "Total Lines"
              << "\n";
    std::cout << "--------------------------------------------------------\n";

    for (const auto& [ext, m] : extStats) {
        std::cout << std::left
                  << std::setw(12) << ext
                  << std::setw(10) << m.totalFiles
                  << std::setw(12) << m.codeLines
                  << std::setw(12) << m.commentLines
                  << std::setw(10) << m.blankLines
                  << std::setw(12) << m.totalLines
                  << "\n";
    }

    std::cout << "--------------------------------------------------------\n";
    std::cout << std::left
              << std::setw(12) << "TOTAL"
              << std::setw(10) << overall.totalFiles
              << std::setw(12) << overall.codeLines
              << std::setw(12) << overall.commentLines
              << std::setw(10) << overall.blankLines
              << std::setw(12) << overall.totalLines
              << "\n\n";

    std::cout << "  Workspace Size: " << (overall.totalBytes / 1024) << " KB (" << overall.totalBytes << " bytes)\n\n";

    return 0;
}

} // namespace forge