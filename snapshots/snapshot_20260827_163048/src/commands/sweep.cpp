#include "forge/commands/sweep.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <numeric>
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

void printSweepHelp() {
    std::cout
        << "\n"
        << "Forge Workspace Disk Space & Cache Cleaner\n\n"

        << "Usage:\n"
        << "  forge sweep [options]\n\n"

        << "Options:\n"
        << "  --purge             Delete identified build directories and caches\n"
        << "  -d, --dir <path>    Target directory to analyze [default: .]\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge sweep\n"
        << "  forge sweep --purge\n";
}

uint64_t getDirectorySize(const fs::path& dirPath) {
    uint64_t totalSize = 0;
    if (!fs::exists(dirPath)) return 0;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(dirPath, fs::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file() && !entry.is_symlink()) {
                totalSize += entry.file_size();
            }
        }
    } catch (...) {}
    return totalSize;
}

std::string formatSize(uint64_t bytes) {
    double size = static_cast<double>(bytes);
    const char* units[] = {"B", "KB", "MB", "GB"};
    int unitIndex = 0;
    while (size >= 1024.0 && unitIndex < 3) {
        size /= 1024.0;
        unitIndex++;
    }
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << size << " " << units[unitIndex];
    return ss.str();
}

} // anonymous namespace

int runSweep(int argc, char* argv[]) {
    enableConsoleEncoding();

    bool purge = false;
    fs::path targetDir = ".";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--purge") {
            purge = true;
        } else if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            targetDir = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            printSweepHelp();
            return 0;
        }
    }

    std::cout << "\nForge Disk Space & Cache Cleaner\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Workspace Target : " << fs::absolute(targetDir).string() << "\n\n";

    std::vector<std::string> cleanTargets = {"build", "dist", ".cache", "node_modules", "out", "bin", "obj"};
    uint64_t totalReclaimable = 0;
    std::vector<std::pair<fs::path, uint64_t>> foundDirs;

    for (const auto& target : cleanTargets) {
        fs::path p = targetDir / target;
        if (fs::exists(p) && fs::is_directory(p)) {
            uint64_t sz = getDirectorySize(p);
            foundDirs.push_back({p, sz});
            totalReclaimable += sz;
        }
    }

    if (foundDirs.empty()) {
        std::cout << "  [OK] Workspace is clean! No unnecessary build/cache directories found.\n\n";
        return 0;
    }

    std::cout << std::left << std::setw(30) << "Directory / Cache" << std::setw(15) << "Size" << "\n";
    std::cout << "--------------------------------------------\n";

    for (const auto& [path, sz] : foundDirs) {
        std::cout << std::left << std::setw(30) << path.filename().string() << std::setw(15) << formatSize(sz) << "\n";
    }

    std::cout << "--------------------------------------------\n";
    std::cout << "  Total Reclaimable Space: " << formatSize(totalReclaimable) << "\n\n";

    if (purge) {
        std::cout << "  [!] Purging targets...\n";
        for (const auto& [path, sz] : foundDirs) {
            try {
                fs::remove_all(path);
                std::cout << "  [-] Removed: " << path.filename().string() << "\n";
            } catch (const std::exception& e) {
                std::cerr << "  [!] Failed to remove " << path.string() << ": " << e.what() << "\n";
            }
        }
        std::cout << "\n  [OK] Disk sweep completed successfully!\n\n";
    } else {
        std::cout << "  [i] Dry-run mode. Run with '--purge' to delete these directories.\n\n";
    }

    return 0;
}

} // namespace forge