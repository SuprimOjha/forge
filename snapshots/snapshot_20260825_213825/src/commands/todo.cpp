#include "forge/commands/todo.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

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

void printTodoHelp() {
    std::cout
        << "\n"
        << "Forge Technical Debt & TODO Comment Extractor\n\n"

        << "Usage:\n"
        << "  forge todo [options]\n\n"

        << "Options:\n"
        << "  -d, --dir <path>    Target directory to scan [default: .]\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge todo\n"
        << "  forge todo -d src/\n";
}

struct TodoItem {
    std::string file;
    size_t lineNum;
    std::string tag;
    std::string comment;
};

bool isSourceFile(const fs::path& p) {
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext == ".cpp" || ext == ".hpp" || ext == ".h" || ext == ".c" || ext == ".py" || ext == ".js" || ext == ".ts" || ext == ".rs");
}

} // anonymous namespace

int runTodo(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path targetDir = ".";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printTodoHelp();
            return 0;
        } else if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            targetDir = argv[++i];
        }
    }

    std::cout << "\nForge Technical Debt Auditor\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Workspace Target : " << fs::absolute(targetDir).string() << "\n\n";

    if (!fs::exists(targetDir)) {
        std::cerr << "  [!] Directory not found: " << targetDir.string() << "\n\n";
        return 1;
    }

    std::vector<TodoItem> items;
    const std::vector<std::string> tags = {"TODO", "FIXME", "HACK", "XXX", "NOTE"};

    for (const auto& entry : fs::recursive_directory_iterator(targetDir, fs::directory_options::skip_permission_denied)) {
        if (entry.is_regular_file() && isSourceFile(entry.path())) {
            std::ifstream file(entry.path());
            if (!file.is_open()) continue;

            std::string line;
            size_t lineNum = 0;

            while (std::getline(file, line)) {
                lineNum++;
                for (const auto& tag : tags) {
                    size_t pos = line.find(tag);
                    if (pos != std::string::npos) {
                        std::string comment = line.substr(pos + tag.length());
                        // Trim leading punctuation or spaces
                        size_t start = comment.find_first_not_of(" :#-");
                        if (start != std::string::npos) {
                            comment = comment.substr(start);
                        }
                        
                        std::string relPath = fs::relative(entry.path(), targetDir).string();
                        std::replace(relPath.begin(), relPath.end(), '\\', '/');
                        items.push_back({relPath, lineNum, tag, comment});
                        break;
                    }
                }
            }
        }
    }

    if (items.empty()) {
        std::cout << "  [OK] No technical debt annotations (TODO, FIXME, HACK) found!\n\n";
        return 0;
    }

    for (const auto& item : items) {
        std::cout << "  [" << item.tag << "] " << item.file << ":" << item.lineNum << "\n";
        std::cout << "      └── " << item.comment << "\n\n";
    }

    std::cout << "--------------------------------------------\n";
    std::cout << "  Total Technical Debt Items Found: " << items.size() << "\n\n";

    return 0;
}

} // namespace forge