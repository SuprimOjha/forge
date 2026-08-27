#include "forge/commands/package.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdlib>

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

void printPackageHelp() {
    std::cout
        << "\n"
        << "Forge Release Packaging & Bundler\n\n"

        << "Usage:\n"
        << "  forge package [options]\n\n"

        << "Options:\n"
        << "  -v, --version <tag>    Set version string for bundle tag [default: latest]\n"
        << "  -o, --out <dir>        Output directory for generated archives [default: dist/]\n"
        << "  -h, --help             Show this help message\n\n"

        << "Examples:\n"
        << "  forge package\n"
        << "  forge package -v 1.2.0 -o dist/\n";
}

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

} // anonymous namespace

int runPackage(int argc, char* argv[]) {
    enableConsoleEncoding();

    std::string versionTag = "latest";
    fs::path outDir = "dist";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printPackageHelp();
            return 0;
        } else if ((arg == "-v" || arg == "--version") && i + 1 < argc) {
            versionTag = argv[++i];
        } else if ((arg == "-o" || arg == "--out") && i + 1 < argc) {
            outDir = argv[++i];
        }
    }

    std::cout << "\nForge Release Packaging & Bundler\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Version Tag : " << versionTag << "\n";
    std::cout << "  Output Dir  : " << fs::absolute(outDir).string() << "\n\n";

    fs::path exePath = fs::path("build") / "forge.exe";
    if (!fs::exists(exePath)) {
        exePath = fs::path("build") / "forge";
    }

    if (!fs::exists(exePath)) {
        std::cerr << "  [!] Error: Executable not found in build/ directory. Run 'forge build' first.\n\n";
        return 1;
    }

    fs::create_directories(outDir);

    std::string archiveName = "forge-" + versionTag + "-" + getCurrentTimestamp() + ".zip";
    fs::path archivePath = outDir / archiveName;

    std::cout << "  [i] Staging Release Package Files...\n";
    std::cout << "      • Executable : " << exePath.string() << "\n";

    if (fs::exists("LICENSE")) {
        std::cout << "      • License    : LICENSE\n";
    }
    if (fs::exists("README.md")) {
        std::cout << "      • Readme     : README.md\n";
    }

    std::cout << "\n  [i] Compressing Distribution Archive...\n";

    // Standard PowerShell compression call on Windows
#ifdef _WIN32
    std::string powershellCmd = "powershell -Command \"Compress-Archive -Path '" + 
        exePath.string() + "', 'LICENSE', 'README.md' -DestinationPath '" + 
        archivePath.string() + "' -Force\"";
    int res = std::system(powershellCmd.c_str());
#else
    std::string zipCmd = "zip -j " + archivePath.string() + " " + exePath.string() + " LICENSE README.md";
    int res = std::system(zipCmd.c_str());
#endif

    std::cout << "--------------------------------------------\n";
    if (res == 0 && fs::exists(archivePath)) {
        auto fileSizeKB = fs::file_size(archivePath) / 1024;
        std::cout << "  Release Package Created: 🟢 SUCCESS\n";
        std::cout << "  Archive Path           : " << archivePath.string() << " (" << fileSizeKB << " KB)\n\n";
        return 0;
    } else {
        std::cout << "  Release Package Created: 🔴 FAILED\n\n";
        return 1;
    }
}

} // namespace forge