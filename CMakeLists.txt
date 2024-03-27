cmake_minimum_required(VERSION 3.28)

project(word-search)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(word-search
        src/Point.h
        src/Letter.h
        src/Letter.cpp
        src/Word.h
        src/Word.cpp
        src/Board.h
        src/Board.cpp
        src/Easy_Board.h
        src/Easy_Board.cpp
        src/Hard_Board.h
        src/Hard_Board.cpp
        src/Player.h
        src/Beginner.h
        src/Expert.h
        src/Game.h
        src/Game.cpp
        src/functions.h
        src/functions.cpp
        main.cpp)