#!/usr/bin/env bash
# Streetlight Repair Desk - one-click build & run (Linux / macOS)
set -e
cd "$(dirname "$0")"

echo "Building Streetlight Repair Desk..."
g++ -std=c++17 -O2 -Wall -Wextra Source_Code/main.cpp -o streetlight_app

echo "Build OK: ./streetlight_app"
echo
./streetlight_app
