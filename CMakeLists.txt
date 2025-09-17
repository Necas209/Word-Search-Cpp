cmake_minimum_required(VERSION 3.30)

project(word-search LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

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

if (MSVC)
    target_compile_options(word-search PRIVATE /W4 /WX /D_CRT_SECURE_NO_WARNINGS)
else ()
    target_compile_options(word-search PRIVATE -Wall -Wextra -Wpedantic -Werror -Wconversion -Wdouble-promotion
            -Wunused-parameter -Wunused-function -Wno-sign-conversion -Wswitch-enum
            -fsanitize=undefined -fsanitize-trap)
endif ()

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