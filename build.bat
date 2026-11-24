@echo off
REM Streetlight Repair Desk - one-click build & run (Windows)
REM Build all source files into the tracked executable before running it.
cd /d "%~dp0"

set "OUTPUT=Source_Code\main.exe"
echo Building Streetlight Repair Desk...
g++ -std=c++17 -O2 -Wall -Wextra Source_Code\main.cpp Source_Code\stack_repair_history.cpp Source_Code\queue_faults_waiting.cpp Source_Code\linked_list_pole_inventory.cpp Source_Code\bst_pole_id_index.cpp -o "%OUTPUT%"

if %errorlevel% neq 0 (
    echo.
    echo Build FAILED.
    echo Make sure MinGW g++ is installed and its bin folder is in PATH.
    echo Test it in CMD:  g++ --version
    pause
    exit /b 1
)

echo Build OK: %OUTPUT%
echo.
"%OUTPUT%"
pause
