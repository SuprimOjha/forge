cmake_minimum_required(VERSION 3.20)

project(forge
    VERSION 0.1.0
    DESCRIPTION "A fast cross-platform developer CLI"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(forge
    src/main.cpp
    src/commands/doctor.cpp
    src/commands/version.cpp
    src/core/detector.cpp
    src/core/path.cpp
    src/ui/output.cpp
    src/commands/config.cpp
    src/commands/env.cpp
    src/commands/check.cpp
    src/commands/env_check.cpp
    src/core/config.cpp
    src/commands/deps.cpp
    src/commands/completion.cpp
    src/commands/pack.cpp
    src/commands/tree.cpp
    src/commands/snapshot.cpp
    src/commands/health.cpp
    src/commands/arch.cpp
    src/commands/dashboard.cpp
    src/commands/package.cpp
    src/commands/build_graph.cpp
    src/commands/metrics.cpp
    src/commands/shell.cpp
    src/commands/cheat.cpp
    src/commands/search.cpp
    src/commands/profile.cpp
    src/commands/project.cpp
    src/commands/fix.cpp
    src/commands/request.cpp
    src/commands/parallel.cpp
    src/commands/status.cpp
    src/commands/diff.cpp
    src/commands/run.cpp
    src/core/process_runner.cpp
    src/commands/plugin.cpp
    src/commands/audit.cpp
    src/commands/release.cpp
    src/commands/clean.cpp
    src/commands/graph.cpp
    src/commands/alias.cpp
    src/commands/license.cpp
    src/commands/secret.cpp
    src/commands/stats.cpp
    src/commands/port.cpp
    src/commands/template.cpp
    src/commands/sweep.cpp
    src/commands/todo.cpp
    src/commands/archive.cpp
    src/commands/score.cpp
    src/commands/bench.cpp
    src/core/fix_engine.cpp
    src/commands/build.cpp  
    src/core/project_detector.cpp
    src/commands/init.cpp
)

target_include_directories(forge
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(forge PRIVATE /W4)
else()
    target_compile_options(forge PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (WIN32)
    target_link_libraries(forge PRIVATE ws2_32)
endif()