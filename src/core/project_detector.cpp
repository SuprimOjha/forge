#include "forge/core/project_detector.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace forge {

namespace {

/*
 * Check whether a file contains a piece of text.
 */
bool fileContains(
    const fs::path& file,
    const std::string& text
) {
    std::ifstream input(file);

    if (!input.is_open()) {
        return false;
    }

    std::string line;

    while (std::getline(input, line)) {

        if (line.find(text) != std::string::npos) {
            return true;
        }
    }

    return false;
}


/*
 * Check whether a command is available.
 */
bool commandAvailable(
    const std::string& command
) {

#ifdef _WIN32

    const std::string check =
        "where " + command + " >nul 2>&1";

#else

    const std::string check =
        "command -v " + command +
        " >/dev/null 2>&1";

#endif

    return std::system(check.c_str()) == 0;
}


/*
 * Detect Node.js version requirement
 * from package.json.
 */
std::string findNodeRequirement(
    const fs::path& packageJson
) {

    std::ifstream input(packageJson);

    if (!input.is_open()) {
        return "";
    }

    std::string line;
    bool inEngines = false;

    while (std::getline(input, line)) {

        if (line.find("\"engines\"") !=
            std::string::npos) {

            inEngines = true;
            continue;
        }

        if (!inEngines) {
            continue;
        }

        if (line.find("\"node\"") !=
            std::string::npos) {

            const std::size_t colon =
                line.find(':');

            if (colon == std::string::npos) {
                return "";
            }

            const std::size_t start =
                line.find('"', colon);

            if (start == std::string::npos) {
                return "";
            }

            const std::size_t end =
                line.find('"', start + 1);

            if (end == std::string::npos) {
                return "";
            }

            return line.substr(
                start + 1,
                end - start - 1
            );
        }

        if (line.find('}') !=
            std::string::npos) {

            inEngines = false;
        }
    }

    return "";
}


/*
 * Parse dependencies from package.json.
 */
std::vector<DependencyInfo> parseDependencies(
    const fs::path& packageJson
) {

    std::vector<DependencyInfo> dependencies;

    std::ifstream input(packageJson);

    if (!input.is_open()) {
        return dependencies;
    }

    std::string line;

    bool inDependencies = false;
    bool inDevDependencies = false;

    while (std::getline(input, line)) {

        /*
         * dependencies
         */

        if (line.find("\"dependencies\"") !=
            std::string::npos) {

            inDependencies = true;
            inDevDependencies = false;

            continue;
        }


        /*
         * devDependencies
         */

        if (line.find("\"devDependencies\"") !=
            std::string::npos) {

            inDependencies = false;
            inDevDependencies = true;

            continue;
        }


        /*
         * Ignore everything outside
         * dependency sections.
         */

        if (!inDependencies &&
            !inDevDependencies) {

            continue;
        }


        /*
         * Detect end of section.
         */

        if (line.find('}') !=
            std::string::npos) {

            inDependencies = false;
            inDevDependencies = false;

            continue;
        }


        /*
         * Find dependency name.
         */

        const std::size_t firstQuote =
            line.find('"');

        if (firstQuote ==
            std::string::npos) {

            continue;
        }

        const std::size_t secondQuote =
            line.find('"', firstQuote + 1);

        if (secondQuote ==
            std::string::npos) {

            continue;
        }

        const std::string name =
            line.substr(
                firstQuote + 1,
                secondQuote - firstQuote - 1
            );


        /*
         * Find colon.
         */

        const std::size_t colon =
            line.find(':', secondQuote);

        if (colon ==
            std::string::npos) {

            continue;
        }


        /*
         * Find version.
         */

        const std::size_t versionStart =
            line.find('"', colon);

        if (versionStart ==
            std::string::npos) {

            continue;
        }

        const std::size_t versionEnd =
            line.find('"', versionStart + 1);

        if (versionEnd ==
            std::string::npos) {

            continue;
        }


        /*
         * Create dependency.
         */

        DependencyInfo dependency;

        dependency.name = name;

        dependency.version =
            line.substr(
                versionStart + 1,
                versionEnd - versionStart - 1
            );

        dependency.development =
            inDevDependencies;

        dependencies.push_back(
            dependency
        );
    }

    return dependencies;
}

} // anonymous namespace


/*
 * Detect current project.
 */

ProjectInfo detectProject() {

    ProjectInfo info;

    const fs::path currentPath =
        fs::current_path();


    /*
     * Basic information.
     */

    info.path =
        currentPath.string();

    info.name =
        currentPath.filename().string();

    if (info.name.empty()) {

        info.name =
            currentPath.root_name().string();
    }


    /*
     * Git
     */

    /*
 * Git repository detection.
 *
 * Search the current directory and all
 * parent directories for a Git repository.
 */

fs::path gitPath = currentPath;

while (!gitPath.empty()) {

    if (fs::exists(gitPath / ".git")) {

        info.gitRepository = true;

        info.gitRoot =
            gitPath.string();

        /*
         * Only show .git as a detected file
         * when it belongs directly to the
         * current project directory.
         */

        if (gitPath == currentPath) {

            info.detectedFiles.push_back(
                ".git"
            );
        }

        break;
    }

    const fs::path parent =
        gitPath.parent_path();

    if (parent == gitPath) {
        break;
    }

    gitPath = parent;
}


    /*
     * Node.js
     */

    const fs::path packageJson =
        currentPath / "package.json";

    if (fs::exists(packageJson)) {

        info.detectedFiles.push_back(
            "package.json"
        );

        info.type = "Node.js";


        /*
         * Dependencies.
         */

        info.dependencies =
            parseDependencies(packageJson);

        
                       /*
 * Check node_modules
 */

         const fs::path nodeModules =
          currentPath / "node_modules";

           info.nodeModulesExists =
            fs::exists(nodeModules) &&
          fs::is_directory(nodeModules);

         if (info.nodeModulesExists) {

          for (const auto& dependency :
            info.dependencies) {
 
            const fs::path dependencyPath =
              nodeModules / dependency.name;

              if (fs::exists(dependencyPath)) {
 
              info.installedDependencies++;
 
             } else {

               info.missingDependencies++;
             }
            }
            } 
        /*
         * Node.js / npm availability.
         */

        info.nodeAvailable =
            commandAvailable("node");

        info.npmAvailable =
            commandAvailable("npm");


        /*
         * Node.js version requirement.
         */

        info.requiredNodeVersion =
            findNodeRequirement(packageJson);


        /*
         * TypeScript.
         */

        if (fs::exists(
                currentPath / "tsconfig.json"
            )) {

            info.detectedFiles.push_back(
                "tsconfig.json"
            );

            info.type = "TypeScript";
        }


        /*
         * Package manager.
         */

        if (fs::exists(
                currentPath / "package-lock.json"
            )) {

            info.detectedFiles.push_back(
                "package-lock.json"
            );

            info.packageManager = "npm";

        } else if (fs::exists(
                currentPath / "yarn.lock"
            )) {

            info.detectedFiles.push_back(
                "yarn.lock"
            );

            info.packageManager = "yarn";

        } else if (fs::exists(
                currentPath / "pnpm-lock.yaml"
            )) {

            info.detectedFiles.push_back(
                "pnpm-lock.yaml"
            );

            info.packageManager = "pnpm";

        } else if (fs::exists(
                currentPath / "bun.lock"
            )) {

            info.detectedFiles.push_back(
                "bun.lock"
            );

            info.packageManager = "bun";

        } else if (fs::exists(
                currentPath / "bun.lockb"
            )) {

            info.detectedFiles.push_back(
                "bun.lockb"
            );

            info.packageManager = "bun";
        }


        /*
         * Next.js.
         */

        if (
            fs::exists(
                currentPath / "next.config.js"
            ) ||
            fs::exists(
                currentPath / "next.config.ts"
            ) ||
            fs::exists(
                currentPath / "next.config.mjs"
            ) ||
            fileContains(
                packageJson,
                "\"next\""
            )
        ) {

            info.frameworks.push_back(
                "Next.js"
            );
        }


        /*
         * Vite.
         */

        if (
            fs::exists(
                currentPath / "vite.config.js"
            ) ||
            fs::exists(
                currentPath / "vite.config.ts"
            ) ||
            fileContains(
                packageJson,
                "\"vite\""
            )
        ) {

            info.frameworks.push_back(
                "Vite"
            );
        }


        /*
         * React.
         */

        if (fileContains(
                packageJson,
                "\"react\""
            )) {

            info.frameworks.push_back(
                "React"
            );
        }


        /*
         * Create React App.
         */

        if (fileContains(
                packageJson,
                "\"react-scripts\""
            )) {

            info.frameworks.push_back(
                "Create React App"
            );
        }


        /*
         * Express.
         */

        if (fileContains(
                packageJson,
                "\"express\""
            )) {

            info.frameworks.push_back(
                "Express"
            );
        }
    }


    /*
     * Python.
     */

    if (fs::exists(
            currentPath / "requirements.txt"
        )) {

        info.detectedFiles.push_back(
            "requirements.txt"
        );

        if (info.type.empty()) {
            info.type = "Python";
        }
    }

    if (fs::exists(
            currentPath / "pyproject.toml"
        )) {

        info.detectedFiles.push_back(
            "pyproject.toml"
        );

        if (info.type.empty()) {
            info.type = "Python";
        }
    }


    /*
     * Django.
     */

    if (fs::exists(
            currentPath / "manage.py"
        )) {

        info.detectedFiles.push_back(
            "manage.py"
        );

        info.type =
            "Django / Python";

        info.frameworks.push_back(
            "Django"
        );
    }


    /*
     * C++ / CMake.
     */

    if (fs::exists(
            currentPath / "CMakeLists.txt"
        )) {

        info.detectedFiles.push_back(
            "CMakeLists.txt"
        );

        if (info.type.empty()) {

            info.type =
                "C++ / CMake";
        }
    }


    /*
     * Rust.
     */

    if (fs::exists(
            currentPath / "Cargo.toml"
        )) {

        info.detectedFiles.push_back(
            "Cargo.toml"
        );

        if (info.type.empty()) {
            info.type = "Rust";
        }
    }


    /*
     * Go.
     */

    if (fs::exists(
            currentPath / "go.mod"
        )) {

        info.detectedFiles.push_back(
            "go.mod"
        );

        if (info.type.empty()) {
            info.type = "Go";
        }
    }


    /*
     * Docker.
     */

    if (fs::exists(
            currentPath / "Dockerfile"
        )) {

        info.detectedFiles.push_back(
            "Dockerfile"
        );
    }

    if (fs::exists(
            currentPath / "docker-compose.yml"
        )) {

        info.detectedFiles.push_back(
            "docker-compose.yml"
        );
    }


    /*
     * Unknown project.
     */

    if (info.type.empty()) {
        info.type = "Unknown";
    }

    return info;
}

} // namespace forge