#include "forge/commands/alias.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

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

void printAliasHelp() {
    std::cout
        << "\n"
        << "Forge Shell Alias Generator\n\n"

        << "Usage:\n"
        << "  forge alias [options]\n\n"

        << "Options:\n"
        << "  -o, --output <dir>    Output directory for alias scripts [default: .]\n"
        << "  -h, --help            Show this help message\n\n"

        << "Examples:\n"
        << "  forge alias\n"
        << "  forge alias -o scripts/\n";
}

} // anonymous namespace

int runAlias(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path outputDir = ".";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printAliasHelp();
            return 0;
        } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            outputDir = argv[++i];
        }
    }

    std::cout << "\nForge Shell Alias & Shortcode Generator\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Generating Alias Scripts in : " << fs::absolute(outputDir).string() << "\n\n";

    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    // 1. PowerShell Profile Snippet (forge_aliases.ps1)
    fs::path psPath = outputDir / "forge_aliases.ps1";
    std::ofstream psFile(psPath);
    if (psFile.is_open()) {
        psFile << "# Forge CLI PowerShell Aliases\n";
        psFile << "function fg { forge $args }\n";
        psFile << "function fgb { forge build $args }\n";
        psFile << "function fgd { forge doctor $args }\n";
        psFile << "function fgh { forge health $args }\n";
        psFile << "function fgs { forge status $args }\n";
        psFile.close();
        std::cout << "  • PowerShell Script : " << psPath.string() << "\n";
    }

    // 2. Bash / Zsh Profile Snippet (forge_aliases.sh)
    fs::path shPath = outputDir / "forge_aliases.sh";
    std::ofstream shFile(shPath);
    if (shFile.is_open()) {
        shFile << "#!/usr/bin/env bash\n";
        shFile << "# Forge CLI Bash/Zsh Aliases\n";
        shFile << "alias fg='forge'\n";
        shFile << "alias fgb='forge build'\n";
        shFile << "alias fgd='forge doctor'\n";
        shFile << "alias fgh='forge health'\n";
        shFile << "alias fgs='forge status'\n";
        shFile.close();
        std::cout << "  • Bash / Zsh Script  : " << shPath.string() << "\n";
    }

    // 3. CMD Batch Command Script (fg.bat)
    fs::path batPath = outputDir / "fg.bat";
    std::ofstream batFile(batPath);
    if (batFile.is_open()) {
        batFile << "@echo off\n";
        batFile << "forge %*\n";
        batFile.close();
        std::cout << "  • Windows CMD Batch : " << batPath.string() << "\n";
    }

    std::cout << "\n--------------------------------------------\n";
    std::cout << "  Alias Export Status : 🟢 SUCCESS\n";
    std::cout << "  Usage Tip           : Dot-source or load into your shell profile\n";
    std::cout << "                        (e.g. '. .\\forge_aliases.ps1')\n\n";

    return 0;
}

} // namespace forge