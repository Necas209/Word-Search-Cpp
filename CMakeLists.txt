cmake_minimum_required(VERSION 3.30)

project(word-search LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Developer toggles
option(ENABLE_WERROR "Treat warnings as errors" OFF)
option(ENABLE_SANITIZERS "Enable sanitizers in Debug builds" ON)
option(ENABLE_LTO "Enable Link-Time Optimization (IPO) in Release builds" OFF)

set(PROJECT_SOURCE_DIR src)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

include(FetchContent)
FetchContent_Declare(
        ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

add_executable(word-search
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_INCLUDE_DIR}/rnd.hpp
        ${PROJECT_INCLUDE_DIR}/ui.hpp
        ${PROJECT_INCLUDE_DIR}/point.hpp
        ${PROJECT_INCLUDE_DIR}/word.hpp
        ${PROJECT_INCLUDE_DIR}/player.hpp
        ${PROJECT_INCLUDE_DIR}/board.hpp
        ${PROJECT_INCLUDE_DIR}/game.hpp
)

# Warnings, sanitizers, and hardening
if (MSVC)
    # Warnings
    target_compile_options(word-search PRIVATE
            /W4
            $<$<BOOL:${ENABLE_WERROR}>:/WX>
            /permissive-
            /Zc:__cplusplus
            /diagnostics:column
            /external:W0
    )
    # Disable CRT warnings noise already via define
    target_compile_definitions(word-search PRIVATE _CRT_SECURE_NO_WARNINGS)
else ()
    # Common warnings for GCC/Clang
    target_compile_options(word-search PRIVATE
            -Wall -Wextra -Wpedantic
            -Wshadow -Wformat=2
            -Wconversion -Wdouble-promotion -Wno-sign-conversion
            -Wswitch-enum -Wold-style-cast -Woverloaded-virtual
            -Wnon-virtual-dtor -Wnull-dereference
            $<$<BOOL:${ENABLE_WERROR}>:-Werror>
    )

    # Extra per-compiler tuning
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(word-search PRIVATE
                -Wimplicit-fallthrough
                -Wdocumentation -Wextra-semi
        )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(word-search PRIVATE
                -Wimplicit-fallthrough=5
        )
    endif()

    # Debug sanitizers (opt-in via ENABLE_SANITIZERS)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND ENABLE_SANITIZERS)
        # UBSan with trap to avoid runtime dependency; add ASan optionally by uncommenting next line
        target_compile_options(word-search PRIVATE
                -fsanitize=undefined
                -fno-sanitize-recover=all
                -fsanitize-trap
                #-fsanitize=address
                -fno-omit-frame-pointer
        )
        target_link_options(word-search PRIVATE
                -fsanitize=undefined
                -fno-sanitize-recover=all
                -fsanitize-trap
                #-fsanitize=address
        )
    endif()

    # Hardening for non-Debug builds
    if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(word-search PRIVATE
                -fstack-protector-strong
                -D_FORTIFY_SOURCE=3
                -fno-omit-frame-pointer
        )
        # GCC supports stack-clash-protection
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(word-search PRIVATE -fstack-clash-protection)
        endif()
    endif()
endif ()

# Optional LTO/IPO in Release
if (ENABLE_LTO AND CMAKE_BUILD_TYPE STREQUAL "Release")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_msg)
    if (ipo_supported)
        set_property(TARGET word-search PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "IPO/LTO not supported: ${ipo_msg}")
    endif()
endif()

target_sources(word-search PRIVATE
        ${PROJECT_SOURCE_DIR}/ui.cpp
        ${PROJECT_SOURCE_DIR}/word.cpp
        ${PROJECT_SOURCE_DIR}/board.cpp
        ${PROJECT_SOURCE_DIR}/player.cpp
        ${PROJECT_SOURCE_DIR}/game.cpp
)
target_include_directories(word-search PRIVATE ${PROJECT_INCLUDE_DIR})

target_link_libraries(word-search PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(word-search PRIVATE ftxui::component ftxui::dom ftxui::screen)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})