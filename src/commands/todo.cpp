#include "forge/commands/todo.hpp"

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

struct TodoItem {
    std::string file;
    std::size_t lineNum;
    std::string tag;
    std::string text;
};

void printTodoHelp() {
    std::cout
        << "\n"
        << "Forge Technical Debt & TODO Scanner\n\n"

        << "Usage:\n"
        << "  forge todo [options]\n\n"

        << "Options:\n"
        << "  -h, --help              Show this help message\n";
}

bool isIgnoredDir(const std::string& dirName) {
    return (dirName == "build" || dirName == ".git" || dirName == "node_modules" || dirName == "dist" || dirName == ".vs");
}

} // anonymous namespace

int runTodo(int argc, char* argv[]) {
    enableConsoleEncoding();

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printTodoHelp();
            return 0;
        }
    }

    std::vector<std::string> tags = {"TODO", "FIXME", "HACK", "BUG", "OPTIMIZE", "XXX"};
    std::map<std::string, std::vector<TodoItem>> groupedItems;
    std::size_t totalCount = 0;

    std::vector<std::string> scanDirs = {"src", "include", "lib", "tests"};

    for (const auto& scanDir : scanDirs) {
        if (!fs::exists(scanDir)) continue;

        for (const auto& entry : fs::recursive_directory_iterator(scanDir)) {
            if (entry.is_directory()) {
                if (isIgnoredDir(entry.path().filename().string())) {
                    continue;
                }
            } else if (entry.is_regular_file()) {
                std::ifstream file(entry.path());
                if (!file.is_open()) continue;

                std::string line;
                std::size_t currentLine = 0;

                while (std::getline(file, line)) {
                    currentLine++;

                    for (const auto& tag : tags) {
                        size_t pos = line.find(tag);
                        if (pos != std::string::npos) {
                            std::string snippet = line.substr(pos);
                            // Clean up trailing whitespace
                            snippet.erase(snippet.find_last_not_of(" \t\r\n") + 1);

                            groupedItems[tag].push_back({
                                entry.path().string(),
                                currentLine,
                                tag,
                                snippet
                            });
                            totalCount++;
                            break;
                        }
                    }
                }
            }
        }
    }

    std::cout << "\nForge Technical Debt & TODO Scanner\n";
    std::cout << "--------------------------------------------\n\n";

    if (totalCount == 0) {
        std::cout << "  [OK] No TODOs, FIXMEs, or technical debt markers found!\n\n";
        return 0;
    }

    std::cout << "  Found " << totalCount << " technical debt marker(s):\n\n";

    for (const auto& tag : tags) {
        if (groupedItems.find(tag) == groupedItems.end()) continue;

        const auto& items = groupedItems[tag];
        std::cout << "  [" << tag << "] (" << items.size() << ")\n";

        for (const auto& item : items) {
            std::cout << "    - " << item.file << ":" << item.lineNum << "\n"
                      << "      " << item.text << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}

} // namespace forge