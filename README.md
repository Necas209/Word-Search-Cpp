# Word Search (C++)

A terminal-based word search game written in modern C++ (C++23) using the
[FTXUI](https://github.com/ArthurSonzogni/FTXUI) TUI library.

Find hidden words on a board, track your score and time, and save your progress to continue later.

[![CMake on multiple platforms](https://github.com/Necas209/Word-Search-Cpp/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=main&event=push)](https://github.com/Necas209/Word-Search-Cpp/actions/workflows/cmake-multi-platform.yml)

## Features

- Multiple difficulty levels and themes (picked when starting a new game)
- Live progress panel (words found, total time, score)
- Keyboard and mouse support
- Save & Continue: quit during a game and resume later
- JSON file format for saving and loading progress (via [nlohmann/json](https://github.com/nlohmann/json))

## Prerequisites

- A C++23 compatible compiler
    - Linux/macOS: GCC 14+ or Clang 16+
    - Windows: MSVC 19.36+ (VS 2022)
- CMake 3.30 or newer
- Git

CMake pulls FTXUI automatically if the project uses FetchContent; otherwise install it via your package manager
or fetch it as a submodule.

## Build and run

First, make sure you have all the prerequisites installed.

Then, clone the repository and run the following commands:

### Linux / macOS

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

cmake --build build -j

cd build && ./word-search
```

### Windows

```shell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64

cmake --build build --config Release

.\build\Release\word-search.exe
```

## How to Play

- New Game: create a player and board, then pick a theme.
- Enter guesses:
    - Type the word and its starting coordinates (X, Y).
    - Choose the orientation from the menu.
    - Press Enter to submit.
- Scoring:
    - Correct word: +10
    - Incorrect word: −5
- Save & Quit:
    - Use the “Save & Quit” button to exit to the main menu; progress is saved to `data/save.json`.
    - From the main menu, choose “Continue Game” to resume.

Tips:

- Tab / Shift+Tab: move focus between inputs and buttons.
- Arrow keys: navigate menus.
- Enter/Space: activate focused item.
- Mouse: click inputs, menus, and buttons (if your terminal supports it).

## Troubleshooting

- If you see a blank screen or no input focus, try pressing Tab to cycle focus.
- If the binary doesn’t run, ensure your terminal supports the required features (truecolor, mouse events)
  and that you’re running from the correct working directory (so the `data/` folder is found).
- Reconfigure with `-DCMAKE_BUILD_TYPE=Debug` if you need debug symbols.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.