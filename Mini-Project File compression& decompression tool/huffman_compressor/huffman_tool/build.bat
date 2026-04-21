@echo off
REM ── Huffman Compressor — Windows Build Script ────────────────────────────
echo.
echo ╔══════════════════════════════════════════╗
echo ║    HUFFMAN COMPRESSOR — Build Script     ║
echo ╚══════════════════════════════════════════╝
echo.

where g++ >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] g++ not found. Please install MinGW-w64 or MSYS2.
    echo         https://www.msys2.org/
    pause
    exit /b 1
)

echo Compiling src\huffman.cpp ...
g++ -std=c++17 -O2 -Wall -o huffman.exe src\huffman.cpp

if %ERRORLEVEL% EQU 0 (
    echo [OK] Binary built: huffman.exe
    echo.
    echo Usage:
    echo   huffman.exe compress   input.txt   output.huf
    echo   huffman.exe decompress output.huf  restored.txt
) else (
    echo [ERROR] Compilation failed.
)

echo.
echo  Browser UI: double-click index.html
echo.
pause
