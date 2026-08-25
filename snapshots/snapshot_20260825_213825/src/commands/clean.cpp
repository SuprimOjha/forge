#include "forge/commands/clean.hpp"

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
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

} // anonymous namespace

int runClean(const std::vector<std::string>& args) {
    enableConsoleEncoding();

    bool dryRun = false;
    bool deepClean = false;

    for (const auto& arg : args) {
        if (arg == "--dry-run" || arg == "-d") {
            dryRun = true;
        } else if (arg == "--deep") {
            deepClean = true;
        }
    }

    std::cout << "\nForge Workspace Cleaner\n";
    std::cout << "--------------------------------------------\n\n";

    // Target directories to remove
    std::vector<std::string> targetDirs = {"build", "bin", "obj", "out", "dist", ".cache"};
    if (deepClean) {
        targetDirs.push_back("node_modules");
        targetDirs.push_back(".ninja_log");
    }

    int removedCount = 0;
    std::uintmax_t bytesFreed = 0;

    for (const auto& dir : targetDirs) {
        if (fs::exists(dir) && fs::is_directory(dir)) {
            if (dryRun) {
                std::cout << "  [DRY-RUN] Would remove directory: ./" << dir << "\n";
                ++removedCount;
            } else {
                std::error_code ec;
                for (const auto& p : fs::recursive_directory_iterator(dir, ec)) {
                    if (fs::is_regular_file(p, ec)) {
                        bytesFreed += fs::file_size(p, ec);
                    }
                }

                std::uintmax_t items = fs::remove_all(dir, ec);
                if (!ec) {
                    std::cout << "  [+] Removed directory: ./" << dir << " (" << items << " items)\n";
                    ++removedCount;
                } else {
                    std::cout << "  [!] Failed to remove directory ./" << dir << ": " << ec.message() << "\n";
                }
            }
        }
    }

    // Single artifact extensions to target in root
    std::vector<std::string> artifactExtensions = {".exe", ".pdb", ".ilk", ".o", ".obj", ".a", ".so", ".dylib"};
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            if (std::find(artifactExtensions.begin(), artifactExtensions.end(), ext) != artifactExtensions.end()) {
                std::string fileName = entry.path().filename().string();
                if (dryRun) {
                    std::cout << "  [DRY-RUN] Would remove artifact file: ./" << fileName << "\n";
                    ++removedCount;
                } else {
                    std::error_code ec;
                    bytesFreed += entry.file_size(ec);
                    if (fs::remove(entry.path(), ec)) {
                        std::cout << "  [+] Removed artifact file: ./" << fileName << "\n";
                        ++removedCount;
                    }
                }
            }
        }
    }

    if (removedCount == 0) {
        std::cout << "  [OK] Workspace is already clean. No build artifacts found.\n\n";
    } else {
        if (dryRun) {
            std::cout << "\n  [DRY-RUN COMPLETE] Identified " << removedCount << " cleanable item(s).\n\n";
        } else {
            std::cout << "\n  [OK] Cleaned " << removedCount << " target(s) successfully.";
            if (bytesFreed > 0) {
                std::cout << " Freed ~" << (bytesFreed / (1024 * 1024)) << " MB.";
            }
            std::cout << "\n\n";
        }
    }

    return 0;
}

} // namespace forge