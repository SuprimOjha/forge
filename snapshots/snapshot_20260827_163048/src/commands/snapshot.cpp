#include "forge/commands/snapshot.hpp"

#include <iostream>
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
        << "Forge Workspace Snapshot & Restore Manager\n\n"

        << "Usage:\n"
        << "  forge snapshot [subcommand] [options]\n\n"

        << "Subcommands:\n"
        << "  create              Create a new timestamped workspace snapshot [default]\n"
        << "  list                List all available workspace snapshots\n"

        << "Options:\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge snapshot create\n"
        << "  forge snapshot list\n";
}

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

bool shouldSkip(const std::string& name) {
    return (name == "build" || name == ".git" || name == "dist" || name == "snapshots" || name == ".forge");
}

} // anonymous namespace

int runSnapshot(int argc, char* argv[]) {
    enableConsoleEncoding();

    std::string subCmd = "create";

    if (argc >= 3) {
        std::string arg = argv[2];
        if (arg == "-h" || arg == "--help") {
            printSnapshotHelp();
            return 0;
        }
        subCmd = arg;
    }

    fs::path snapshotDir = "snapshots";

    if (subCmd == "list") {
        std::cout << "\nForge Workspace Snapshots\n";
        std::cout << "--------------------------------------------\n";
        if (!fs::exists(snapshotDir) || fs::is_empty(snapshotDir)) {
            std::cout << "  No snapshots found in snapshots/\n\n";
            return 0;
        }

        for (const auto& entry : fs::directory_iterator(snapshotDir)) {
            if (entry.is_directory()) {
                std::cout << "  • " << entry.path().filename().string() << "\n";
            }
        }
        std::cout << "\n";
        return 0;
    }

    // Default: Create snapshot
    std::string snapshotName = "snapshot_" + getCurrentTimestamp();
    fs::path targetPath = snapshotDir / snapshotName;

    std::cout << "\nForge Workspace Snapshot Manager\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Creating Snapshot : " << snapshotName << "\n";
    std::cout << "  Destination       : " << fs::absolute(targetPath).string() << "\n\n";

    size_t fileCount = 0;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(".", fs::directory_options::skip_permission_denied)) {
            fs::path rel = fs::relative(entry.path(), ".");
            std::string rootFolder = rel.begin() != rel.end() ? rel.begin()->string() : "";

            if (shouldSkip(rootFolder)) {
                continue;
            }

            fs::path dest = targetPath / rel;

            if (entry.is_directory()) {
                fs::create_directories(dest);
            } else if (entry.is_regular_file()) {
                fs::create_directories(dest.parent_path());
                fs::copy_file(entry.path(), dest, fs::copy_options::overwrite_existing);
                fileCount++;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "  [!] Copy Error: " << e.what() << "\n";
        return 1;
    }

    std::cout << "--------------------------------------------\n";
    std::cout << "  Snapshot Status : 🟢 SUCCESS\n";
    std::cout << "  Files Archived  : " << fileCount << " source file(s)\n\n";

    return 0;
}

} // namespace forge