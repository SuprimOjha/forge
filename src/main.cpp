#include "forge/commands/doctor.hpp"
#include "forge/commands/version.hpp"
#include "forge/commands/config.hpp"
#include "forge/commands/env.hpp"
#include "forge/commands/check.hpp"
#include "forge/commands/project.hpp"
#include "forge/commands/score.hpp"
#include "forge/commands/secret.hpp"
#include "forge/commands/sweep.hpp"
#include "forge/commands/request.hpp"
#include "forge/commands/todo.hpp"
#include "forge/commands/parallel.hpp"
#include "forge/commands/graph.hpp"
#include "forge/commands/port.hpp"
#include "forge/commands/plugin.hpp"
#include "forge/commands/license.hpp"
#include "forge/commands/stats.hpp"
#include "forge/commands/fix.hpp"
#include "forge/commands/completion.hpp"
#include "forge/commands/template.hpp"
#include "forge/commands/bench.hpp"
#include "forge/commands/release.hpp"
#include "forge/commands/alias.hpp"
#include "forge/commands/build.hpp"
#include "forge/commands/diff.hpp"
#include "forge/commands/audit.hpp"
#include "forge/commands/run.hpp"
#include "forge/commands/init.hpp"
#include "forge/commands/clean.hpp"
#include "forge/commands/archive.hpp"
#include "forge/commands/status.hpp"
#include <string>
#include <vector>
#include <iostream>

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
        << "  env        Show development environment\n"
        << "  check      Check if development environment is ready\n"
        << "  project    Detect the current project\n"
        << "  fix        Run automated repair actions\n"
        << "  build      Build current project\n"
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

void printBuildHelp() {

    std::cout
        << "\n"
        << "Forge Build\n\n"

        << "Usage:\n"
        << "  forge build [extra args]\n\n"

        << "Description:\n"
        << "  Automatically build C++ (CMake) or Node.js projects.\n\n"

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

    if (command == "init") {
    std::vector<std::string> extraArgs(argv + 2, argv + argc);
    return forge::runInit(extraArgs);
} 
    if (command == "status") {
    std::vector<std::string> extraArgs(argv + 2, argv + argc);
    return forge::runStatus(extraArgs);
}
    if (command == "audit") {
    return forge::runAudit(argc, argv);
}
    if (command == "sweep") {
    return forge::runSweep(argc, argv);
}
    if (command == "bench") {
    return forge::runBench(argc, argv);
}
  if (command == "alias") {
    return forge::runAlias(argc, argv);
}
   if (command == "request") {
    return forge::runRequest(argc, argv);
}
   if (command == "secret") {
    return forge::runSecret(argc, argv);
}
   if (command == "plugin") {
    return forge::runPlugin(argc, argv);
}
if (command == "clean") {
    std::vector<std::string> extraArgs(argv + 2, argv + argc);
    return forge::runClean(extraArgs);
}
 if (command == "completion") {
    return forge::runCompletion(argc, argv);
}
if (command == "stats") {
    return forge::runStats(argc, argv);
}
   if (command == "archive") {
    return forge::runArchive(argc, argv);
}
    if (command == "graph") {
    return forge::runGraph(argc, argv);
}
    if (command == "parallel") {
    return forge::runParallel(argc, argv);
}
    if (command == "port") {
    return forge::runPort(argc, argv);
}
    if (command == "todo") {
    return forge::runTodo(argc, argv);
}
    if (command == "license") {
    return forge::runLicense(argc, argv);
}
    if (command == "run") {
    return forge::runRun(argc, argv);
}
    if (command == "template") {
    return forge::runTemplate(argc, argv);
}
    if (command == "diff") {
    return forge::runDiff(argc, argv);
}
    if (command == "score") {
    return forge::runScore(argc, argv);
}
   if (command == "env") {
    return forge::runEnv(argc, argv);
}
    
    if (command == "check") {
        return forge::runCheck();
    }
   if (command == "release") {
    return forge::runRelease(argc, argv);
}

    if (command == "project") {
        return forge::runProject();
    }

    if (command == "fix") {
        return forge::runFix();
    }

    /*
     * Build command
     */

    if (command == "build") {

        if (argc >= 3) {

            const std::string option = argv[2];

            if (
                option == "--help" ||
                option == "-h"
            ) {

                printBuildHelp();
                return 0;
            }
        }

        std::vector<std::string> extraArgs;

        for (int i = 2; i < argc; ++i) {
            extraArgs.push_back(argv[i]);
        }

        return forge::runBuild(extraArgs);
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