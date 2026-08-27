#include "forge/commands/profile.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif

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

void printProfileHelp() {
    std::cout
        << "\n"
        << "Forge Command Execution Profile & Performance Benchmark\n\n"

        << "Usage:\n"
        << "  forge profile <command> [args...]\n\n"

        << "Options:\n"
        << "  -h, --help    Show this help message\n\n"

        << "Examples:\n"
        << "  forge profile forge build\n"
        << "  forge profile forge health\n";
}

size_t getPeakMemoryMB() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.PeakWorkingSetSize / (1024 * 1024);
    }
#endif
    return 0;
}

} // anonymous namespace

int runProfile(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printProfileHelp();
        return 0;
    }

    std::string firstArg = argv[2];
    if (firstArg == "-h" || firstArg == "--help") {
        printProfileHelp();
        return 0;
    }

    std::string fullCmd = "";
    for (int i = 2; i < argc; ++i) {
        if (i > 2) fullCmd += " ";
        fullCmd += argv[i];
    }

    std::cout << "\nForge Command Execution Profiler\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Profiling Command: " << fullCmd << "\n\n";

    auto start = std::chrono::high_resolution_clock::now();

    int exitCode = std::system(fullCmd.c_str());

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationMs = end - start;

    size_t peakMemMB = getPeakMemoryMB();

    std::cout << "\n--------------------------------------------\n";
    std::cout << "  Execution Metrics Summary:\n";
    std::cout << "  • Elapsed Time : " << std::fixed << std::setprecision(2) << durationMs.count() << " ms (" 
              << durationMs.count() / 1000.0 << " s)\n";
    std::cout << "  • Exit Code    : " << exitCode << " (" << (exitCode == 0 ? "SUCCESS" : "FAILED") << ")\n";
    if (peakMemMB > 0) {
        std::cout << "  • Peak Memory  : ~" << peakMemMB << " MB\n";
    }
    std::cout << "\n";

    return exitCode == 0 ? 0 : 1;
}

} // namespace forge