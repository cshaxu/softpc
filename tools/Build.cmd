@echo off
setlocal EnableExtensions DisableDelayedExpansion
set "build_result=1"
set "build_pause=1"
if /i "%~1"=="--no-pause" set "build_pause=0"
if not "%~1"=="" if /i not "%~1"=="--no-pause" goto usage
if not "%~2"=="" goto usage
pushd "%~dp0.."
if errorlevel 1 goto finish
for %%T in (cmake.exe ninja.exe ccache.exe gcc.exe i686-w64-mingw32-gcc.exe) do (
    where.exe %%T >nul 2>&1
    if errorlevel 1 (
        echo Missing required tool on PATH: %%T
        goto done
    )
)
call :build x86
if errorlevel 1 goto failed
call :build x64
if errorlevel 1 goto failed
set "build_result=0"
echo Published assets\binary\softpc32.exe and softpc64.exe.
goto done

:build
echo Building and publishing %1...
cmake.exe --preset ninja-ccache-gcc-%1-release
if errorlevel 1 exit /b %errorlevel%
cmake.exe --build --preset ninja-ccache-package-%1
exit /b %errorlevel%

:failed
set "build_result=%errorlevel%"
echo Build failed with exit code %build_result%. Both packages may not be updated.
:done
popd
goto finish
:usage
echo Usage: Build.cmd [--no-pause]
:finish
if "%build_pause%"=="1" pause
exit /b %build_result%
