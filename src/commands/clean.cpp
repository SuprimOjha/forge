#include "forge/commands/clean.hpp"

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cctype>

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

struct CleanTarget {
    fs::path path;
    bool isDirectory;
    std::uintmax_t sizeBytes;
};

std::string formatSize(std::uintmax_t bytes) {
    if (bytes < 1024) {
        return std::to_string(bytes) + " B";
    } else if (bytes < 1024 * 1024) {
        double kb = static_cast<double>(bytes) / 1024.0;
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << kb << " KB";
        return ss.str();
    } else {
        double mb = static_cast<double>(bytes) / (1024.0 * 1024.0);
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << mb << " MB";
        return ss.str();
    }
}

std::uintmax_t getDirectorySize(const fs::path& dirPath) {
    std::uintmax_t totalSize = 0;
    std::error_code ec;
    for (const auto& entry : fs::recursive_directory_iterator(dirPath, ec)) {
        if (fs::is_regular_file(entry, ec)) {
            totalSize += entry.file_size(ec);
        }
    }
    return totalSize;
}

} // anonymous namespace

int runClean(const std::vector<std::string>& args) {
    enableConsoleEncoding();

    bool dryRun = false;
    bool deepClean = false;
    bool autoConfirm = false;

    for (const auto& arg : args) {
        if (arg == "--dry-run" || arg == "-d") {
            dryRun = true;
        } else if (arg == "--deep") {
            deepClean = true;
        } else if (arg == "-y" || arg == "--yes") {
            autoConfirm = true;
        }
    }

    std::cout << "\nForge Workspace Cleaner\n";
    std::cout << "--------------------------------------------\n\n";

    std::vector<CleanTarget> targets;
    std::uintmax_t totalBytesFreed = 0;

    // Target directories to scan
    std::vector<std::string> targetDirs = {"build", "bin", "obj", "out", "dist", ".cache"};
    if (deepClean) {
        targetDirs.push_back("node_modules");
        targetDirs.push_back(".ninja_log");
    }

    for (const auto& dir : targetDirs) {
        if (fs::exists(dir) && fs::is_directory(dir)) {
            std::uintmax_t dirSize = getDirectorySize(dir);
            targets.push_back({fs::path(dir), true, dirSize});
            totalBytesFreed += dirSize;
        }
    }

    // Single artifact extensions to target in root workspace
    std::vector<std::string> artifactExtensions = {".exe", ".pdb", ".ilk", ".o", ".obj", ".a", ".so", ".dylib"};
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            if (std::find(artifactExtensions.begin(), artifactExtensions.end(), ext) != artifactExtensions.end()) {
                std::error_code ec;
                std::uintmax_t fileSize = entry.file_size(ec);
                targets.push_back({entry.path(), false, fileSize});
                totalBytesFreed += fileSize;
            }
        }
    }

    if (targets.empty()) {
        std::cout << "  [OK] Workspace is already clean. No build artifacts found.\n\n";
        return 0;
    }

    // Display list of candidates for deletion with sizes
    std::cout << "  Identified Build Artifacts to Clean:\n";
    for (const auto& item : targets) {
        std::cout << "    • [" << (item.isDirectory ? "DIR " : "FILE") << "] "
                  << std::left << std::setw(30) << item.path.string()
                  << " (" << formatSize(item.sizeBytes) << ")\n";
    }

    std::cout << "\n  Total size to free: " << formatSize(totalBytesFreed) << "\n\n";

    if (dryRun) {
        std::cout << "  [DRY-RUN COMPLETE] Found " << targets.size() << " item(s). No files were deleted.\n\n";
        return 0;
    }

    // Interactive prompt confirmation
    if (!autoConfirm) {
        std::cout << "  Are you sure you want to delete these " << targets.size() << " target(s)? [y/N]: ";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice.empty() || (std::tolower(choice[0]) != 'y')) {
            std::cout << "\n  [i] Clean operation canceled by user.\n\n";
            return 0;
        }
        std::cout << "\n";
    }

    // Execute deletion sequence
    int removedCount = 0;
    std::uintmax_t actualBytesFreed = 0;

    for (const auto& item : targets) {
        std::error_code ec;
        if (item.isDirectory) {
            std::uintmax_t removedItems = fs::remove_all(item.path, ec);
            if (!ec) {
                std::cout << "  [+] Removed directory: ./" << item.path.string()
                          << " (" << removedItems << " items, " << formatSize(item.sizeBytes) << ")\n";
                ++removedCount;
                actualBytesFreed += item.sizeBytes;
            } else {
                std::cout << "  [!] Failed to remove directory ./" << item.path.string() << ": " << ec.message() << "\n";
            }
        } else {
            if (fs::remove(item.path, ec)) {
                std::cout << "  [+] Removed artifact file: ./" << item.path.string()
                          << " (" << formatSize(item.sizeBytes) << ")\n";
                ++removedCount;
                actualBytesFreed += item.sizeBytes;
            } else {
                std::cout << "  [!] Failed to remove file ./" << item.path.string() << ": " << ec.message() << "\n";
            }
        }
    }

    std::cout << "\n  [OK] Cleaned " << removedCount << " target(s) successfully. Freed " << formatSize(actualBytesFreed) << ".\n\n";

    return 0;
}

} // namespace forge