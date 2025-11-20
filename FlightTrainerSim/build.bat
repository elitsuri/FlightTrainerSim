@echo off
echo ================================================
echo   FlightTrainerSim Build Script
echo ================================================
echo.

set BUILD_METHOD=%1
if "%BUILD_METHOD%"=="" set BUILD_METHOD=cmake

if "%BUILD_METHOD%"=="cmake" (
    echo Building with CMake...
    if exist build (
        echo Cleaning existing build directory...
        rmdir /s /q build
    )
    mkdir build
    cd build
    echo Configuring...
    cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
    echo Compiling...
    cmake --build . --config Release
    echo.
    echo ================================================
    echo Build completed!
    echo Executable: build\bin\FlightTrainerSim.exe
    echo ================================================
    cd ..
) else if "%BUILD_METHOD%"=="qmake" (
    echo Building with qmake...
    echo Generating Makefile...
    qmake FlightTrainerSim.pro
    echo Compiling...
    nmake
    echo.
    echo ================================================
    echo Build completed!
    echo Executable: release\FlightTrainerSim.exe
    echo ================================================
) else (
    echo Unknown build method: %BUILD_METHOD%
    echo Usage: build.bat [cmake^|qmake]
    exit /b 1
)
