#include "forge/commands/license.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>

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

void printLicenseHelp() {
    std::cout
        << "\n"
        << "Forge License & Header Checker\n\n"

        << "Usage:\n"
        << "  forge license [options]\n\n"

        << "Options:\n"
        << "  --check                 Check source files for missing copyright headers\n"
        << "  --apply <holder>        Inject standard copyright header with specified holder\n"
        << "  -h, --help              Show this help message\n";
}

} // anonymous namespace

int runLicense(int argc, char* argv[]) {
    enableConsoleEncoding();

    bool checkHeaders = false;
    bool applyHeaders = false;
    std::string holderName = "";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--check") {
            checkHeaders = true;
        } else if (arg == "--apply" && i + 1 < argc) {
            applyHeaders = true;
            holderName = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            printLicenseHelp();
            return 0;
        }
    }

    std::cout << "\nForge License & Header Compliance Engine\n";
    std::cout << "--------------------------------------------\n\n";

    // 1. Detect Root LICENSE file
    if (fs::exists("LICENSE") || fs::exists("LICENSE.md") || fs::exists("LICENSE.txt")) {
        std::cout << "  [OK] Project LICENSE file detected.\n";
    } else {
        std::cout << "  [WARN] No root LICENSE file found in workspace.\n";
    }

    if (!checkHeaders && !applyHeaders) {
        std::cout << "\n  Use 'forge license --check' to scan source file headers.\n\n";
        return 0;
    }

    std::vector<fs::path> missingHeaders;
    std::vector<std::string> targetExts = {".cpp", ".hpp", ".h", ".c"};

    auto scanAndProcess = [&](const std::string& dir) {
        if (!fs::exists(dir)) return;
        for (const auto& entry : fs::recursive_directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (std::find(targetExts.begin(), targetExts.end(), ext) != targetExts.end()) {
                    std::ifstream inFile(entry.path());
                    std::string firstLine;
                    std::getline(inFile, firstLine);
                    inFile.close();

                    if (firstLine.find("Copyright") == std::string::npos && firstLine.find("SPDX") == std::string::npos) {
                        missingHeaders.push_back(entry.path());
                    }
                }
            }
        }
    };

    scanAndProcess("src");
    scanAndProcess("include");

    if (checkHeaders) {
        std::cout << "\n  Header Compliance Report:\n";
        if (missingHeaders.empty()) {
            std::cout << "    [OK] All source files contain copyright or SPDX headers.\n\n";
        } else {
            std::cout << "    [WARN] Found " << missingHeaders.size() << " file(s) missing copyright headers:\n";
            for (const auto& file : missingHeaders) {
                std::cout << "      - " << file.string() << "\n";
            }
            std::cout << "\n";
        }
    }

    if (applyHeaders && !holderName.empty()) {
        std::string headerText = "// Copyright (c) " + holderName + ". All rights reserved.\n";
        int updated = 0;

        for (const auto& filePath : missingHeaders) {
            std::ifstream inFile(filePath);
            std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
            inFile.close();

            std::ofstream outFile(filePath);
            outFile << headerText << content;
            outFile.close();
            ++updated;
        }

        std::cout << "  [+] Injected copyright headers into " << updated << " file(s).\n\n";
    }

    return 0;
}

} // namespace forge