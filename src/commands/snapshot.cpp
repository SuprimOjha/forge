#include "forge/commands/snapshot.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>

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

void printSnapshotHelp() {
    std::cout
        << "\n"
        << "Forge Workspace Snapshot & Archival Generator\n\n"

        << "Usage:\n"
        << "  forge snapshot [options]\n\n"

        << "Options:\n"
        << "  -o, --output <dir>  Output directory for snapshots [default: snapshots/]\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge snapshot\n"
        << "  forge snapshot -o backups/\n";
}

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

bool shouldIgnorePath(const fs::path& p) {
    std::string name = p.filename().string();
    return (name == "build" || name == "dist" || name == ".git" || name == ".forge" || name == "snapshots");
}

} // anonymous namespace

int runSnapshot(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path targetDir = "snapshots";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printSnapshotHelp();
            return 0;
        } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            targetDir = argv[++i];
        }
    }

    std::string timestamp = getCurrentTimestamp();
    fs::path snapshotFolder = targetDir / ("snapshot_" + timestamp);

    std::cout << "\nForge Workspace Snapshot Generator\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Creating snapshot at: " << fs::absolute(snapshotFolder).string() << "\n\n";

    size_t filesCopied = 0;
    uintmax_t totalBytes = 0;

    try {
        fs::create_directories(snapshotFolder);

        for (const auto& entry : fs::recursive_directory_iterator(".", fs::directory_options::skip_permission_denied)) {
            const auto& path = entry.path();

            bool skip = false;
            for (const auto& part : path) {
                if (shouldIgnorePath(part)) {
                    skip = true;
                    break;
                }
            }
            if (skip) continue;

            if (fs::is_regular_file(path)) {
                fs::path relPath = fs::relative(path, ".");
                fs::path destPath = snapshotFolder / relPath;

                fs::create_directories(destPath.parent_path());
                fs::copy_file(path, destPath, fs::copy_options::overwrite_existing);

                filesCopied++;
                totalBytes += fs::file_size(path);
            }
        }

        double sizeMB = static_cast<double>(totalBytes) / (1024.0 * 1024.0);

        std::cout << "  [+] Copied Files  : " << filesCopied << "\n";
        std::cout << "  [+] Archive Size  : " << sizeMB << " MB\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "  Snapshot Created Successfully!\n\n";

    } catch (const std::exception& ex) {
        std::cerr << "  [!] Snapshot failed: " << ex.what() << "\n\n";
        return 1;
    }

    return 0;
}

} // namespace forge