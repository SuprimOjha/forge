#include "forge/core/project_detector.hpp"

#include <cctype>
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
 * Parse C++ standard and minimum version from CMakeLists.txt.
 */
void parseCMakeLists(
    const fs::path& cmakeListsPath,
    std::string& cxxStandard,
    std::string& minVersion
) {
    std::ifstream input(cmakeListsPath);

    if (!input.is_open()) {
        return;
    }

    std::string line;

    while (std::getline(input, line)) {

        const std::size_t stdPos =
            line.find("CMAKE_CXX_STANDARD");

        if (stdPos != std::string::npos) {

            for (std::size_t i = stdPos + 18; i < line.size(); ++i) {

                if (std::isdigit(line[i])) {

                    std::size_t len = 0;

                    while (i + len < line.size() && std::isdigit(line[i + len])) {
                        len++;
                    }

                    cxxStandard = line.substr(i, len);
                    break;
                }
            }
        }

        const std::size_t reqPos =
            line.find("cmake_minimum_required");

        if (reqPos != std::string::npos) {

            const std::size_t vPos =
                line.find("VERSION", reqPos);

            if (vPos != std::string::npos) {

                std::size_t start = vPos + 7;

                while (start < line.size() && (line[start] == ' ' || line[start] == '=')) {
                    start++;
                }

                std::size_t end = start;

                while (end < line.size() && (std::isdigit(line[end]) || line[end] == '.')) {
                    end++;
                }

                if (end > start) {
                    minVersion = line.substr(start, end - start);
                }
            }
        }
    }
}


/*
 * Parse active CMake generator from build/CMakeCache.txt.
 */
void parseCMakeCache(
    const fs::path& cachePath,
    std::string& generator
) {
    std::ifstream input(cachePath);

    if (!input.is_open()) {
        return;
    }

    std::string line;

    while (std::getline(input, line)) {

        if (line.rfind("CMAKE_GENERATOR:INTERNAL=", 0) == 0) {
            generator = line.substr(25);
        }
    }
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

        if (line.find("\"dependencies\"") !=
            std::string::npos) {

            inDependencies = true;
            inDevDependencies = false;
            continue;
        }

        if (line.find("\"devDependencies\"") !=
            std::string::npos) {

            inDependencies = false;
            inDevDependencies = true;
            continue;
        }

        if (!inDependencies && !inDevDependencies) {
            continue;
        }

        if (line.find('}') != std::string::npos) {
            inDependencies = false;
            inDevDependencies = false;
            continue;
        }

        const std::size_t firstQuote = line.find('"');
        if (firstQuote == std::string::npos) continue;

        const std::size_t secondQuote = line.find('"', firstQuote + 1);
        if (secondQuote == std::string::npos) continue;

        const std::string name = line.substr(
            firstQuote + 1,
            secondQuote - firstQuote - 1
        );

        const std::size_t colon = line.find(':', secondQuote);
        if (colon == std::string::npos) continue;

        const std::size_t versionStart = line.find('"', colon);
        if (versionStart == std::string::npos) continue;

        const std::size_t versionEnd = line.find('"', versionStart + 1);
        if (versionEnd == std::string::npos) continue;

        DependencyInfo dependency;
        dependency.name = name;
        dependency.version = line.substr(
            versionStart + 1,
            versionEnd - versionStart - 1
        );
        dependency.development = inDevDependencies;

        if (dependency.name == "react-scripts") {
            dependency.status = "WARNING: Legacy Create React App dependency";
        } else {
            dependency.status = "OK";
        }

        dependencies.push_back(dependency);
    }

    return dependencies;
}


/*
 * Parse scripts from package.json.
 */
std::vector<ScriptInfo> parseScripts(
    const fs::path& packageJson
) {
    std::vector<ScriptInfo> scripts;

    std::ifstream input(packageJson);

    if (!input.is_open()) {
        return scripts;
    }

    std::string line;
    bool inScripts = false;

    while (std::getline(input, line)) {

        if (line.find("\"scripts\"") != std::string::npos) {
            inScripts = true;
            continue;
        }

        if (!inScripts) continue;

        if (line.find('}') != std::string::npos) break;

        const std::size_t firstQuote = line.find('"');
        if (firstQuote == std::string::npos) continue;

        const std::size_t secondQuote = line.find('"', firstQuote + 1);
        if (secondQuote == std::string::npos) continue;

        const std::string name = line.substr(
            firstQuote + 1,
            secondQuote - firstQuote - 1
        );

        const std::size_t colon = line.find(':', secondQuote);
        if (colon == std::string::npos) continue;

        const std::size_t valueStart = line.find('"', colon);
        if (valueStart == std::string::npos) continue;

        const std::size_t valueEnd = line.find('"', valueStart + 1);
        if (valueEnd == std::string::npos) continue;

        ScriptInfo script;
        script.name = name;
        script.command = line.substr(
            valueStart + 1,
            valueEnd - valueStart - 1
        );

        scripts.push_back(script);
    }

    return scripts;
}


/*
 * Generate project diagnostics.
 */
void generateProjectIssues(
    ProjectInfo& info
) {

    /*
     * Git
     */
    if (!info.gitRepository) {

        ProjectIssue issue;
        issue.severity = ProjectIssue::Severity::Warning;
        issue.message = "Git repository not detected";
        issue.suggestion = "Initialize Git with: git init";

        info.issues.push_back(issue);
    }


    /*
     * C++ / CMake Diagnostics
     */
    if (info.type == "C++ / CMake") {

        if (!info.cmakeAvailable) {

            ProjectIssue issue;
            issue.severity = ProjectIssue::Severity::Error;
            issue.message = "cmake binary not found";
            issue.suggestion = "Install CMake and ensure it is available in system PATH";

            info.issues.push_back(issue);
        }

        if (!info.cmakeConfigured) {

            ProjectIssue issue;
            issue.severity = ProjectIssue::Severity::Error;
            issue.message = "CMake build directory not configured";
            issue.suggestion = "Run: cmake -B build";

            info.issues.push_back(issue);
        }
    }


    /*
     * Node.js
     */
    if (info.type == "Node.js" || info.type == "TypeScript") {

        if (!info.nodeAvailable) {

            ProjectIssue issue;
            issue.severity = ProjectIssue::Severity::Error;
            issue.message = "Node.js not found";
            issue.suggestion = "Install Node.js and make sure it is available in PATH";

            info.issues.push_back(issue);
        }

        if (info.packageManager == "npm" && !info.npmAvailable) {

            ProjectIssue issue;
            issue.severity = ProjectIssue::Severity::Error;
            issue.message = "npm not found";
            issue.suggestion = "Install Node.js, which includes npm";

            info.issues.push_back(issue);
        }

        if (!info.nodeModulesExists) {

            ProjectIssue issue;
            issue.severity = ProjectIssue::Severity::Error;
            issue.message = "node_modules not found";

            if (info.packageManager == "npm") {
                issue.suggestion = "Run: npm install";
            } else if (info.packageManager == "yarn") {
                issue.suggestion = "Run: yarn install";
            } else if (info.packageManager == "pnpm") {
                issue.suggestion = "Run: pnpm install";
            } else if (info.packageManager == "bun") {
                issue.suggestion = "Run: bun install";
            } else {
                issue.suggestion = "Install the project's dependencies";
            }

            info.issues.push_back(issue);
        }

        if (info.missingDependencies > 0) {

            ProjectIssue issue;
            issue.severity = ProjectIssue::Severity::Error;
            issue.message = std::to_string(info.missingDependencies) + " project dependency(s) missing";

            if (info.packageManager == "npm") {
                issue.suggestion = "Run: npm install";
            } else if (info.packageManager == "yarn") {
                issue.suggestion = "Run: yarn install";
            } else if (info.packageManager == "pnpm") {
                issue.suggestion = "Run: pnpm install";
            } else if (info.packageManager == "bun") {
                issue.suggestion = "Run: bun install";
            } else {
                issue.suggestion = "Install the missing dependencies";
            }

            info.issues.push_back(issue);
        }

        if (info.dependencies.empty()) {

            ProjectIssue issue;
            issue.severity = ProjectIssue::Severity::Warning;
            issue.message = "No dependencies detected";
            issue.suggestion = "Check package.json dependencies";

            info.issues.push_back(issue);
        }

        if (info.packageManager.empty()) {

            ProjectIssue issue;
            issue.severity = ProjectIssue::Severity::Warning;
            issue.message = "Package manager not detected";
            issue.suggestion = "Add a package manager lock file";

            info.issues.push_back(issue);
        }
    }


    /*
     * Generate recommendations
     */
    info.recommendations.clear();

    for (const auto& issue : info.issues) {

        if (issue.message == "node_modules not found") {
            info.recommendations.push_back("Run: npm install");
        }

        if (issue.message == "CMake build directory not configured") {
            info.recommendations.push_back("Run: cmake -B build");
        }

        if (issue.message.find("react-scripts") != std::string::npos) {
            info.recommendations.push_back("Consider migrating from Create React App to Vite");
        }

        if (issue.message.find("Git repository") != std::string::npos) {
            info.recommendations.push_back("Initialize a Git repository with: git init");
        }
    }
}

} // anonymous namespace


/*
 * Detect current project.
 */
ProjectInfo detectProject() {

    ProjectInfo info;

    const fs::path currentPath = fs::current_path();


    /*
     * Basic information.
     */
    info.path = currentPath.string();
    info.name = currentPath.filename().string();

    if (info.name.empty()) {
        info.name = currentPath.root_name().string();
    }


    /*
     * Git repository detection.
     */
    fs::path gitPath = currentPath;

    while (!gitPath.empty()) {

        if (fs::exists(gitPath / ".git")) {

            info.gitRepository = true;
            info.gitRoot = gitPath.string();

            if (gitPath == currentPath) {
                info.detectedFiles.push_back(".git");
            }

            break;
        }

        const fs::path parent = gitPath.parent_path();

        if (parent == gitPath) {
            break;
        }

        gitPath = parent;
    }


    /*
     * Global tool availability
     */
    info.cmakeAvailable = commandAvailable("cmake");


    /*
     * C++ / CMake.
     */
    const fs::path cmakeLists = currentPath / "CMakeLists.txt";

    if (fs::exists(cmakeLists)) {

        info.detectedFiles.push_back("CMakeLists.txt");

        if (info.type.empty()) {
            info.type = "C++ / CMake";
        }

        parseCMakeLists(cmakeLists, info.cxxStandard, info.cmakeMinimumVersion);

        const fs::path cachePath = currentPath / "build" / "CMakeCache.txt";
        const fs::path rootCachePath = currentPath / "CMakeCache.txt";

        if (fs::exists(cachePath)) {
            info.cmakeConfigured = true;
            parseCMakeCache(cachePath, info.cmakeGenerator);
        } else if (fs::exists(rootCachePath)) {
            info.cmakeConfigured = true;
            parseCMakeCache(rootCachePath, info.cmakeGenerator);
        }
    }


    /*
     * Node.js.
     */
    const fs::path packageJson = currentPath / "package.json";

    if (fs::exists(packageJson)) {

        info.detectedFiles.push_back("package.json");
        info.type = "Node.js";

        info.dependencies = parseDependencies(packageJson);
        info.scripts = parseScripts(packageJson);

        const fs::path nodeModules = currentPath / "node_modules";

        info.nodeModulesExists = fs::exists(nodeModules) && fs::is_directory(nodeModules);

        if (info.nodeModulesExists) {

            for (const auto& dependency : info.dependencies) {

                const fs::path dependencyPath = nodeModules / dependency.name;

                if (fs::exists(dependencyPath)) {
                    info.installedDependencies++;
                } else {
                    info.missingDependencies++;
                }
            }
        }

        info.nodeAvailable = commandAvailable("node");
        info.npmAvailable = commandAvailable("npm");
        info.requiredNodeVersion = findNodeRequirement(packageJson);

        if (fs::exists(currentPath / "tsconfig.json")) {
            info.detectedFiles.push_back("tsconfig.json");
            info.type = "TypeScript";
        }

        if (fs::exists(currentPath / "package-lock.json")) {
            info.detectedFiles.push_back("package-lock.json");
            info.packageManager = "npm";
        } else if (fs::exists(currentPath / "yarn.lock")) {
            info.detectedFiles.push_back("yarn.lock");
            info.packageManager = "yarn";
        } else if (fs::exists(currentPath / "pnpm-lock.yaml")) {
            info.detectedFiles.push_back("pnpm-lock.yaml");
            info.packageManager = "pnpm";
        } else if (fs::exists(currentPath / "bun.lock") || fs::exists(currentPath / "bun.lockb")) {
            info.detectedFiles.push_back("bun.lock");
            info.packageManager = "bun";
        }

        if (fs::exists(currentPath / "next.config.js") || fileContains(packageJson, "\"next\"")) {
            info.frameworks.push_back("Next.js");
        }

        if (fs::exists(currentPath / "vite.config.js") || fileContains(packageJson, "\"vite\"")) {
            info.frameworks.push_back("Vite");
        }

        if (fileContains(packageJson, "\"react\"")) {
            info.frameworks.push_back("React");
        }

        if (fileContains(packageJson, "\"react-scripts\"")) {
            info.frameworks.push_back("Create React App");
        }

        if (fileContains(packageJson, "\"express\"")) {
            info.frameworks.push_back("Express");
        }
    }


    /*
     * Python.
     */
    if (fs::exists(currentPath / "requirements.txt")) {
        info.detectedFiles.push_back("requirements.txt");
        if (info.type.empty()) info.type = "Python";
    }

    if (fs::exists(currentPath / "pyproject.toml")) {
        info.detectedFiles.push_back("pyproject.toml");
        if (info.type.empty()) info.type = "Python";
    }

    if (fs::exists(currentPath / "manage.py")) {
        info.detectedFiles.push_back("manage.py");
        info.type = "Django / Python";
        info.frameworks.push_back("Django");
    }


    /*
     * Rust.
     */
    if (fs::exists(currentPath / "Cargo.toml")) {
        info.detectedFiles.push_back("Cargo.toml");
        if (info.type.empty()) info.type = "Rust";
    }


    /*
     * Go.
     */
    if (fs::exists(currentPath / "go.mod")) {
        info.detectedFiles.push_back("go.mod");
        if (info.type.empty()) info.type = "Go";
    }


    /*
     * Docker.
     */
    if (fs::exists(currentPath / "Dockerfile")) {
        info.detectedFiles.push_back("Dockerfile");
    }

    if (fs::exists(currentPath / "docker-compose.yml")) {
        info.detectedFiles.push_back("docker-compose.yml");
    }


    /*
     * Unknown project.
     */
    if (info.type.empty()) {
        info.type = "Unknown";
    }

    generateProjectIssues(info);

    return info;
}

} // namespace forge