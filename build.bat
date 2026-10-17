@echo off
echo ========================================================
echo Compiling Campus Navigation System...
echo ========================================================
g++ sorce\main.cpp sorce\data.cpp sorce\graph.cpp sorce\router.cpp sorce\search.cpp -o campus_nav.exe
if %ERRORLEVEL% EQU 0 (
    echo [SUCCESS] Build completed successfully: campus_nav.exe
) else (
    echo [ERROR] Build failed with error code %ERRORLEVEL%.
)
