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
    src/core/config.cpp
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