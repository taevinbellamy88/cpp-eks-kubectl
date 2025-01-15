cmake_minimum_required(VERSION 3.12)
set(VCPKG_APPLOCAL_DEPS ON)

# Project name
project(CppKubernetesApp)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64/Debug)

find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(/usr/lib/x86_64-linux-gnu)


# Add the executable
add_executable(CppKubernetesApp CppKubernetesApp.cpp)

# Link Boost libraries (if needed)
target_link_libraries(CppKubernetesApp ${Boost_LIBRARIES})