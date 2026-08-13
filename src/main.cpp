#include "forge/commands/doctor.hpp"
#include "forge/commands/version.hpp"

#include <iostream>
#include <string>

void printHelp() {

    std::cout
        << "Forge - Developer Workspace CLI\n\n"

        << "Usage:\n"
        << "  forge <command>\n\n"

        << "Commands:\n"
        << "  doctor     Check development environment\n"
        << "  version    Show Forge version\n"
        << "  help       Show this help message\n";
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printHelp();
        return 0;
    }

    const std::string command = argv[1];

    if (command == "doctor") {
        return forge::runDoctor();
    }

    if (command == "version") {
        return forge::runVersion();
    }

    if (command == "help" ||
        command == "--help" ||
        command == "-h") {

        printHelp();
        return 0;
    }

    std::cerr
        << "forge: unknown command '"
        << command
        << "'\n\n";

    printHelp();

    return 1;
}