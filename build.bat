@echo off
setlocal enabledelayedexpansion

if not defined DXSDK_DIR (
    echo Error: DirectX SDK not found. Please install the DirectX SDK ^(June 2010^)
    echo and ensure DXSDK_DIR environment variable is set.
    exit /b 1
)

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "!VSWHERE!" (
    echo Error: Visual Studio not found. Please install Visual Studio with C++ development tools.
    exit /b 1
)

for /f "usebackq tokens=*" %%i in (`"!VSWHERE!" -latest -requires Microsoft.VisualStudio.Workload.NativeDesktop -property installationPath`) do (
    set VS_PATH=%%i
)

if not defined VS_PATH (
    echo Error: Visual Studio with C++ development tools not found.
    exit /b 1
)

if not exist "build" mkdir build
cd build

cmake -G "Visual Studio 17 2022" -A x64 ..
if errorlevel 1 (
    echo Error: CMake generation failed.
    exit /b 1
)

cmake --build . --config Release
if errorlevel 1 (
    echo Error: Build failed.
    exit /b 1
)

echo.
echo Build completed successfully!
echo The executable can be found in build\Release\wind.exe
echo.

cd ..
endlocal 