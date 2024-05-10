#include "word_search.h"

#include <print>
#include <ranges>
#include <thread>

#include "lib/lab.h"

namespace rv = std::ranges::views;

void word_search::play() {
    start_ = chr::high_resolution_clock::now();

    do {
        board_.show_letters();
        board_.show_words();
        round();
        console::msleep(1000);
        console::clear();
    } while (board_.words_found() < board_.words_count());

    const auto end = chr::high_resolution_clock::now();
    const auto elapsed = end - start_ + elapsed_;

    console::clear();
    board_.show_letters();
    board_.show_words();

    const auto seconds = chr::duration_cast<chr::seconds>(elapsed);
    console::gotoxy(0, board_.height() + 5);

    std::println("{}'s final score: {}.", player_.name(), player_.final_score(seconds, board_.words_count()));
    std::println("\nYou took {} to finish the game.", seconds);
    console::pause();
}

void word_search::exit_game() {
    std::println("Exiting the game...");
    const auto end = chr::high_resolution_clock::now();
    elapsed_ += end - start_;
    save_game();
    exit(0);
}

void word_search::save_game(const std::string &filename) const {
    std::println("Saving the game...");
    std::ofstream ofs(filename);
    if (!ofs) {
        std::println("Error: The file could not be opened.");
        exit(1);
    }
    const json j = *this;
    ofs << std::setw(2) << j;
    std::println("The game was saved successfully.");
}

void word_search::find_word() {
    std::print("\nWhat is your word? ");
    std::string str;
    std::getline(std::cin, str);

    size_t x, y;
    std::println("\nWhere is it?");
    std::print("x: ");
    std::cin >> x;
    std::cin.ignore();
    std::print("y: ");
    std::cin >> y;
    std::cin.ignore();

    str = string::upper(string::strip(str));
    const word word{str, {x, y}, orientation::none};
    const bool word_found = board_.find_word(word);

    int points;
    if (word_found) {
        std::println("\nYou found the word!");
        points = 10;
    } else {
        std::println("\nYou didn't find the word.");
        points = -5;
    }
    player_.update_score(points);
}

void word_search::round() {
    console::gotoxy(1, board_.height() + 5);

    std::println("Game Menu:");
    std::println("\t1 -> Find a word");
    std::println("\t2 -> Save the game");
    std::println("\t3 -> Exit");
    std::print("Option: ");
    int option;
    std::cin >> option;
    std::cin.ignore();

    switch (option) {
        case 1:
            find_word();
            break;
        case 2:
            save_game();
            break;
        case 3:
            exit_game();
        default:
            std::cout << "Invalid option.\n";
            break;
    }
}

auto word_search::load_game(const std::string &filename) -> word_search {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::println("Error: The file could not be opened.");
        exit(1);
    }
    json j;
    ifs >> j;
    return j.get<word_search>();
}

auto load_words() -> std::vector<std::string> {
    std::println("Pick the theme of the word search:");
    std::println("\t1 -> Portugal");
    std::println("\t2 -> United States of America");
    std::print("Option: ");
    uint16_t option;
    std::cin >> option;
    std::cin.ignore();

    std::string filename;
    switch (option) {
        case 1:
            std::println("Loading the districts of Portugal...");
            filename = "words/portugal.txt";
            break;
        case 2:
            std::println("Loading the United States of America...");
            filename = "words/usa.txt";
            break;
        default:
            std::println("Invalid option.");
            exit(1);
    }
    std::ifstream file(filename);
    if (!file) {
        std::println("Error: The file could not be opened.");
        exit(1);
    }
    std::vector<std::string> words;

    while (!file.eof()) {
        std::string str;
        std::getline(file, str);
        if (str.empty())
            continue;
        words.emplace_back(str);
    }
    return words;
}
