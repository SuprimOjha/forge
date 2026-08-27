#include "forge/commands/cheat.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <map>
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

struct CheatEntry {
    std::string category;
    std::string description;
    std::string example;
};

void printCheatHelp() {
    std::cout
        << "\n"
        << "Forge Command Cheat Sheet & Quick Reference\n\n"

        << "Usage:\n"
        << "  forge cheat [command]\n\n"

        << "Options:\n"
        << "  -h, --help    Show this help message\n\n"

        << "Examples:\n"
        << "  forge cheat\n"
        << "  forge cheat build\n";
}

} // anonymous namespace

int runCheat(int argc, char* argv[]) {
    enableConsoleEncoding();

    std::string queryCmd = "";
    if (argc >= 3) {
        queryCmd = argv[2];
        if (queryCmd == "-h" || queryCmd == "--help") {
            printCheatHelp();
            return 0;
        }
    }

    std::map<std::string, std::vector<CheatEntry>> cheats = {
        {"build", {
            {"Compile project", "Build debug configuration", "forge build"},
            {"Release mode", "Build release optimized target", "forge build --config Release"}
        }},
        {"health", {
            {"Workspace Audit", "Run end-to-end readiness audit", "forge health"},
            {"Target Dir", "Audit specific path", "forge health -d /path/to/project"}
        }},
        {"todo", {
            {"Find Debt", "Scan workspace for TODO/FIXME tags", "forge todo"},
            {"Target Scan", "Scan custom directory", "forge todo -d src/"}
        }},
        {"sweep", {
            {"Analyze Disk", "Calculate reclaimable cache size", "forge sweep"},
            {"Purge Trash", "Remove build artifacts & cache", "forge sweep --purge"}
        }},
        {"tree", {
            {"Map Directives", "Display visual dependency tree", "forge tree"},
            {"Include Dir", "Map specific target path", "forge tree -d include/"}
        }}
    };

    std::cout << "\nForge Interactive Command Cheat Sheet\n";
    std::cout << "--------------------------------------------\n";

    if (!queryCmd.empty()) {
        if (cheats.find(queryCmd) != cheats.end()) {
            std::cout << "  Command Reference: forge " << queryCmd << "\n\n";
            for (const auto& entry : cheats[queryCmd]) {
                std::cout << "  • " << entry.category << " - " << entry.description << "\n";
                std::cout << "    $ " << entry.example << "\n\n";
            }
            return 0;
        } else {
            std::cout << "  [i] No specific cheat sheet for '" << queryCmd << "'. Showing general cheat sheet.\n\n";
        }
    }

    std::cout << "  Common Command Workflows:\n\n";
    for (const auto& [cmd, entries] : cheats) {
        std::cout << "  🔹 forge " << std::left << std::setw(12) << cmd << " -> " << entries[0].example << "\n";
    }
    std::cout << "\n  Run 'forge cheat <command>' for detailed command reference.\n\n";

    return 0;
}

} // namespace forge