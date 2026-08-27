#include "forge/commands/parallel.hpp"
#include "forge/core/process_runner.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include <chrono>
#include <atomic>
#include <sstream>

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

void printParallelHelp() {
    std::cout
        << "\n"
        << "Forge Parallel Worker Pool Runner\n\n"

        << "Usage:\n"
        << "  forge parallel -- <cmd1> :: <cmd2> :: <cmd3>...\n\n"

        << "Options:\n"
        << "  -j, --jobs <N>    Set number of parallel worker threads [default: hardware concurrency]\n"
        << "  -h, --help        Show this help message\n\n"

        << "Example:\n"
        << "  forge parallel -j 4 -- \"cmake --version\" :: \"git --version\" :: \"node --version\"\n";
}

struct TaskResult {
    std::string command;
    int exitCode;
    double durationMs;
};

} // anonymous namespace

int runParallel(int argc, char* argv[]) {
    enableConsoleEncoding();

    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    std::vector<std::string> commands;
    bool parsingCommands = false;
    std::string currentCmd;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--") {
            parsingCommands = true;
            continue;
        }

        if (parsingCommands) {
            if (arg == "::") {
                if (!currentCmd.empty()) {
                    commands.push_back(currentCmd);
                    currentCmd.clear();
                }
            } else {
                if (!currentCmd.empty()) currentCmd += " ";
                currentCmd += arg;
            }
        } else {
            if ((arg == "-j" || arg == "--jobs") && i + 1 < argc) {
                numThreads = static_cast<unsigned int>(std::stoi(argv[++i]));
            } else if (arg == "-h" || arg == "--help") {
                printParallelHelp();
                return 0;
            }
        }
    }

    if (!currentCmd.empty()) {
        commands.push_back(currentCmd);
    }

    if (commands.empty()) {
        std::cerr << "  [!] Error: No commands provided to execute.\n";
        printParallelHelp();
        return 1;
    }

    std::cout << "\nForge Parallel Worker Pool\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Worker Threads : " << numThreads << "\n";
    std::cout << "  Queued Tasks   : " << commands.size() << "\n\n";

    std::mutex queueMutex;
    std::mutex consoleMutex;
    size_t taskIndex = 0;
    std::atomic<int> failedTasks{0};
    std::vector<TaskResult> results(commands.size());

    auto startTime = std::chrono::high_resolution_clock::now();

    auto workerLambda = [&]() {
        while (true) {
            size_t idx = 0;
            std::string cmd;

            {
                std::lock_guard<std::mutex> lock(queueMutex);
                if (taskIndex >= commands.size()) {
                    return;
                }
                idx = taskIndex;
                cmd = commands[taskIndex++];
            }

            auto taskStart = std::chrono::high_resolution_clock::now();
            ProcessResult res = ProcessRunner::run(cmd);
            auto taskEnd = std::chrono::high_resolution_clock::now();

            double duration = std::chrono::duration<double, std::milli>(taskEnd - taskStart).count();
            results[idx] = {cmd, res.exitCode, duration};

            {
                std::lock_guard<std::mutex> lock(consoleMutex);
                if (res.exitCode == 0) {
                    std::cout << "  [OK] Task " << (idx + 1) << "/" << commands.size() 
                              << " finished (" << duration << " ms): " << cmd << "\n";
                } else {
                    std::cout << "  [FAIL] Task " << (idx + 1) << "/" << commands.size() 
                              << " failed (exit code " << res.exitCode << "): " << cmd << "\n";
                    failedTasks++;
                }
            }
        }
    };

    std::vector<std::thread> pool;
    for (unsigned int i = 0; i < std::min<unsigned int>(numThreads, static_cast<unsigned int>(commands.size())); ++i) {
        pool.emplace_back(workerLambda);
    }

    for (auto& t : pool) {
        if (t.joinable()) t.join();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    double totalTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    std::cout << "\n--------------------------------------------\n";
    std::cout << "  Parallel Pipeline Completed in " << totalTime << " ms\n";
    std::cout << "  Passed: " << (commands.size() - failedTasks) << " | Failed: " << failedTasks << "\n\n";

    return failedTasks > 0 ? 1 : 0;
}

} // namespace forge