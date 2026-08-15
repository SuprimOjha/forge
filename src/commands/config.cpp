#include "forge/commands/config.hpp"

#include "forge/core/config.hpp"

#include <iostream>
#include <string>

namespace forge {

void printConfigHelp() {

    std::cout
        << "\n"
        << "Forge Config\n\n"

        << "Usage:\n"
        << "  forge config <command> [options]\n\n"

        << "Commands:\n"
        << "  show                    Show current configuration\n"
        << "  path                    Show configuration file path\n"
        << "  get <key>               Get a configuration value\n"
        << "  set <key> <value>       Set a configuration value\n\n"

        << "Keys:\n"
        << "  editor                  Preferred editor\n"
        << "  shell                   Preferred shell\n\n"

        << "Options:\n"
        << "  -h, --help              Show this help message\n";
}

bool setConfigValue(
    Config& config,
    const std::string& key,
    const std::string& value
) {

    if (key == "editor") {
        config.editor = value;
        return true;
    }

    if (key == "shell") {
        config.shell = value;
        return true;
    }

    return false;
}

bool getConfigValue(
    const Config& config,
    const std::string& key,
    std::string& value
) {

    if (key == "editor") {
        value = config.editor;
        return true;
    }

    if (key == "shell") {
        value = config.shell;
        return true;
    }

    return false;
}

int runConfig(int argc, char* argv[]) {

    /*
     * forge config
     */

    if (argc < 3) {
        printConfigHelp();
        return 0;
    }

    const std::string command = argv[2];

    /*
     * Help
     */

    if (
        command == "--help" ||
        command == "-h"
    ) {
        printConfigHelp();
        return 0;
    }

    /*
     * Show
     */

    if (command == "show") {

        const Config config = loadConfig();

        std::cout
            << "\n"
            << "Forge Configuration\n"
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

    /*
     * Path
     */

    if (command == "path") {

        std::cout
            << getConfigPath()
            << "\n";

        return 0;
    }

    /*
     * Get
     */

    if (command == "get") {

        if (argc < 4) {

            std::cerr
                << "forge config get: missing key\n\n";

            printConfigHelp();

            return 1;
        }

        const std::string key = argv[3];

        const Config config = loadConfig();

        std::string value;

        if (!getConfigValue(config, key, value)) {

            std::cerr
                << "forge config get: unknown key '"
                << key
                << "'\n\n";

            std::cerr
                << "Available keys:\n"
                << "  editor\n"
                << "  shell\n";

            return 1;
        }

        if (value.empty()) {
            std::cout << "(not configured)\n";
        } else {
            std::cout << value << "\n";
        }

        return 0;
    }

    /*
     * Set
     */

    if (command == "set") {

        if (argc < 5) {

            std::cerr
                << "forge config set: missing key or value\n\n";

            std::cerr
                << "Usage:\n"
                << "  forge config set <key> <value>\n";

            return 1;
        }

        const std::string key = argv[3];
        const std::string value = argv[4];

        Config config = loadConfig();

        if (!setConfigValue(config, key, value)) {

            std::cerr
                << "forge config set: unknown key '"
                << key
                << "'\n\n";

            std::cerr
                << "Available keys:\n"
                << "  editor\n"
                << "  shell\n";

            return 1;
        }

        if (!saveConfig(config)) {

            std::cerr
                << "forge config: failed to save configuration\n";

            return 1;
        }

        std::cout
            << "✓ Set "
            << key
            << " = "
            << value
            << "\n";

        return 0;
    }

    /*
     * Unknown command
     */

    std::cerr
        << "forge config: unknown command '"
        << command
        << "'\n\n";

    printConfigHelp();

    return 1;
}

}