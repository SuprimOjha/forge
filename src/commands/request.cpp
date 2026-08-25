#include "forge/commands/request.hpp"
#include "forge/core/process_runner.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

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

void printRequestHelp() {
    std::cout
        << "\n"
        << "Forge HTTP & REST API Request Client\n\n"

        << "Usage:\n"
        << "  forge request <url> [options]\n\n"

        << "Options:\n"
        << "  -X, --method <METHOD>   HTTP method (GET, POST, PUT, DELETE) [default: GET]\n"
        << "  -d, --data <JSON/DATA>  Request payload body for POST/PUT\n"
        << "  -i, --include-headers   Include response HTTP headers in output\n"
        << "  -h, --help              Show this help message\n\n"

        << "Examples:\n"
        << "  forge request https://api.github.com/zen\n"
        << "  forge request http://localhost:8080/api -X POST -d '{\"name\":\"test\"}'\n";
}

} // anonymous namespace

int runRequest(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printRequestHelp();
        return 1;
    }

    std::string url;
    std::string method = "GET";
    std::string bodyData;
    bool includeHeaders = false;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printRequestHelp();
            return 0;
        } else if ((arg == "-X" || arg == "--method") && i + 1 < argc) {
            method = argv[++i];
        } else if ((arg == "-d" || arg == "--data") && i + 1 < argc) {
            bodyData = argv[++i];
        } else if (arg == "-i" || arg == "--include-headers") {
            includeHeaders = true;
        } else if (url.empty() && arg[0] != '-') {
            url = arg;
        }
    }

    if (url.empty()) {
        std::cerr << "  [!] Error: Target URL is required.\n\n";
        printRequestHelp();
        return 1;
    }

    std::cout << "\nForge HTTP Client\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Target URL : " << url << "\n";
    std::cout << "  Method     : " << method << "\n\n";

    std::string cmd = "curl -s -w \"\\n--- Latency: %{time_total}s | Status: %{http_code}\\n\"";
    if (includeHeaders) {
        cmd += " -i";
    }
    if (method != "GET") {
        cmd += " -X " + method;
    }
    if (!bodyData.empty()) {
        cmd += " -H \"Content-Type: application/json\" -d \"" + bodyData + "\"";
    }
    cmd += " \"" + url + "\"";

    auto start = std::chrono::high_resolution_clock::now();
    ProcessResult res = ProcessRunner::run(cmd);
    auto end = std::chrono::high_resolution_clock::now();

    if (res.exitCode != 0) {
        std::cerr << "  [!] Request failed or cURL utility is unavailable.\n\n";
        return res.exitCode;
    }

 return 0;
}

} // namespace forge