cmake_minimum_required(VERSION 3.12)
project(cvrpptpl LANGUAGES CXX)

enable_testing()
find_package(GTest CONFIG REQUIRED)
# Set C++ standard 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather all source files
file(GLOB_RECURSE PROBLEM_SOURCES src/problem/*.cpp)
file(GLOB_RECURSE HEURISTIC_SOURCES src/heuristic/*.cpp)

add_library(cvrpptpl_lib ${PROBLEM_SOURCES} ${HEURISTIC_SOURCES})

# Add the main executable
add_executable(cvrpptpl "main.cpp")
target_link_libraries(cvrpptpl PRIVATE cvrpptpl_lib)

add_subdirectory(tests)