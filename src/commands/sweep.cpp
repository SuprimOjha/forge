#include "forge/commands/sweep.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <set>

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

void printSweepHelp() {
    std::cout
        << "\n"
        << "Forge Smart Workspace Junk Sweeper\n\n"

        << "Usage:\n"
        << "  forge sweep [options]\n\n"

        << "Options:\n"
        << "  --dry-run               Preview junk files without deleting\n"
        << "  -f, --force             Delete junk files without confirmation\n"
        << "  -h, --help              Show this help message\n";
}

bool isJunkFile(const fs::path& p) {
    std::string ext = p.extension().string();
    std::string name = p.filename().string();

    std::set<std::string> junkExts = {
        ".tmp", ".temp", ".log", ".bak", ".swp", ".o", ".obj", ".ilk", ".pdb", ".tlog"
    };

    std::set<std::string> junkNames = {
        ".DS_Store", "Thumbs.db", "desktop.ini"
    };

    return junkExts.count(ext) > 0 || junkNames.count(name) > 0;
}

bool isIgnoredDir(const std::string& dirName) {
    return (dirName == ".git" || dirName == "node_modules" || dirName == ".vs");
}

} // anonymous namespace

int runSweep(int argc, char* argv[]) {
    enableConsoleEncoding();

    bool dryRun = false;
    bool force = false;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--dry-run") {
            dryRun = true;
        } else if (arg == "-f" || arg == "--force") {
            force = true;
        } else if (arg == "-h" || arg == "--help") {
            printSweepHelp();
            return 0;
        }
    }

    std::cout << "\nForge Workspace Junk Sweeper\n";
    std::cout << "--------------------------------------------\n\n";

    std::vector<fs::path> junkFiles;
    uint64_t totalBytes = 0;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(fs::current_path())) {
            if (entry.is_directory()) {
                if (isIgnoredDir(entry.path().filename().string())) {
                    continue;
                }
            } else if (entry.is_regular_file()) {
                if (isJunkFile(entry.path())) {
                    junkFiles.push_back(entry.path());
                    totalBytes += entry.file_size();
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "  [!] Error scanning filesystem: " << e.what() << "\n\n";
        return 1;
    }

    if (junkFiles.empty()) {
        std::cout << "  [OK] Workspace is completely clean! No junk files detected.\n\n";
        return 0;
    }

    std::cout << "  Detected " << junkFiles.size() << " junk file(s) (" << (totalBytes / 1024) << " KB):\n\n";
    for (const auto& file : junkFiles) {
        std::cout << "    - " << fs::relative(file).string() << "\n";
    }
    std::cout << "\n";

    if (dryRun) {
        std::cout << "  [i] Dry-run mode enabled. No files were deleted.\n\n";
        return 0;
    }

    if (!force) {
        std::cout << "  Perform sweep and delete these files? [y/N]: ";
        std::string response;
        std::cin >> response;
        if (response != "y" && response != "Y") {
            std::cout << "  Sweep cancelled.\n\n";
            return 0;
        }
    }

    std::size_t deletedCount = 0;
    for (const auto& file : junkFiles) {
        std::error_code ec;
        if (fs::remove(file, ec)) {
            deletedCount++;
        }
    }

    std::cout << "  [+] Swept and removed " << deletedCount << " file(s) (" << (totalBytes / 1024) << " KB freed).\n\n";
    return 0;
}

} // namespace forge