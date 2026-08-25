#include "forge/commands/search.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
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

void printSearchHelp() {
    std::cout
        << "\n"
        << "Forge Global Project Search & Symbol Finder\n\n"

        << "Usage:\n"
        << "  forge search <query> [options]\n\n"

        << "Options:\n"
        << "  -d, --dir <path>    Target directory to search [default: src/]\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge search runBuild\n"
        << "  forge search process_runner -d include/\n";
}

bool shouldSkipDir(const std::string& name) {
    return (name == "build" || name == ".git" || name == "dist" || name == "snapshots" || name == ".forge");
}

bool isTextFile(const fs::path& p) {
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext == ".cpp" || ext == ".hpp" || ext == ".h" || ext == ".c" || ext == ".txt" || ext == ".cmake" || ext == ".json" || ext == ".md");
}

} // anonymous namespace

int runSearch(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printSearchHelp();
        return 0;
    }

    std::string query = argv[2];
    if (query == "-h" || query == "--help") {
        printSearchHelp();
        return 0;
    }

    fs::path targetDir = "src";

    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            targetDir = argv[++i];
        }
    }

    if (!fs::exists(targetDir)) {
        targetDir = ".";
    }

    std::cout << "\nForge Global Workspace Search\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Query Target : \"" << query << "\"\n";
    std::cout << "  Search Path  : " << fs::absolute(targetDir).string() << "\n\n";

    size_t matchCount = 0;
    size_t fileCount = 0;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(targetDir, fs::directory_options::skip_permission_denied)) {
            if (entry.is_directory() && shouldSkipDir(entry.path().filename().string())) {
                continue;
            }

            if (entry.is_regular_file() && isTextFile(entry.path())) {
                std::ifstream file(entry.path());
                if (!file.is_open()) continue;

                std::string line;
                size_t lineNum = 0;
                bool fileHeaderPrinted = false;

                while (std::getline(file, line)) {
                    lineNum++;
                    if (line.find(query) != std::string::npos) {
                        if (!fileHeaderPrinted) {
                            std::string relPath = fs::relative(entry.path(), targetDir).string();
                            std::replace(relPath.begin(), relPath.end(), '\\', '/');
                            std::cout << "  📄 " << relPath << "\n";
                            fileHeaderPrinted = true;
                            fileCount++;
                        }
                        
                        // Trim leading spaces for display
                        size_t firstChar = line.find_first_not_of(" \t");
                        std::string trimmedLine = (firstChar != std::string::npos) ? line.substr(firstChar) : line;

                        std::cout << "      L" << lineNum << ": " << trimmedLine << "\n";
                        matchCount++;
                    }
                }

                if (fileHeaderPrinted) {
                    std::cout << "\n";
                }
            }
        }
    } catch (...) {}

    std::cout << "--------------------------------------------\n";
    std::cout << "  Matches Found : " << matchCount << " match(es) across " << fileCount << " file(s)\n\n";

    return 0;
}

} // namespace forge