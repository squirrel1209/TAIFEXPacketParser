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
include_directories(
    include
)

# ✅ 收集所有模組的 cpp 檔案（自動遞迴搜尋 src 目錄）
file(GLOB_RECURSE PARSER_SRC     src/parser/*.cpp)
file(GLOB_RECURSE CONTROLLER_SRC src/controller/*.cpp)
file(GLOB_RECURSE DISPATCHER_SRC src/dispatcher/*.cpp)
file(GLOB_RECURSE STORAGE_SRC    src/storage/*.cpp)
file(GLOB_RECURSE DATA_SRC       src/data/*.cpp)
file(GLOB_RECURSE OUTPUT_SRC     src/output/*.cpp)
file(GLOB_RECURSE BASE_SRC       src/base/*.cpp)        # 若有 base 模組
file(GLOB_RECURSE PROTOCOL_SRC   src/protocol/*.cpp)    # protocol 資料夾
file(GLOB_RECURSE QUERY_SRC      src/query/*.cpp)       # 查詢模組
file(GLOB_RECURSE STATE_SRC      src/state/*.cpp)       # 狀態追蹤模組
file(GLOB_RECURSE COMMON_SRC     src/common/*.cpp)      # BCD/FixedArray 工具類別

# ✅ 主程式進入點
set(MAIN_SRC main.cpp)

# ✅ 建立執行檔
add_executable(TAIFEXMain
    ${MAIN_SRC}
    ${PARSER_SRC}
    ${CONTROLLER_SRC}
    ${DISPATCHER_SRC}
    ${STORAGE_SRC}
    ${DATA_SRC}
    ${OUTPUT_SRC}
    ${BASE_SRC}
    ${PROTOCOL_SRC}
    ${QUERY_SRC}
    ${STATE_SRC}
    ${COMMON_SRC}
)

# ✅ 額外選項（如有需要可加入）
# target_compile_definitions(TAIFEXMain PRIVATE ...)
# target_link_libraries(TAIFEXMain ...)
