#!/usr/bin/env bash
# Streetlight Repair Desk - one-click build & run (Linux / macOS)
set -e
cd "$(dirname "$0")"

echo "Building Streetlight Repair Desk..."
g++ -std=c++17 -O2 -Wall -Wextra \
    Source_Code/main.cpp \
    Source_Code/stack_repair_history.cpp \
    Source_Code/queue_faults_waiting.cpp \
    Source_Code/linked_list_pole_inventory.cpp \
    Source_Code/bst_pole_id_index.cpp \
    -o streetlight_app

echo "Build OK: ./streetlight_app"
echo
./streetlight_app
