#!/bin/bash
cd ~/CppKubernetesApp/CppKubernetesApp
rm -rf build
mkdir build
cd build
cmake ..
make

