#include "forge/commands/port.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
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
#include <fcntl.h>
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

struct PortTarget {
    int port;
    std::string defaultService;
};

void printPortHelp() {
    std::cout
        << "\n"
        << "Forge Network Port & Local Service Health Analyzer\n\n"

        << "Usage:\n"
        << "  forge port [port|host:port] [options]\n\n"

        << "Options:\n"
        << "  -h, --host <host>    Target host address [default: 127.0.0.1]\n"
        << "  --check <port>       Check connectivity to a specific port\n"
        << "  --help               Show this help message\n\n"

        << "Examples:\n"
        << "  forge port                   (Scans common development ports on localhost)\n"
        << "  forge port 8080\n"
        << "  forge port 443 -h github.com\n";
}

bool initSockets() {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
    return true;
#endif
}

void cleanupSockets() {
#ifdef _WIN32
    WSACleanup();
#endif
}

bool isPortOpen(const std::string& host, int port, double& latencyMs, int timeoutMs = 300) {
    struct addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::string portStr = std::to_string(port);
    if (getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res) != 0) {
        return false;
    }

#ifdef _WIN32
    SOCKET sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == INVALID_SOCKET) { freeaddrinfo(res); return false; }
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
#else
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) { freeaddrinfo(res); return false; }
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif

    auto start = std::chrono::high_resolution_clock::now();
    connect(sock, res->ai_addr, static_cast<int>(res->ai_addrlen));

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(sock, &fdset);

    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeoutMs * 1000;

    bool connected = false;
    if (select(static_cast<int>(sock + 1), nullptr, &fdset, nullptr, &tv) == 1) {
        int so_error = 0;
        socklen_t len = sizeof(so_error);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&so_error), &len);
        if (so_error == 0) {
            connected = true;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    latencyMs = std::chrono::duration<double, std::milli>(end - start).count();

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    freeaddrinfo(res);

    return connected;
}

} // anonymous namespace

int runPort(int argc, char* argv[]) {
    enableConsoleEncoding();

    std::string host = "127.0.0.1";
    int specificPort = 0;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            printPortHelp();
            return 0;
        } else if ((arg == "-h" || arg == "--host") && i + 1 < argc) {
            host = argv[++i];
        } else if (arg == "--check" && i + 1 < argc) {
            specificPort = std::stoi(argv[++i]);
        } else if (specificPort == 0 && arg[0] != '-') {
            size_t colon = arg.find(':');
            if (colon != std::string::npos) {
                host = arg.substr(0, colon);
                specificPort = std::stoi(arg.substr(colon + 1));
            } else {
                specificPort = std::stoi(arg);
            }
        }
    }

    if (!initSockets()) {
        std::cerr << "  [!] Failed to initialize socket subsystem.\n\n";
        return 1;
    }

    std::cout << "\nForge Network Port & Service Health\n";
    std::cout << "--------------------------------------------\n";

    if (specificPort > 0) {
        std::cout << "  Target Host : " << host << "\n";
        std::cout << "  Target Port : " << specificPort << "\n\n";

        double latency = 0.0;
        bool open = isPortOpen(host, specificPort, latency);
        
        if (open) {
            std::cout << "  [OPEN / LISTENING] Port " << specificPort << " is active (" << latency << " ms).\n\n";
        } else {
            std::cout << "  [CLOSED / AVAILABLE] Port " << specificPort << " is not accepting connections.\n\n";
        }

        cleanupSockets();
        return open ? 0 : 1;
    }

    std::cout << "  Target Host : " << host << " (Local Dev Matrix)\n\n";

    std::vector<PortTarget> commonPorts = {
        {80, "HTTP Web Server"},
        {443, "HTTPS Web Server"},
        {3000, "Node / React / Vue Dev Server"},
        {5000, "Flask / ASP.NET Dev Server"},
        {5432, "PostgreSQL Database"},
        {6379, "Redis Cache Server"},
        {8000, "Django / FastAPI Dev Server"},
        {8080, "Spring / Vite / General Web"},
        {27017, "MongoDB Database"}
    };

    std::cout << std::left
              << std::setw(10) << "Port"
              << std::setw(34) << "Service / Description"
              << std::setw(20) << "Status"
              << "\n";
    std::cout << "--------------------------------------------------------\n";

    for (const auto& pt : commonPorts) {
        double latency = 0.0;
        bool active = isPortOpen(host, pt.port, latency);
        std::cout << std::left
                  << std::setw(10) << pt.port
                  << std::setw(34) << pt.defaultService
                  << std::setw(20) << (active ? "[OCCUPIED / OPEN]" : "[AVAILABLE]")
                  << "\n";
    }

    std::cout << "--------------------------------------------------------\n\n";

    cleanupSockets();
    return 0;
}

} // namespace forge