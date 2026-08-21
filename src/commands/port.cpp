#include "forge/commands/port.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
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
        << "Forge Network Port & Service Health Analyzer\n\n"

        << "Usage:\n"
        << "  forge port [options]\n\n"

        << "Options:\n"
        << "  --check <port>          Check connectivity to a specific port\n"
        << "  -h, --help              Show this help message\n";
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

bool isPortOpen(const std::string& host, int port, int timeoutMs = 200) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) return false;

    // Non-blocking socket setup
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
#else
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(static_cast<u_short>(port));
    inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);

    connect(sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(sock, &fdset);

    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeoutMs * 1000;

    bool connected = false;
    if (select(static_cast<int>(sock + 1), nullptr, &fdset, nullptr, &tv) == 1) {
        int so_error;
        socklen_t len = sizeof(so_error);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&so_error), &len);
        if (so_error == 0) {
            connected = true;
        }
    }

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif

    return connected;
}

} // anonymous namespace

int runPort(int argc, char* argv[]) {
    enableConsoleEncoding();

    int specificPort = 0;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--check" && i + 1 < argc) {
            specificPort = std::stoi(argv[++i]);
        } else if (arg == "-h" || arg == "--help") {
            printPortHelp();
            return 0;
        }
    }

    if (!initSockets()) {
        std::cerr << "  [!] Failed to initialize socket subsystem.\n\n";
        return 1;
    }

    std::cout << "\nForge Network Port & Service Health\n";
    std::cout << "--------------------------------------------\n\n";

    if (specificPort > 0) {
        bool open = isPortOpen("127.0.0.1", specificPort);
        std::cout << "  Port " << specificPort << ": " 
                  << (open ? "[OCCUPIED / LISTENING]" : "[FREE / AVAILABLE]") << "\n\n";
        cleanupSockets();
        return 0;
    }

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
              << std::setw(32) << "Service / Description"
              << std::setw(20) << "Status"
              << "\n";
    std::cout << "--------------------------------------------------------\n";

    for (const auto& pt : commonPorts) {
        bool active = isPortOpen("127.0.0.1", pt.port);
        std::cout << std::left
                  << std::setw(10) << pt.port
                  << std::setw(32) << pt.defaultService
                  << std::setw(20) << (active ? "[OCCUPIED]" : "[AVAILABLE]")
                  << "\n";
    }

    std::cout << "--------------------------------------------------------\n\n";

    cleanupSockets();
    return 0;
}

} // namespace forge