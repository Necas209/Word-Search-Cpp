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
        src/lib/lab.cpp
        src/board.cpp
        src/word_search.cpp
        src/main.cpp
        src/player.cpp
)

target_link_libraries(word-search PRIVATE nlohmann_json::nlohmann_json)