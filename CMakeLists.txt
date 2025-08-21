cmake_minimum_required(VERSION 3.28)

project(word-search)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(word-search
        src/lab.cpp
        src/board.cpp
        src/game.cpp
        src/main.cpp
        src/player.cpp
        src/word.cpp
)

if (MSVC)
    target_compile_options(word-search PRIVATE /W4 /WX)
else ()
    target_compile_options(word-search PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()

target_include_directories(word-search SYSTEM PRIVATE ${nlohmann_json_SOURCE_DIR}/include)
target_include_directories(word-search PRIVATE include)
target_link_libraries(word-search PRIVATE nlohmann_json::nlohmann_json)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})