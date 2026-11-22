@echo off
REM Streetlight Repair Desk - one-click build & run (Windows)
REM Double-click this file: builds streetlight_app.exe with g++ and runs it.
cd /d %~dp0

echo Building Streetlight Repair Desk...
g++ -std=c++17 -O2 -Wall -Wextra Source_Code\main.cpp -o streetlight_app.exe

if %errorlevel% neq 0 (
    echo.
    echo Build FAILED.
    echo Make sure MinGW g++ is installed and its bin folder is in PATH.
    echo Test it in CMD:  g++ --version
    pause
    exit /b 1
)

echo Build OK: streetlight_app.exe
echo.
streetlight_app.exe
pause
