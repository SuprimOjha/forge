#include "forge/core/project_detector.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace forge {

namespace {

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

        if (line.find("\"dependencies\"") != std::string::npos) {

            inDependencies = true;
            inDevDependencies = false;

            continue;
        }

        /*
         * devDependencies
         */

        if (line.find("\"devDependencies\"") != std::string::npos) {

            inDependencies = false;
            inDevDependencies = true;

            continue;
        }

        /*
         * Ignore everything outside dependency sections
         */

        if (!inDependencies && !inDevDependencies) {
            continue;
        }

        /*
         * Detect end of dependency section
         */

        if (line.find('}') != std::string::npos) {

            inDependencies = false;
            inDevDependencies = false;

            continue;
        }

        /*
         * Find dependency name
         */

        const std::size_t firstQuote =
            line.find('"');

        if (firstQuote == std::string::npos) {
            continue;
        }

        const std::size_t secondQuote =
            line.find('"', firstQuote + 1);

        if (secondQuote == std::string::npos) {
            continue;
        }

        const std::string name =
            line.substr(
                firstQuote + 1,
                secondQuote - firstQuote - 1
            );

        /*
         * Find :
         */

        const std::size_t colon =
            line.find(':', secondQuote);

        if (colon == std::string::npos) {
            continue;
        }

        /*
         * Find version
         */

        const std::size_t versionStart =
            line.find('"', colon);

        if (versionStart == std::string::npos) {
            continue;
        }

        const std::size_t versionEnd =
            line.find('"', versionStart + 1);

        if (versionEnd == std::string::npos) {
            continue;
        }

        /*
         * Create dependency
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

        dependencies.push_back(dependency);
    }

    return dependencies;
}

}


/*
 * Detect current project
 */

ProjectInfo detectProject() {

    ProjectInfo info;

    const fs::path currentPath =
        fs::current_path();

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

    if (fs::exists(currentPath / ".git")) {

        info.gitRepository = true;

        info.detectedFiles.push_back(".git");
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

        /*
         * Dependencies
         */

        info.dependencies =
            parseDependencies(packageJson);

        info.type = "Node.js";


        /*
         * TypeScript
         */

        if (fs::exists(currentPath / "tsconfig.json")) {

            info.detectedFiles.push_back(
                "tsconfig.json"
            );

            info.type = "TypeScript";
        }


        /*
         * Package manager
         */

        if (fs::exists(currentPath / "package-lock.json")) {

            info.detectedFiles.push_back(
                "package-lock.json"
            );

            info.packageManager = "npm";

        } else if (fs::exists(currentPath / "yarn.lock")) {

            info.detectedFiles.push_back(
                "yarn.lock"
            );

            info.packageManager = "yarn";

        } else if (fs::exists(currentPath / "pnpm-lock.yaml")) {

            info.detectedFiles.push_back(
                "pnpm-lock.yaml"
            );

            info.packageManager = "pnpm";

        } else if (fs::exists(currentPath / "bun.lock")) {

            info.detectedFiles.push_back(
                "bun.lock"
            );

            info.packageManager = "bun";

        } else if (fs::exists(currentPath / "bun.lockb")) {

            info.detectedFiles.push_back(
                "bun.lockb"
            );

            info.packageManager = "bun";
        }


        /*
         * Next.js
         */

        if (
            fs::exists(currentPath / "next.config.js") ||
            fs::exists(currentPath / "next.config.ts") ||
            fs::exists(currentPath / "next.config.mjs") ||
            fileContains(packageJson, "\"next\"")
        ) {

            info.frameworks.push_back(
                "Next.js"
            );
        }


        /*
         * Vite
         */

        if (
            fs::exists(currentPath / "vite.config.js") ||
            fs::exists(currentPath / "vite.config.ts") ||
            fileContains(packageJson, "\"vite\"")
        ) {

            info.frameworks.push_back(
                "Vite"
            );
        }


        /*
         * React
         */

        if (fileContains(packageJson, "\"react\"")) {

            info.frameworks.push_back(
                "React"
            );
        }


        /*
         * Create React App
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
         * Express
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
     * Python
     */

    if (fs::exists(currentPath / "requirements.txt")) {

        info.detectedFiles.push_back(
            "requirements.txt"
        );

        if (info.type.empty()) {

            info.type = "Python";
        }
    }


    if (fs::exists(currentPath / "pyproject.toml")) {

        info.detectedFiles.push_back(
            "pyproject.toml"
        );

        if (info.type.empty()) {

            info.type = "Python";
        }
    }


    /*
     * Django
     */

    if (fs::exists(currentPath / "manage.py")) {

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
     * C++
     */

    if (fs::exists(currentPath / "CMakeLists.txt")) {

        info.detectedFiles.push_back(
            "CMakeLists.txt"
        );

        if (info.type.empty()) {

            info.type =
                "C++ / CMake";
        }
    }


    /*
     * Rust
     */

    if (fs::exists(currentPath / "Cargo.toml")) {

        info.detectedFiles.push_back(
            "Cargo.toml"
        );

        if (info.type.empty()) {

            info.type = "Rust";
        }
    }


    /*
     * Go
     */

    if (fs::exists(currentPath / "go.mod")) {

        info.detectedFiles.push_back(
            "go.mod"
        );

        if (info.type.empty()) {

            info.type = "Go";
        }
    }


    /*
     * Docker
     */

    if (fs::exists(currentPath / "Dockerfile")) {

        info.detectedFiles.push_back(
            "Dockerfile"
        );
    }


    if (fs::exists(
            currentPath /
            "docker-compose.yml"
        )) {

        info.detectedFiles.push_back(
            "docker-compose.yml"
        );
    }


    /*
     * Unknown project
     */

    if (info.type.empty()) {

        info.type = "Unknown";
    }


    return info;
}

}