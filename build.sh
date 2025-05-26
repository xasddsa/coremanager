#!/bin/bash
# Basic build script for CPUSpeedManager

set -e # Exit immediately if a command exits with a non-zero status.

# Prefer qmake6 if available, otherwise use qmake (for Qt5)
QMAKE_CMD="qmake"
if command -v qmake6 &> /dev/null; then
    QMAKE_CMD="qmake6"
elif ! command -v qmake &> /dev/null; then
    echo "Error: Neither qmake6 nor qmake found in PATH. Please install Qt development tools."
    exit 1
fi

echo "Using $(command -v $QMAKE_CMD) (version: $($QMAKE_CMD -v))"

$QMAKE_CMD CPUSpeedManager.pro
make

echo ""
echo "Build complete."
echo "Executable is ./CPUSpeedManager"
echo "To install (example): sudo make install (if defined in .pro) or sudo cp CPUSpeedManager /usr/local/bin/"
echo "To make it available in menus, copy cpuspeedmanager.desktop to ~/.local/share/applications/ or /usr/share/applications/"
