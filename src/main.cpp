#include "forge/commands/doctor.hpp"
#include "forge/commands/version.hpp"
#include "forge/commands/config.hpp"

#include <iostream>
#include <string>

void printHelp() {

    std::cout
        << "\n"
        << "Forge - Developer Workspace CLI\n\n"

        << "Usage:\n"
        << "  forge <command> [options]\n\n"

        << "Commands:\n"
        << "  doctor     Check development environment\n"
        << "  version    Show Forge version\n"
        << "  config     Manage Forge configuration\n"
        << "  help       Show this help message\n\n"

        << "Global Options:\n"
        << "  -h, --help       Show help message\n"
        << "  --version        Show Forge version\n";
}

void printDoctorHelp() {

    std::cout
        << "\n"
        << "Forge Doctor\n\n"

        << "Usage:\n"
        << "  forge doctor [options]\n\n"

        << "Description:\n"
        << "  Check the development environment and installed tools.\n\n"

        << "Options:\n"
        << "  -h, --help       Show this help message\n";
}

int main(int argc, char* argv[]) {

    /*
     * No arguments
     */

    if (argc < 2) {
        printHelp();
        return 0;
    }

    const std::string command = argv[1];

    /*
     * Global help
     */

    if (
        command == "help" ||
        command == "--help" ||
        command == "-h"
    ) {

        printHelp();
        return 0;
    }

    /*
     * Global version
     */

    if (command == "--version") {
        return forge::runVersion();
    }

    /*
     * Doctor command
     */

    if (command == "doctor") {

        if (argc >= 3) {

            const std::string option = argv[2];

            if (
                option == "--help" ||
                option == "-h"
            ) {

                printDoctorHelp();
                return 0;
            }

            std::cerr
                << "forge doctor: unknown option '"
                << option
                << "'\n\n";

            printDoctorHelp();

            return 1;
        }

        return forge::runDoctor();
    }

    /*
     * Version command
     */

    if (command == "version") {

        if (argc >= 3) {

            const std::string option = argv[2];

            if (
                option == "--help" ||
                option == "-h"
            ) {

                std::cout
                    << "\n"
                    << "Forge Version\n\n"
                    << "Usage:\n"
                    << "  forge version\n";

                return 0;
            }

            std::cerr
                << "forge version: unknown option '"
                << option
                << "'\n";

            return 1;
        }

        return forge::runVersion();
    }

    /*
     * Config command
     */

    if (command == "config") {
        return forge::runConfig(argc, argv);
    }

    /*
     * Unknown command
     */

    std::cerr
        << "forge: unknown command '"
        << command
        << "'\n\n";

    printHelp();

    return 1;
}
