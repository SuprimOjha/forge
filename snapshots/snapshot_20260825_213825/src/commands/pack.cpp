#include "forge/commands/pack.hpp"

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

void printPackHelp() {
    std::cout
        << "\n"
<< "Forge Release Packaging & Asset Bundler\n\n"
        << "Usage:\n"
 << "  forge pack [options]\n\n"

        << "Options:\n"
        << "  -o, --output <dir>  Output directory for release bundle [default: dist/]\n"
 << "  -h, --help          Show this help message\n\n"

        << "Examples:\n"
 << "  forge pack\n"
        << "  forge pack -o release_v1.0/\n";
}

} // anonymous namespace

int runPack(int argc, char* argv[]) {
    enableConsoleEncoding();

    fs::path outputDir = "dist";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
if (arg == "-h" || arg == "--help") {
            printPackHelp();
            return 0;
 } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            outputDir = argv[++i];
        }
    }

    std::cout << "\nForge Release Packager\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Packaging target bundle to: " << fs::absolute(outputDir).string() << "\n\n";

    fs::path buildExecutable = fs::path("build") / "forge.exe";
if (!fs::exists(buildExecutable)) {
        buildExecutable = fs::path("build") / "forge";
    }

if (!fs::exists(buildExecutable)) {
std::cerr << "  [!] Error: Built executable not found in build/. Run 'forge build' first.\n\n";
     return 1;
    }

    try {
        if (!fs::exists(outputDir)) {
            fs::create_directories(outputDir);
        }

fs::path destExe = outputDir / buildExecutable.filename();
fs::copy_file(buildExecutable, destExe, fs::copy_options::overwrite_existing);

        std::cout << "  [+] Bundled Binary : " << destExe.string() << "\n";
        
uintmax_t bytes = fs::file_size(destExe);
double sizeMB = static_cast<double>(bytes) / (1024.0 * 1024.0);

        std::cout << "--------------------------------------------\n";
        std::cout << "  Release Package Created Successfully! (" << sizeMB << " MB)\n\n";
    } catch (const std::exception& ex) {
        std::cerr << "  [!] Packaging failed: " << ex.what() << "\n\n";
 return 1;
    }

return 0;
}

} // namespace forge