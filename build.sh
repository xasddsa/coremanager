#!/bin/bash
# Build script for CPUSpeedManager using CMake

set -e # Exit immediately if a command exits with a non-zero status.

# Create a build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Run CMake to configure the project
echo "Configuring project with CMake..."
cmake ..

# Run the build tool to compile the project
echo "Building project..."
cmake --build .

# Navigate back to the root directory
cd ..

echo ""
echo "Build complete."
echo "Executable is ./build/CPUSpeedManager"
echo "To install, you might need to add an install target to CMakeLists.txt and then run: cd build && sudo cmake --build . --target install"
echo "To make it available in menus, copy cpuspeedmanager.desktop to ~/.local/share/applications/ or /usr/share/applications/"
