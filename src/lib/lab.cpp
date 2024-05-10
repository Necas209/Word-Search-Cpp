#include "lab.h"

#include <chrono>
#include <iostream>
#include <print>
#include <thread>

void console::clear() {
    if (system("clear") == 0)
        return;

    throw std::runtime_error("Failed to clear the screen");
}

void console::pause() {
    std::cout << "Press any key to continue...";
    std::cin.get();
}

void console::set_color(const ansi_color foreground, const ansi_bg_color background) {
    std::print("\033[{};{}m", foreground, background);
}

void console::set_foreground_color(const ansi_color foreground) {
    std::print("\033[{}m", foreground);
}

void console::reset_color() {
    std::cout << "\033[0m";
}

void console::gotoxy(const uint32_t x, const uint32_t y) {
    std::print("\033[{};{}H", y, x);
}

void console::show_rectangle_at(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) {
    // Move the cursor to the starting position
    gotoxy(x, y);

    // Print the top border
    std::cout << "╔"; // top left corner
    for (int i = 0; i < width - 2; i++) {
        std::cout << "═"; // horizontal side
    }
    std::cout << "╗\n"; // top right corner

    // Print the middle part
    for (int i = 0; i < height - 2; i++) {
        gotoxy(x, y + i + 1);
        std::cout << "║"; // vertical side
        for (int j = 0; j < width - 2; j++) {
            std::cout << " "; // space
        }
        std::cout << "║\n"; // vertical side
    }

    // Print the bottom border
    gotoxy(x, y + height - 1);
    std::cout << "╚"; // bottom left corner
    for (int i = 0; i < width - 2; i++) {
        std::cout << "═"; // horizontal side
    }
    std::cout << "╝\n"; // bottom right corner
}

void console::msleep(const uint32_t milliseconds) {
    const auto ms = std::chrono::milliseconds(milliseconds);
    std::this_thread::sleep_for(ms);
}
