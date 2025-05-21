cmake_minimum_required(VERSION 3.14)
project(TAIFEXPacketParser)

# ✅ 使用 C++17 標準
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ 顯示所有編譯細節（方便除錯）
set(CMAKE_VERBOSE_MAKEFILE ON)

# ✅ 編譯器特定設定（MinGW 支援 UTF-8）
if (MINGW)
    add_compile_options(-Wall -Wextra -Wpedantic -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

# ✅ 包含標頭檔資料夾
include_directories(include)

# ✅ 將所有模組的 cpp 加入編譯
file(GLOB_RECURSE PARSER_SRC     src/parser/*.cpp)
file(GLOB_RECURSE CONTROLLER_SRC src/controller/*.cpp)
file(GLOB_RECURSE DISPATCHER_SRC src/dispatcher/*.cpp)
file(GLOB_RECURSE STORAGE_SRC    src/storage/*.cpp)
file(GLOB_RECURSE DATA_SRC       src/data/*.cpp)
file(GLOB_RECURSE OUTPUT_SRC     src/output/*.cpp)

# ✅ 主程式進入點
set(MAIN_SRC src/main.cpp)

# ✅ 建立執行檔
add_executable(TAIFEXMain
    ${MAIN_SRC}
    ${PARSER_SRC}
    ${CONTROLLER_SRC}
    ${DISPATCHER_SRC}
    ${STORAGE_SRC}
    ${DATA_SRC}
    ${OUTPUT_SRC}
)
