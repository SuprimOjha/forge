#include "forge/commands/config.hpp"

#include "forge/core/config.hpp"

#include <iostream>

namespace forge {

void printConfigHelp() {

    std::cout
        << "\n"
        << "Forge Config\n\n"

        << "Usage:\n"
        << "  forge config <command>\n\n"

        << "Commands:\n"
        << "  show       Show current configuration\n"
        << "  path       Show configuration file path\n\n"

        << "Options:\n"
        << "  -h, --help Show this help message\n";
}

int runConfig(int argc, char* argv[]) {

    if (argc < 3) {
        printConfigHelp();
        return 0;
    }

    const std::string command = argv[2];

    if (
        command == "--help" ||
        command == "-h"
    ) {
        printConfigHelp();
        return 0;
    }

    if (command == "path") {

        std::cout
            << getConfigPath()
            << "\n";

        return 0;
    }

    if (command == "show") {

        const Config config = loadConfig();

        std::cout
            << "\nForge Configuration\n"
            << "────────────────────────────\n\n"

            << "Config path:\n"
            << "  " << getConfigPath() << "\n\n"

            << "Version: "
            << config.version
            << "\n"

            << "Editor: "
            << (config.editor.empty()
                ? "(not configured)"
                : config.editor)
            << "\n"

            << "Shell: "
            << (config.shell.empty()
                ? "(not configured)"
                : config.shell)
            << "\n\n";

        return 0;
    }

    std::cerr
        << "forge config: unknown command '"
        << command
        << "'\n\n";

    printConfigHelp();

    return 1;
}

}