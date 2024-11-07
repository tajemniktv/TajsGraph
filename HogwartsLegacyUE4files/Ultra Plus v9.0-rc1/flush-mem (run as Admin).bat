@echo off
title flush-mem v0.6

setlocal enableextensions enabledelayedexpansion

echo [1mflush-mem v0.6 by SammiLucia[0m
echo.
echo.
echo|set /p="Checking admin priviliges... "
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"

if '%errorlevel%' NEQ '0' (
    timeout /T 1 >nul
    echo Not found.
    timeout /T 0 >nul
    echo.
    echo.
    echo Requesting admin - ONLY accept if you got this from me...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    timeout /T 2 >nul

    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\flush-mem.vbs"
    echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%temp%\flush-mem.vbs"

    "%temp%\flush-mem.vbs"
    exit /B

:gotAdmin
    if exist "%temp%\flush-mem.vbs" ( del "%temp%\flush-mem.vbs" )
    echo Found!
    pushd "%CD%"
    CD /D "%~dp0"

echo.
echo.
echo Flusing Unused Memory:
echo.

echo|set /p="Working Sets..."
EmptyStandbyList.exe workingsets
echo ........................Done

echo|set /p="Modified Page List..."
EmptyStandbyList.exe modifiedpagelist
echo ..................Done

echo|set /p="Priority 0 Standby List..."
EmptyStandbyList.exe priority0standbylist
echo .............Done

echo|set /p="Flushing Unused Memory Standby List..."
EmptyStandbyList.exe standbylist
echo .Done

for /F "tokens=1 delims=# " %%a in ('"prompt #$H# & echo on & for %%b in (1) do rem"') do set "BSPACE=%%a"
echo.
echo.
echo.
echo Done! All standby memory is cleared.
timeout /T 1 >nul
echo.
echo.
echo|set /p="Closing in (3)..."
timeout /T 1 >nul
echo|set /p=%BSPACE%%BSPACE%%BSPACE%%BSPACE%%BSPACE%%BSPACE%(2)...
timeout /T 1 >nul
echo|set /p=%BSPACE%%BSPACE%%BSPACE%%BSPACE%%BSPACE%%BSPACE%(1)...
timeout /T 1 >nul