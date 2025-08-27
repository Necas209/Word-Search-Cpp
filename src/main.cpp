#include <chrono>
#include <iostream>
#include <print>
#include <thread>

#include "console.hpp"
#include "game.hpp"

int main()
{
    std::println("Welcome to the Word Search Game!");
    std::println("Author: Diogo Medeiros, no. 70633");
    std::println("Press any key to start the game.");
    std::cin.get();
    console::clear();

    while (true)
    {
        std::println("Main Menu:");
        std::println("  1 -> Start a new game");
        std::println("  2 -> Load a game");
        std::println("  3 -> Exit");
        std::print("Option: ");
        std::uint8_t option;
        std::cin >> option;
        std::cin.ignore();
        console::clear();

        switch (option)
        {
        case '1':
            word_search::new_game();
            break;
        case '2':
            word_search::load_game();
            break;
        case '3':
            using namespace std::chrono_literals;
            std::println("Exiting the game...");
            std::this_thread::sleep_for(1s);
            return 0;
        default:
            std::println("Invalid option.");
            break;
        }
    }
}
