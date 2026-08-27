#include "forge/commands/bench.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <numeric>
#include <iomanip>
#include <cstdlib>

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
        << "Forge Command Performance Benchmark\n\n"

        << "Usage:\n"
        << "  forge bench \"<command>\" [options]\n\n"

        << "Options:\n"
        << "  -n, --runs <count>    Number of iterations [default: 5]\n"
        << "  -h, --help            Show this help message\n\n"

        << "Examples:\n"
        << "  forge bench \"forge doctor\"\n"
        << "  forge bench \"forge health\" -n 10\n";
}

} // anonymous namespace

int runBench(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printBenchHelp();
        return 0;
    }

    std::string targetCmd = argv[2];
    if (targetCmd == "-h" || targetCmd == "--help") {
        printBenchHelp();
        return 0;
    }

    int runs = 5;
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-n" || arg == "--runs") && i + 1 < argc) {
            runs = std::stoi(argv[++i]);
        }
    }

    if (runs <= 0) runs = 1;

    std::cout << "\nForge Subcommand Performance Benchmark\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Benchmark Target : \"" << targetCmd << "\"\n";
    std::cout << "  Iterations       : " << runs << "\n\n";

    std::vector<double> timings;
    timings.reserve(runs);

    for (int i = 1; i <= runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();

#ifdef _WIN32
        std::string suppressedCmd = targetCmd + " > NUL 2>&1";
#else
        std::string suppressedCmd = targetCmd + " > /dev/null 2>&1";
#endif

        int status = std::system(suppressedCmd.c_str());
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> elapsed = end - start;
        timings.push_back(elapsed.count());

        std::cout << "  Run #" << i << ": " << std::fixed << std::setprecision(2) << elapsed.count() << " ms"
                  << (status == 0 ? " (OK)" : " (FAILED)") << "\n";
    }

    double sum = std::accumulate(timings.begin(), timings.end(), 0.0);
    double mean = sum / timings.size();

    double sqSum = 0.0;
    for (double t : timings) {
        sqSum += (t - mean) * (t - mean);
    }
    double stdDev = std::sqrt(sqSum / timings.size());

    auto minMax = std::minmax_element(timings.begin(), timings.end());

    std::cout << "\n--------------------------------------------\n";
    std::cout << "  Benchmark Execution Metrics Summary:\n";
    std::cout << "  • Mean Duration : " << std::fixed << std::setprecision(2) << mean << " ms\n";
    std::cout << "  • Min / Max     : " << *minMax.first << " ms / " << *minMax.second << " ms\n";
    std::cout << "  • Std Dev       : ±" << stdDev << " ms\n\n";

    return 0;
}

} // namespace forge