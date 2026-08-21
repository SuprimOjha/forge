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
        << "Forge License & Open Source Compliance Auditor\n\n"

        << "Usage:\n"
        << "  forge license [options]\n\n"

        << "Options:\n"
        << "  -d, --dir <path>    Path to workspace root [default: .]\n"
        << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
        << "  forge license\n"
        << "  forge license -d /path/to/project\n";
}

std::string detectLicenseType(const std::string& content) {
    std::string lower = content;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower.find("mit license") != std::string::npos || lower.find("permission is hereby granted, free of charge") != std::string::npos) {
        return "MIT (Permissive)";
    }
    if (lower.find("apache license") != std::string::npos || lower.find("http://www.apache.org/licenses/license-2.0") != std::string::npos) {
        return "Apache 2.0 (Permissive)";
    }
    if (lower.find("bsd 3-clause") != std::string::npos || lower.find("redistribution and use in source and binary forms") != std::string::npos) {
        return "BSD 3-Clause (Permissive)";
    }
    if (lower.find("gnu general public license") != std::string::npos || lower.find("gpl") != std::string::npos) {
        return "GPL / Copyleft (Restrictive)";
    }
    return "Unknown / Custom License";
}

} // anonymous namespace

int runLicense(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path targetDir = ".";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printLicenseHelp();
            return 0;
        } else if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            targetDir = argv[++i];
        }
    }

    std::cout << "\nForge License & Compliance Auditor\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Workspace Target : " << fs::absolute(targetDir).string() << "\n\n";

    fs::path licenseFile;
    for (const auto& name : {"LICENSE", "LICENSE.txt", "LICENSE.md", "COPYING"}) {
        if (fs::exists(targetDir / name)) {
            licenseFile = targetDir / name;
            break;
        }
    }

    if (licenseFile.empty()) {
        std::cout << "  [!] Warning: No primary LICENSE or COPYING file found in workspace root.\n";
        std::cout << "      Consider adding an explicit license file (e.g., MIT, Apache-2.0).\n\n";
        return 1;
    }

    std::ifstream file(licenseFile);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::string licenseType = detectLicenseType(content);

    std::cout << "  [+] Primary File : " << licenseFile.filename().string() << "\n";
    std::cout << "  [+] License Type : " << licenseType << "\n\n";
    std::cout << "  [OK] Workspace license audit completed cleanly.\n\n";

    return 0;
}

} // namespace forge