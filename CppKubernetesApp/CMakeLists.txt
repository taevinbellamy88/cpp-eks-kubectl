cmake_minimum_required(VERSION 3.12)

# Project name
project(CppKubernetesApp)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64/Debug)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Add the executable
add_executable(CppKubernetesApp CppKubernetesApp.cpp)

# Link Boost libraries (if needed)
target_link_libraries(CppKubernetesApp ${Boost_LIBRARIES})