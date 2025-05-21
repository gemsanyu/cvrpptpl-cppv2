cmake_minimum_required(VERSION 3.12)
project(cvrpptpl LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather all source files
file(GLOB_RECURSE PROBLEM_SOURCES src/problem/*.cpp)

# Add the main executable
add_executable(cvrpptpl "main.cpp" ${PROBLEM_SOURCES})

# TODO: Add tests and install targets
