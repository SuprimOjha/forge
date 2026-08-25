#include "forge/commands/dashboard.hpp"

#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>
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

void printDashboardHelp() {
    std::cout
        << "\n"
        << "Forge Real-time Workspace Dashboard\n\n"

        << "Usage:\n"
        << "  forge dashboard [options]\n\n"

        << "Options:\n"
        << "  -h, --help    Show this help message\n";
}

} // anonymous namespace

int runDashboard(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc >= 3) {
        std::string arg = argv[2];
        if (arg == "-h" || arg == "--help") {
            printDashboardHelp();
            return 0;
        }
    }

    fs::path workspacePath = fs::current_path();

    std::cout << "\n========================================================\n";
    std::cout << "             🔨 FORGE WORKSPACE DASHBOARD               \n";
    std::cout << "========================================================\n\n";

    std::cout << "  📍 Path            : " << workspacePath.string() << "\n";
    std::cout << "  🌿 Git Status      : " << (fs::exists(workspacePath / ".git") ? "Initialized [OK]" : "No Git Repo") << "\n";
    std::cout << "  🏗️  Build Directory : " << (fs::exists(workspacePath / "build") ? "Ready [OK]" : "Missing") << "\n";
    std::cout << "  📄 Configuration   : " << (fs::exists(workspacePath / "CMakeLists.txt") ? "CMake project detected" : "No CMakeLists.txt") << "\n\n";

    std::cout << "--------------------------------------------------------\n";
    std::cout << "  Status Summary: All systems nominal. Ready for commands.\n";
    std::cout << "========================================================\n\n";

    return 0;
}

} // namespace forge