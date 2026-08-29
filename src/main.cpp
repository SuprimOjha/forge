#include "forge/commands/doctor.hpp"
#include "forge/commands/version.hpp"
#include "forge/commands/config.hpp"
#include "forge/commands/env.hpp"
#include "forge/commands/check.hpp"
#include "forge/commands/project.hpp"
#include "forge/commands/score.hpp"
#include "forge/commands/secret.hpp"
#include "forge/commands/strip.hpp"
#include "forge/commands/sweep.hpp"
#include "forge/commands/pack.hpp"
#include "forge/commands/profile.hpp"
#include "forge/commands/request.hpp"
#include "forge/commands/todo.hpp"
#include "forge/commands/package.hpp"
#include "forge/commands/build_graph.hpp"
#include "forge/commands/health.hpp"
#include "forge/commands/parallel.hpp"
#include "forge/commands/graph.hpp"
#include "forge/commands/port.hpp"
#include "forge/commands/plugin.hpp"
#include "forge/commands/license.hpp"
#include "forge/commands/cheat.hpp"
#include "forge/commands/deps.hpp"
#include "forge/commands/stats.hpp"
#include "forge/commands/fix.hpp"
#include "forge/commands/search.hpp"
#include "forge/commands/dashboard.hpp"
#include "forge/commands/completion.hpp"
#include "forge/commands/arch.hpp"
#include "forge/commands/template.hpp"
#include "forge/commands/bench.hpp"
#include "forge/commands/release.hpp"
#include "forge/commands/metrics.hpp"
#include "forge/commands/tree.hpp"
#include "forge/commands/alias.hpp"
#include "forge/commands/build.hpp"
#include "forge/commands/env_check.hpp"
#include "forge/commands/shell.hpp"
#include "forge/commands/release_notes.hpp"
#include "forge/commands/diff.hpp"
#include "forge/commands/audit.hpp"
#include "forge/commands/run.hpp"
#include "forge/commands/init.hpp"
#include "forge/commands/snapshot.hpp"
#include "forge/commands/clean.hpp"
#include "forge/commands/archive.hpp"
#include "forge/commands/status.hpp"
#include <string>
#include <vector>
#include <iostream>

void printHelp() {
    std::cout
        << "\n"
        << "Forge - Developer Workspace CLI\n"
        << "============================================\n\n"

        << "Usage:\n"
        << "  forge <command> [options]\n\n"

        << "Forge helps developers inspect, diagnose, build,\n"
        << "maintain, analyze and manage software projects.\n\n"

        << "CORE PROJECT COMMANDS\n"
        << "--------------------------------------------\n"
        << "  project       Detect and analyze the current project\n"
        << "                Usage: forge project\n\n"

        << "  init          Initialize a new Forge workspace/config\n"
        << "                Usage: forge init [options]\n\n"

        << "  status        Show repository and project status\n"
        << "                Usage: forge status [options]\n\n"

        << "  build         Build the current project automatically\n"
        << "                Usage: forge build [options]\n\n"

        << "  clean         Remove build artifacts and generated files\n"
        << "                Usage: forge clean [options]\n\n"

        << "  fix           Detect problems and offer automated fixes\n"
        << "                Usage: forge fix\n\n"

        << "  run           Run project commands through Forge\n"
        << "                Usage: forge run [options]\n\n"


        << "DIAGNOSTICS & HEALTH\n"
        << "--------------------------------------------\n"
        << "  doctor        Check development environment and tools\n"
        << "                Usage: forge doctor\n\n"

        << "  health        Analyze overall project health\n"
        << "                Usage: forge health [options]\n\n"

        << "  check         Check whether the environment is ready\n"
        << "                Usage: forge check\n\n"

        << "  env           Show detected development environment\n"
        << "                Usage: forge env [options]\n\n"

        << "  env-check     Validate environment requirements\n"
        << "                Usage: forge env-check [options]\n\n"

        << "  audit         Audit project configuration and structure\n"
        << "                Usage: forge audit [options]\n\n"

        << "  score         Calculate project health score\n"
        << "                Usage: forge score [options]\n\n"

        << "  metrics       Show project metrics and measurements\n"
        << "                Usage: forge metrics [options]\n\n"

        << "  stats         Show project statistics\n"
        << "                Usage: forge stats [options]\n\n"


        << "DEPENDENCY & CODE ANALYSIS\n"
        << "--------------------------------------------\n"
        << "  deps          Analyze dependencies and module coupling\n"
        << "                Usage: forge deps [options]\n\n"

        << "  graph         Analyze project dependency relationships\n"
        << "                Usage: forge graph [options]\n\n"

        << "  build-graph   Analyze build dependencies\n"
        << "                Usage: forge build-graph [options]\n\n"

        << "  tree          Display project directory structure\n"
        << "                Usage: forge tree [options]\n\n"

        << "  diff          Analyze project changes\n"
        << "                Usage: forge diff [options]\n\n"

        << "  profile       Analyze project/profile information\n"
        << "                Usage: forge profile [options]\n\n"

        << "  arch          Analyze project architecture\n"
        << "                Usage: forge arch [options]\n\n"


        << "PACKAGE & RELEASE TOOLS\n"
        << "--------------------------------------------\n"
        << "  package       Manage project packaging\n"
        << "                Usage: forge package [options]\n\n"

        << "  pack          Create project packages\n"
        << "                Usage: forge pack [options]\n\n"

        << "  archive       Archive project files\n"
        << "                Usage: forge archive [options]\n\n"

        << "  release       Manage project releases\n"
        << "                Usage: forge release [options]\n\n"

        << "  release-notes Generate release notes\n"
        << "                Usage: forge release-notes [options]\n\n"

        << "  snapshot      Create or inspect project snapshots\n"
        << "                Usage: forge snapshot [options]\n\n"


        << "PROJECT UTILITIES\n"
        << "--------------------------------------------\n"
        << "  config        Manage Forge configuration\n"
        << "                Usage: forge config [options]\n\n"

        << "  template      Manage project templates\n"
        << "                Usage: forge template [options]\n\n"

        << "  alias         Manage Forge command aliases\n"
        << "                Usage: forge alias [options]\n\n"

        << "  completion     Generate shell completion\n"
        << "                Usage: forge completion [options]\n\n"

        << "  shell         Run shell-related operations\n"
        << "                Usage: forge shell [options]\n\n"

        << "  port          Inspect or manage project ports\n"
        << "                Usage: forge port [options]\n\n"

        << "  parallel      Run supported operations in parallel\n"
        << "                Usage: forge parallel [options]\n\n"


        << "SECURITY & MAINTENANCE\n"
        << "--------------------------------------------\n"
        << "  secret        Scan or inspect project secrets\n"
        << "                Usage: forge secret [options]\n\n"

        << "  license       Inspect project licenses\n"
        << "                Usage: forge license [options]\n\n"

        << "  strip         Remove unnecessary project files\n"
        << "                Usage: forge strip [options]\n\n"

        << "  sweep         Sweep the project for unnecessary artifacts\n"
        << "                Usage: forge sweep [options]\n\n"

        << "  todo          Find and manage TODO items\n"
        << "                Usage: forge todo [options]\n\n"


        << "DEVELOPER TOOLS\n"
        << "--------------------------------------------\n"
        << "  bench         Benchmark Forge/project operations\n"
        << "                Usage: forge bench [options]\n\n"

        << "  search        Search project files and information\n"
        << "                Usage: forge search [options]\n\n"

        << "  cheat         Show Forge command cheat sheet\n"
        << "                Usage: forge cheat [command]\n\n"

        << "  dashboard     Show project dashboard\n"
        << "                Usage: forge dashboard [options]\n\n"

        << "  plugin        Manage Forge plugins\n"
        << "                Usage: forge plugin [options]\n\n"

        << "  request       Manage project requests/tasks\n"
        << "                Usage: forge request [options]\n\n"


        << "VERSION\n"
        << "--------------------------------------------\n"
        << "  version       Show Forge version information\n"
        << "                Usage: forge version\n\n"

        << "  --version     Show Forge version\n\n"


        << "GLOBAL OPTIONS\n"
        << "--------------------------------------------\n"
        << "  -h, --help    Show this help message\n"
        << "  --version     Show Forge version\n\n"


        << "COMMAND HELP\n"
        << "--------------------------------------------\n"
        << "  forge <command> --help\n"
        << "  forge <command> -h\n\n"

        << "Examples:\n"
        << "  forge project\n"
        << "  forge doctor\n"
        << "  forge health\n"
        << "  forge deps\n"
        << "  forge audit\n"
        << "  forge fix\n"
        << "  forge build\n"
        << "  forge clean\n"
        << "  forge status\n\n";
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
if (command == "build-graph") {
    return forge::runBuildGraph(argc, argv);
}
    if (command == "bench") {
    return forge::runBench(argc, argv);
}
   if (command == "health") {
    return forge::runHealth(argc, argv);
}
   if (command == "dashboard") {
    return forge::runDashboard(argc, argv);
}

   if (command == "env-check") {
    return forge::runEnvCheck(argc, argv);
}
if (command == "search") {
    return forge::runSearch(argc, argv);
}
if (command == "cheat") {
    return forge::runCheat(argc, argv);
}
  if (command == "alias") {
    return forge::runAlias(argc, argv);
}
   if (command == "request") {
    return forge::runRequest(argc, argv);
}
   if (command == "snapshot") {
    return forge::runSnapshot(argc, argv);
}
   if (command == "secret") {
    return forge::runSecret(argc, argv);
}
   if (command == "plugin") {
    return forge::runPlugin(argc, argv);
}
  if (command == "strip") {
    return forge::runStrip(argc, argv);
}
if (command == "clean") {
    std::vector<std::string> extraArgs(argv + 2, argv + argc);
    return forge::runClean(extraArgs);
}
 if (command == "completion") {
    return forge::runCompletion(argc, argv);
}
if (command == "deps") {
    return forge::runDeps(argc, argv);
}
if (command == "stats") {
    return forge::runStats(argc, argv);
}
   if (command == "archive") {
    return forge::runArchive(argc, argv);
}
if (command == "release-notes") {
    return forge::runReleaseNotes(argc, argv);
}
if (command == "metrics") {
    return forge::runMetrics(argc, argv);
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
if (command == "shell") {
    return forge::runShell(argc, argv);
}
    if (command == "run") {
    return forge::runRun(argc, argv);
}
    if (command == "template") {
    return forge::runTemplate(argc, argv);
}
if (command == "package") {
    return forge::runPackage(argc, argv);
}
    if (command == "diff") {
    return forge::runDiff(argc, argv);
}
    if (command == "score") {
    return forge::runScore(argc, argv);
}
if (command == "tree") {
    return forge::runTree(argc, argv);
}
if (command == "profile") {
    return forge::runProfile(argc, argv);
}
   
if (command == "pack") {
    return forge::runPack(argc, argv);
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
  if (command == "arch") {
    return forge::runArch(argc, argv);
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