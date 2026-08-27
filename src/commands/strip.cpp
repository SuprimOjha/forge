#include "forge/commands/strip.hpp"

#include <iostream>
#include <filesystem>
#include <string>
#include <cstdlib>

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

void printStripHelp() {
    std::cout
        << "\n"
        << "Forge Binary Stripper & Optimizer\n\n"

        << "Usage:\n"
        << "  forge strip [target] [options]\n\n"

        << "Options:\n"
        << "  -h, --help    Show this help message\n\n"

        << "Examples:\n"
        << "  forge strip\n"
        << "  forge strip build/forge.exe\n";
}

} // anonymous namespace

int runStrip(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path targetBinary = fs::path("build") / "forge.exe";
    if (!fs::exists(targetBinary)) {
        targetBinary = fs::path("build") / "forge";
    }

    if (argc >= 3) {
        std::string arg = argv[2];
        if (arg == "-h" || arg == "--help") {
            printStripHelp();
            return 0;
        }
        targetBinary = arg;
    }

    std::cout << "\nForge Binary Symbol Stripper\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Target Binary : " << fs::absolute(targetBinary).string() << "\n\n";

    if (!fs::exists(targetBinary)) {
        std::cerr << "  [!] Error: Binary not found at path. Run 'forge build' first.\n\n";
        return 1;
    }

    size_t sizeBefore = fs::file_size(targetBinary);
    std::cout << "  [i] Size Before Stripping : " << (sizeBefore / 1024) << " KB\n";

    std::cout << "  [i] Stripping Debug Symbols...\n";

    std::string stripCmd = "strip \"" + targetBinary.string() + "\"";

#ifdef _WIN32
    // Suppress output or attempt strip via system environment path
    stripCmd += " > NUL 2>&1";
#else
    stripCmd += " > /dev/null 2>&1";
#endif

    int result = std::system(stripCmd.c_str());

    size_t sizeAfter = fs::file_size(targetBinary);
    double reduction = sizeBefore > 0 ? (1.0 - (double)sizeAfter / sizeBefore) * 100.0 : 0.0;

    std::cout << "--------------------------------------------\n";
    if (result == 0) {
        std::cout << "  Symbol Stripping  : 🟢 SUCCESS\n";
        std::cout << "  Size After        : " << (sizeAfter / 1024) << " KB\n";
        std::cout << "  Space Reduction   : " << std::fixed << (1.0 - (double)sizeAfter / sizeBefore) * 100.0 << "%\n\n";
        return 0;
    } else {
        std::cout << "  Symbol Stripping  : 🟡 NOTICE (Tool 'strip' not found in standard PATH)\n";
        std::cout << "  Binary Size       : " << (sizeAfter / 1024) << " KB (Unmodified)\n\n";
        return 0;
    }
}

} // namespace forge