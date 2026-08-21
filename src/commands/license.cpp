#include "forge/commands/license.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

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
        << "Forge License Compliance & Risk Auditor\n\n"

        << "Usage:\n"
        << "  forge license [options]\n\n"

        << "Options:\n"
        << "  -h, --help              Show this help message\n";
}

struct LicenseInfo {
    std::string name;
    std::string type; // Permissive, Weak Copyleft, Strong Copyleft
    std::string risk; // LOW, MEDIUM, HIGH
};

LicenseInfo identifyLicense(const std::string& content) {
    std::string lowerContent = content;
    std::transform(lowerContent.begin(), lowerContent.end(), lowerContent.begin(), ::tolower);

    if (lowerContent.find("mit license") != std::string::npos || lowerContent.find("permission is hereby granted, free of charge") != std::string::npos) {
        return {"MIT License", "Permissive", "LOW"};
    }
    if (lowerContent.find("apache license") != std::string::npos || lowerContent.find("http://www.apache.org/licenses/license-2.0") != std::string::npos) {
        return {"Apache-2.0", "Permissive", "LOW"};
    }
    if (lowerContent.find("bsd 3-clause") != std::string::npos || lowerContent.find("redistribution and use in source and binary forms") != std::string::npos) {
        return {"BSD-3-Clause", "Permissive", "LOW"};
    }
    if (lowerContent.find("gnu general public license") != std::string::npos || lowerContent.find("gpl-3.0") != std::string::npos) {
        return {"GPL-3.0", "Strong Copyleft", "HIGH"};
    }
    if (lowerContent.find("gnu lesser general public license") != std::string::npos || lowerContent.find("lgpl") != std::string::npos) {
        return {"LGPL-3.0", "Weak Copyleft", "MEDIUM"};
    }
    if (lowerContent.find("mozilla public license") != std::string::npos || lowerContent.find("mpl-2.0") != std::string::npos) {
        return {"MPL-2.0", "Weak Copyleft", "MEDIUM"};
    }

    return {"Unknown / Custom", "Uncategorized", "UNKNOWN"};
}

} // anonymous namespace

int runLicense(int argc, char* argv[]) {
    enableConsoleEncoding();

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printLicenseHelp();
            return 0;
        }
    }

    std::cout << "\nForge License Compliance & Risk Auditor\n";
    std::cout << "--------------------------------------------\n\n";

    fs::path licensePath;
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        std::string filename = entry.path().filename().string();
        std::string lowerName = filename;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        if (lowerName == "license" || lowerName == "license.md" || lowerName == "license.txt" || lowerName == "copying") {
            licensePath = entry.path();
            break;
        }
    }

    if (licensePath.empty()) {
        std::cout << "  [!] Warning: No primary LICENSE file detected in workspace root.\n\n";
        return 1;
    }

    std::ifstream file(licensePath);
    if (!file.is_open()) {
        std::cerr << "  [!] Error opening license file: " << licensePath.string() << "\n\n";
        return 1;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    LicenseInfo info = identifyLicense(content);

    std::cout << "  Primary License File : " << licensePath.filename().string() << "\n";
    std::cout << "  Detected License     : " << info.name << "\n";
    std::cout << "  Category / Type      : " << info.type << "\n";
    std::cout << "  Compliance Risk      : [" << info.risk << "]\n\n";

    if (info.risk == "LOW") {
        std::cout << "  [OK] License is commercially friendly and standard for open source software.\n\n";
    } else if (info.risk == "HIGH") {
        std::cout << "  [!] Notice: Copyleft terms apply. Code using this library may need to be open-sourced.\n\n";
    }

    return 0;
}

} // namespace forge