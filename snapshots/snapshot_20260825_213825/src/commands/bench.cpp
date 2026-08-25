#include "forge/commands/bench.hpp"
#include "forge/core/process_runner.hpp"

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
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

void printBenchHelp() {
    std::cout
        << "\n"
        << "Forge Benchmark Tool\n\n"

        << "Usage:\n"
        << "  forge bench <command>\n\n"

        << "Examples:\n"
        << "  forge bench forge build\n"
        << "  forge bench forge check\n\n";
}

} // anonymous namespace

int runBench(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printBenchHelp();
        return 1;
    }

    // Reconstruct command string from arguments starting at argv[2]
    std::string targetCommand = argv[2];
    for (int i = 3; i < argc; ++i) {
        targetCommand += " ";
        targetCommand += argv[i];
    }

    std::cout << "\nForge Command Benchmarker\n";
    std::cout << "--------------------------------------------\n\n";
    std::cout << "  [*] Benchmarking target: " << targetCommand << "\n\n";

    auto start = std::chrono::high_resolution_clock::now();

    ProcessResult result = ProcessRunner::run(targetCommand);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationMs = end - start;

    std::cout << "  Output:\n";
    std::cout << "  ------------------------------------------\n";
    if (!result.stdOut.empty()) {
        std::cout << result.stdOut << "\n";
    }
    if (!result.stdErr.empty()) {
        std::cerr << result.stdErr << "\n";
    }
    std::cout << "  ------------------------------------------\n\n";

    std::cout << "  Benchmark Summary:\n";
    std::cout << "    Exit Code:     " << result.exitCode << "\n";
    std::cout << "    Execution Time:" << std::fixed << std::setprecision(2) << durationMs.count() << " ms (" << (durationMs.count() / 1000.0) << " s)\n";
    std::cout << "    Status:        " << (result.exitCode == 0 ? "[SUCCESS]" : "[FAILED]") << "\n\n";

    return result.exitCode;
}

} // namespace forge