#include "forge/commands/archive.hpp"
#include "forge/core/process_runner.hpp"

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

void printArchiveHelp() {
    std::cout
        << "\n"
        << "Forge Clean Workspace Archiver\n\n"

        << "Usage:\n"
        << "  forge archive [options]\n\n"

        << "Options:\n"
        << "  -o, --output <filename>  Specify output archive path/filename\n"
        << "  -h, --help               Show this help message\n\n";
}

std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

} // anonymous namespace

int runArchive(int argc, char* argv[]) {
    enableConsoleEncoding();

    std::string customOutput;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            customOutput = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            printArchiveHelp();
            return 0;
        }
    }

    std::cout << "\nForge Project Archiver\n";
    std::cout << "--------------------------------------------\n\n";

    std::string projName = fs::current_path().filename().string();
    fs::create_directories("dist");

    std::string archiveName = customOutput.empty() ? ("dist/" + projName + "_" + getTimestamp() + ".zip") : customOutput;

    // Detect paths that actually exist to prevent Compress-Archive errors
    std::vector<std::string> candidateTargets = {"src", "include", "CMakeLists.txt", "package.json", "README.md", "LICENSE"};
    std::vector<std::string> existingTargets;

    for (const auto& target : candidateTargets) {
        if (fs::exists(target)) {
            existingTargets.push_back("'" + target + "'");
        }
    }

    if (existingTargets.empty()) {
        std::cout << "  [ERROR] No valid source targets found to archive.\n\n";
        return 1;
    }

    std::cout << "  [*] Packaging workspace source files for '" << projName << "'...\n";

    std::string pathList = "";
    for (size_t i = 0; i < existingTargets.size(); ++i) {
        pathList += existingTargets[i];
        if (i + 1 < existingTargets.size()) pathList += ", ";
    }

#ifdef _WIN32
    std::string cmd = "powershell -Command \"Compress-Archive -Path " + pathList + " -DestinationPath '" + archiveName + "' -Force\"";
#else
    std::string cmd = "zip -r " + archiveName + " src include CMakeLists.txt package.json README.md LICENSE -x 'build/*' '.git/*'";
#endif

    ProcessRunner::run(cmd);

    if (fs::exists(archiveName)) {
        std::uintmax_t size = fs::file_size(archiveName);
        std::cout << "  [OK] Created archive: " << archiveName << " (" << (size / 1024) << " KB)\n\n";
        return 0;
    }

    std::cout << "  [ERROR] Failed to generate archive bundle.\n\n";
    return 1;
}

} // namespace forge