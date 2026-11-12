@echo off
echo ========================================================
echo Compiling Campus Navigation System...
echo ========================================================
REM Prefer MSYS2 ucrt64 toolchain if present (avoids broken Anaconda mingw mix).
set "PATH=C:\msys64\ucrt64\bin;%PATH%"
g++ -std=c++17 -I"./include" -Wall sorce\main.cpp sorce\data.cpp sorce\graph.cpp sorce\router.cpp sorce\search.cpp -o campus_nav.exe
if %ERRORLEVEL% EQU 0 (
    echo [SUCCESS] Build completed successfully: campus_nav.exe
) else (
    echo [ERROR] Build failed with error code %ERRORLEVEL%.
)
