@echo off
setlocal enabledelayedexpansion

:: ✅ 建立 build 資料夾
if not exist build (
    mkdir build
)

:: ✅ 進入 build
cd build

:: ✅ 使用 MinGW Makefiles 產生 Makefile
cmake -G "MinGW Makefiles" ..

:: ✅ 編譯專案
mingw32-make

:: ✅ 返回上一層
cd ..

echo.
echo 🔧 編譯完成，執行檔為：build\TAIFEXMain.exe
echo.

endlocal
pause
