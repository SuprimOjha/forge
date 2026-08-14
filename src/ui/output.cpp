#include "forge/ui/output.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace forge::ui {

void init() {

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

}

void success(const std::string& message) {

    std::cout << "✓ " << message << '\n';

}

void error(const std::string& message) {

    std::cout << "✗ " << message << '\n';

}

void warning(const std::string& message) {

    std::cout << "⚠ " << message << '\n';

}

void info(const std::string& message) {

    std::cout << "ℹ " << message << '\n';

}

void header(const std::string& title) {

    std::cout << '\n';
    std::cout << title << '\n';
    std::cout << "────────────────────────────\n\n";

}

}