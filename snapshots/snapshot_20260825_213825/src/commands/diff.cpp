#include "forge/commands/diff.hpp"
#include "forge/core/process_runner.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

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

void printDiffHelp() {
    std::cout
        << "\n"
        << "Forge Workspace Diff & Snapshot Inspector\n\n"

        << "Usage:\n"
        << "  forge diff [options]\n\n"

        << "Options:\n"
        << "  --stat                  Show summary stat of modified files\n"
        << "  --cached, --staged      Show staged changes ready for commit\n"
        << "  -h, --help              Show this help message\n";
}

} // anonymous namespace

int runDiff(int argc, char* argv[]) {
    enableConsoleEncoding();

    bool statOnly = false;
    bool staged = false;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--stat") {
            statOnly = true;
        } else if (arg == "--cached" || arg == "--staged") {
            staged = true;
        } else if (arg == "-h" || arg == "--help") {
            printDiffHelp();
            return 0;
        }
    }

    std::cout << "\nForge Workspace Diff Inspector\n";
    std::cout << "--------------------------------------------\n\n";

    if (!fs::exists(".git")) {
        std::cout << "  [!] No Git repository detected in workspace root.\n\n";
        return 1;
    }

    std::string cmd = "git diff";
    if (staged) cmd += " --staged";
    if (statOnly) cmd += " --stat";

    ProcessResult res = ProcessRunner::run(cmd);

    if (res.exitCode == 0) {
        return 0;
    }

    std::cerr << "  [!] Failed to execute workspace diff inspection.\n\n";
    return res.exitCode;
}

} // namespace forge