#include "forge/commands/port.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
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

void printPortHelp() {
    std::cout
        << "\n"
        << "Forge Port Availability & Socket Diagnostics\n\n"

        << "Usage:\n"
        << "  forge port <port|host:port> [options]\n\n"

        << "Options:\n"
        << "  -h, --host <host>    Target host address [default: 127.0.0.1]\n"
        << "  --help               Show this help message\n\n"

        << "Examples:\n"
        << "  forge port 8080\n"
        << "  forge port 443 -h github.com\n";
}

bool checkPort(const std::string& host, int port, double& latencyMs) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return false;
    }
#endif

    struct addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::string portStr = std::to_string(port);
    if (getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res) != 0) {
#ifdef _WIN32
        WSACleanup();
#endif
        return false;
    }

    auto start = std::chrono::high_resolution_clock::now();
#ifdef _WIN32
    SOCKET sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bool connected = (sock != INVALID_SOCKET && connect(sock, res->ai_addr, (int)res->ai_addrlen) == 0);
    if (sock != INVALID_SOCKET) closesocket(sock);
    WSACleanup();
#else
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bool connected = (sock >= 0 && connect(sock, res->ai_addr, res->ai_addrlen) == 0);
    if (sock >= 0) close(sock);
#endif
    auto end = std::chrono::high_resolution_clock::now();

    freeaddrinfo(res);
    latencyMs = std::chrono::duration<double, std::milli>(end - start).count();

    return connected;
}

} // anonymous namespace

int runPort(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printPortHelp();
        return 1;
    }

    std::string host = "127.0.0.1";
    int port = 0;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            printPortHelp();
            return 0;
        } else if ((arg == "-h" || arg == "--host") && i + 1 < argc) {
            host = argv[++i];
        } else if (port == 0 && arg[0] != '-') {
            size_t colon = arg.find(':');
            if (colon != std::string::npos) {
                host = arg.substr(0, colon);
                port = std::stoi(arg.substr(colon + 1));
            } else {
                port = std::stoi(arg);
            }
        }
    }

    if (port <= 0 || port > 65535) {
        std::cerr << "  [!] Error: Invalid port number specified.\n\n";
        return 1;
    }

    std::cout << "\nForge Socket Diagnostic\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Target Host : " << host << "\n";
    std::cout << "  Target Port : " << port << "\n\n";

    double latency = 0.0;
    bool isOpen = checkPort(host, port, latency);

    if (isOpen) {
        std::cout << "  [OPEN] Port " << port << " is actively accepting connections (" << latency << " ms).\n\n";
        return 0;
    } else {
        std::cout << "  [CLOSED/AVAILABLE] Port " << port << " is not accepting connections.\n\n";
        return 1;
    }
}

} // namespace forge