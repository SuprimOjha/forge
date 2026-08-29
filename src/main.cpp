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

        << "DESCRIPTION\n"
        << "--------------------------------------------\n"
        << "  Forge is a developer workspace assistant for\n"
        << "  inspecting, diagnosing, building, maintaining,\n"
        << "  analyzing and managing software projects.\n\n"

        << "CORE PROJECT COMMANDS\n"
        << "--------------------------------------------\n"
        << "  project       Detect and analyze the current project\n"
        << "                forge project\n\n"

        << "  init          Initialize a Forge workspace\n"
        << "                forge init [options]\n\n"

        << "  status        Show repository and project status\n"
        << "                forge status [options]\n\n"

        << "  build         Automatically build the project\n"
        << "                forge build [options]\n\n"

        << "  clean         Remove project build artifacts\n"
        << "                forge clean [options]\n\n"

        << "  fix           Detect problems and offer fixes\n"
        << "                forge fix\n\n"

        << "  run           Run project operations\n"
        << "                forge run [options]\n\n"


        << "DIAGNOSTICS & HEALTH\n"
        << "--------------------------------------------\n"
        << "  doctor        Check development environment\n"
        << "                forge doctor\n\n"

        << "  check         Check environment readiness\n"
        << "                forge check\n\n"

        << "  env           Show detected environment\n"
        << "                forge env [options]\n\n"

        << "  env-check     Validate environment requirements\n"
        << "                forge env-check [options]\n\n"

        << "  health        Analyze project health\n"
        << "                forge health [options]\n\n"

        << "  score         Calculate project health score\n"
        << "                forge score [options]\n\n"

        << "  metrics       Show project metrics\n"
        << "                forge metrics [options]\n\n"

        << "  stats         Show project statistics\n"
        << "                forge stats [options]\n\n"

        << "  audit         Audit project configuration\n"
        << "                forge audit [options]\n\n"


        << "CODE & DEPENDENCY ANALYSIS\n"
        << "--------------------------------------------\n"
        << "  deps          Analyze dependencies and coupling\n"
        << "                forge deps [options]\n\n"

        << "  graph         Analyze dependency relationships\n"
        << "                forge graph [options]\n\n"

        << "  build-graph   Analyze build dependencies\n"
        << "                forge build-graph [options]\n\n"

        << "  tree          Display project directory tree\n"
        << "                forge tree [options]\n\n"

        << "  diff          Analyze project changes\n"
        << "                forge diff [options]\n\n"

        << "  arch          Analyze project architecture\n"
        << "                forge arch [options]\n\n"

        << "  profile       Show project profile\n"
        << "                forge profile [options]\n\n"


        << "SECURITY & MAINTENANCE\n"
        << "--------------------------------------------\n"
        << "  secret        Detect potential secrets\n"
        << "                forge secret [options]\n\n"

        << "  license       Inspect project licenses\n"
        << "                forge license [options]\n\n"

        << "  sweep         Find unnecessary artifacts\n"
        << "                forge sweep [options]\n\n"

        << "  strip         Remove unnecessary files\n"
        << "                forge strip [options]\n\n"

        << "  todo          Find TODO/FIXME items\n"
        << "                forge todo [options]\n\n"


        << "PACKAGING & RELEASE\n"
        << "--------------------------------------------\n"
        << "  package       Manage project packaging\n"
        << "                forge package [options]\n\n"

        << "  pack          Create project packages\n"
        << "                forge pack [options]\n\n"

        << "  archive       Archive project files\n"
        << "                forge archive [options]\n\n"

        << "  snapshot      Create project snapshot\n"
        << "                forge snapshot [options]\n\n"

        << "  release       Manage project releases\n"
        << "                forge release [options]\n\n"

        << "  release-notes Generate release notes\n"
        << "                forge release-notes [options]\n\n"


        << "WORKSPACE & AUTOMATION\n"
        << "--------------------------------------------\n"
        << "  config        Manage Forge configuration\n"
        << "                forge config [options]\n\n"

        << "  alias         Manage command aliases\n"
        << "                forge alias [options]\n\n"

        << "  template      Manage project templates\n"
        << "                forge template [options]\n\n"

        << "  plugin        Manage Forge plugins\n"
        << "                forge plugin [options]\n\n"

        << "  completion    Generate shell completion\n"
        << "                forge completion [options]\n\n"

        << "  parallel      Run operations in parallel\n"
        << "                forge parallel [options]\n\n"

        << "  shell         Perform shell operations\n"
        << "                forge shell [options]\n\n"

        << "  port          Inspect project ports\n"
        << "                forge port [options]\n\n"


        << "DEVELOPER UTILITIES\n"
        << "--------------------------------------------\n"
        << "  search        Search project files\n"
        << "                forge search [options]\n\n"

        << "  cheat         Show command cheat sheet\n"
        << "                forge cheat [command]\n\n"

        << "  dashboard     Show project dashboard\n"
        << "                forge dashboard [options]\n\n"

        << "  request       Manage project requests\n"
        << "                forge request [options]\n\n"

        << "  bench         Benchmark operations\n"
        << "                forge bench [options]\n\n"


        << "VERSION\n"
        << "--------------------------------------------\n"
        << "  version       Show Forge version\n"
        << "                forge version\n\n"


        << "COMMAND HELP\n"
        << "--------------------------------------------\n"
        << "  forge <command> --help\n"
        << "  forge <command> -h\n\n"


        << "COMMON WORKFLOWS\n"
        << "--------------------------------------------\n"
        << "  forge doctor\n"
        << "  forge project\n"
        << "  forge health\n"
        << "  forge audit\n"
        << "  forge fix\n"
        << "  forge build\n"
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