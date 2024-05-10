#include <iostream>
#include <print>

#include "lib/lab.h"
#include "board.h"
#include "player.h"
#include "word_search.h"

int main() {
    std::println("Welcome to the Word Search Game!");
    std::println("Author: Diogo Medeiros, no. 70633");
    std::println("Press any key to start the game.");
    std::cin.get();
    console::clear();

    std::println("Main Menu:");
    std::println("\t1 -> Start a new game");
    std::println("\t2 -> Load a game");
    std::println("\t3 -> Exit");
    std::print("Option: ");
    uint16_t option;
    std::cin >> option;
    std::cin.ignore();
    console::clear();

    switch (option) {
        case 1: {
            auto player = player::new_player();
            std::cout << std::endl;

            auto words = load_words();
            console::pause();
            console::clear();

            auto board = board::new_board();
            board.fill_letters(words, 10);
            console::clear();

            auto game = word_search{board, player};
            game.play();
            break;
        }
        case 2: {
            auto game = word_search::load_game();
            game.play();
            break;
        }
        case 3:
            exit(0);
        default: {
            std::println("Invalid option.");
            break;
        }
    }

    return 0;
}
