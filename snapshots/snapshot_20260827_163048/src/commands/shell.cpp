#include "forge/commands/shell.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
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

void printShellHelp() {
    std::cout
        << "\n"
        << "Forge Interactive Command Shell (REPL)\n\n"

        << "Usage:\n"
        << "  forge shell [options]\n\n"

        << "Options:\n"
        << "  -h, --help    Show this help message\n\n"

        << "REPL Commands:\n"
        << "  exit, quit    Exit the REPL session\n"
        << "  help          Show available forge commands\n"
        << "  clear         Clear the terminal screen\n";
}

std::vector<std::string> tokenizeInput(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

} // anonymous namespace

int runShell(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc >= 3) {
        std::string arg = argv[2];
        if (arg == "-h" || arg == "--help") {
            printShellHelp();
            return 0;
        }
    }

    std::cout << "\n============================================\n";
    std::cout << "  🔨 Forge Interactive Shell v1.0 (REPL)\n";
    std::cout << "  Type 'help' for command list, 'exit' to quit.\n";
    std::cout << "============================================\n\n";

    std::string line;
    while (true) {
        std::cout << "forge> ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        std::vector<std::string> tokens = tokenizeInput(line);
        if (tokens.empty()) continue;

        std::string cmd = tokens[0];

        if (cmd == "exit" || cmd == "quit") {
            std::cout << "Exiting Forge shell. Goodbye!\n\n";
            break;
        } else if (cmd == "clear" || cmd == "cls") {
#ifdef _WIN32
            std::system("cls");
#else
            std::system("clear");
#endif
            continue;
        } else if (cmd == "help") {
            std::cout << "  Available REPL commands:\n";
            std::cout << "    • health, build, test, profile, cheat, arch, search, deps, env-check, sweep, tree, todo\n";
            std::cout << "    • clear (clear screen), exit (exit shell)\n\n";
            continue;
        }

        // Reconstruct full forge command string to execute
        std::string fullCmd = "forge " + line;
        std::system(fullCmd.c_str());
        std::cout << "\n";
    }

    return 0;
}

} // namespace forge