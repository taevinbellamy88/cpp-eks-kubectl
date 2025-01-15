cmake_minimum_required(VERSION 3.10)

# Project name
project(CppKubernetesApp)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64/Debug)


# Add the executable
add_executable(CppKubernetesApp CppKubernetesApp.cpp)
