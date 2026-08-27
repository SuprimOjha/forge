#include "forge/commands/run.hpp"
#include "forge/core/process_runner.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <thread>
#include <chrono>
#include <map>

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

void printRunHelp() {
    std::cout
        << "\n"
        << "Forge Universal Process Runner & Watcher\n\n"

        << "Usage:\n"
        << "  forge run [options] -- <command...>\n\n"

        << "Options:\n"
        << "  -w, --watch             Watch workspace directory and auto-restart process on file changes\n"
        << "  -h, --help              Show this help message\n\n"

        << "Examples:\n"
        << "  forge run -- python main.py\n"
        << "  forge run --watch -- cargo run\n";
}

std::map<std::string, fs::file_time_type> getDirectoryState(const fs::path& dirPath) {
    std::map<std::string, fs::file_time_type> state;
    try {
        for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                std::string relPath = fs::relative(entry.path(), dirPath).string();
                if (relPath.find("build") != 0 && relPath.find(".git") != 0 && relPath.find("node_modules") != 0) {
                    std::error_code ec;
                    auto lastTime = fs::last_write_time(entry.path(), ec);
                    if (!ec) {
                        state[relPath] = lastTime;
                    }
                }
            }
        }
    } catch (...) {}
    return state;
}

bool hasDirectoryChanged(const std::map<std::string, fs::file_time_type>& oldState,
                        const std::map<std::string, fs::file_time_type>& newState) {
    if (oldState.size() != newState.size()) return true;

    for (const auto& [path, time] : newState) {
        auto it = oldState.find(path);
        if (it == oldState.end() || it->second != time) {
            return true;
        }
    }
    return false;
}

} // anonymous namespace

int runRun(int argc, char* argv[]) {
    enableConsoleEncoding();

    bool watch = false;
    std::string commandStr;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-w" || arg == "--watch") {
            watch = true;
        } else if (arg == "-h" || arg == "--help") {
            printRunHelp();
            return 0;
        } else if (arg == "--") {
            for (int j = i + 1; j < argc; ++j) {
                if (!commandStr.empty()) commandStr += " ";
                commandStr += argv[j];
            }
            break;
        } else if (commandStr.empty() && arg[0] != '-') {
            for (int j = i; j < argc; ++j) {
                if (!commandStr.empty()) commandStr += " ";
                commandStr += argv[j];
            }
            break;
        }
    }

    if (commandStr.empty()) {
        std::cerr << "  [!] Error: No target executable command provided.\n\n";
        printRunHelp();
        return 1;
    }

    std::cout << "\nForge Dev Runner\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Command: " << commandStr << "\n";
    std::cout << "  Mode   : " << (watch ? "Watch & Auto-Reload" : "Single Execution") << "\n\n";

    if (!watch) {
        ProcessResult res = ProcessRunner::run(commandStr);
        return res.exitCode;
    }

    // Watch mode loop
    auto currentState = getDirectoryState(fs::current_path());
    std::cout << "  [+] Watching directory for changes... (Press Ctrl+C to stop)\n\n";

    ProcessRunner::run(commandStr);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        auto newState = getDirectoryState(fs::current_path());

        if (hasDirectoryChanged(currentState, newState)) {
            currentState = newState;
            std::cout << "\n  [!] File modification detected. Restarting process...\n";
            std::cout << "--------------------------------------------\n\n";
            ProcessRunner::run(commandStr);
        }
    }

    return 0;
}

} // namespace forge