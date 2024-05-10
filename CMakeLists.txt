cmake_minimum_required(VERSION 3.28)

project(word-search)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(word-search
        src/lib/colors.h
        src/lib/lab.h
        src/lib/lab.cpp
        src/lib/json.hpp
        src/point.h
        src/word.h
        src/board.h
        src/board.cpp
        src/player.h
        src/word_search.h
        src/word_search.cpp
        src/main.cpp
)