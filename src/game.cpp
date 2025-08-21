#include "game.hpp"

#include <filesystem>
#include <fstream>
#include <thread>

#include "lab.hpp"

using namespace std::chrono_literals;
namespace rv = std::ranges::views;

constexpr game::game(board board, player player) noexcept
    : board_(std::move(board)), player_(std::move(player))
{
}

void from_json(const json& j, game& game)
{
    j.at("elapsed").get_to(game.elapsed_);
    j.at("board").get_to(game.board_);
    j.at("player").get_to(game.player_);
}

void to_json(json& j, const game& game)
{
    j = json{
        {"elapsed", game.elapsed_},
        {"board", game.board_},
        {"player", game.player_},
    };
}

void game::play()
{
    start_ = std::chrono::high_resolution_clock::now();
    while (!board_.solved())
    {
        board_.show_letters();
        board_.show_words();

        console::gotoxy(1, board_.height() + 5);

        std::println("Game Menu:");
        std::println("\t1 -> Find a word");
        std::println("\t2 -> Save the game");
        std::println("\t3 -> Exit");
        std::print("Option: ");
        int option;
        std::cin >> option;
        std::cin.ignore();

        switch (option)
        {
        case 1:
            find_word();
            break;
        case 2:
            save_game();
            break;
        case 3:
            std::println("Exiting the game...");
            pause_game();
            return;
        default:
            std::cout << "Invalid option.\n";
            break;
        }

        std::this_thread::sleep_for(1s);
        console::clear();
    }

    const auto end = std::chrono::high_resolution_clock::now();
    elapsed_ += end - start_;

    console::clear();
    board_.show_letters();
    board_.show_words();

    console::gotoxy(0, board_.height() + 5);

    const auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_);
    std::println("{}'s final score: {}.", player_.name(), player_.final_score(elapsed_seconds, board_.words_found()));
    std::println("\nYou took {} to finish the game.", elapsed_seconds);
    console::pause();
}

void game::pause_game()
{
    const auto end = std::chrono::high_resolution_clock::now();
    elapsed_ += end - start_;
    save_game();
}

void game::save_game(const std::filesystem::path& filename) const
{
    std::println("Saving the game...");
    std::ofstream file{filename};
    if (!file)
    {
        throw std::filesystem::filesystem_error{
            "Could not write file.",
            filename,
            std::make_error_code(std::errc::io_error)
        };
    }

    const json j = *this;
    file << std::setw(2) << j;
    std::println("The game was saved successfully.");
}

void game::find_word()
{
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

    switch (board_.find_word(word{word::cleanup(str), orientation::none, point{x, y}}))
    {
    case word_state::not_found:
        std::println("\nYou didn't find the word.");
        player_.update_score(-5);
        break;
    case word_state::found:
        std::println("\nYou found the word!");
        player_.update_score(10);
        break;
    case word_state::already_found:
        std::println("The word has already been found.");
        break;
    }
}

void game::new_game()
{
    std::println("Starting a new game...");

    auto player = player::new_player();
    std::cout << std::endl;

    auto words = load_words();
    console::pause();
    console::clear();

    auto board = board::new_board();
    board.fill_letters(words, 10);
    console::clear();

    game game{std::move(board), std::move(player)};
    game.play();
}

void game::load_game(const std::filesystem::path& filename)
{
    std::println("Loading a game...");
    if (!std::filesystem::exists(filename))
    {
        std::println("There is no save.");
        return;
    }
    std::ifstream file{filename};
    if (!file)
    {
        throw std::filesystem::filesystem_error{
            "Could not read file.",
            filename,
            std::make_error_code(std::errc::io_error)
        };
    }

    json j;
    file >> j;
    auto game = j.get<::game>();
    game.play();
}

auto load_words() -> std::vector<std::string>
{
    std::println("Pick the theme of the word search:");
    std::println("\t1 -> Portugal");
    std::println("\t2 -> United States of America");
    uint16_t option;
    while (true)
    {
        std::print("Option: ");
        if (!(std::cin >> option))
        {
            std::cin.clear();
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (option == 1 || option == 2)
            break;

        std::println("Invalid option. Please enter 1 or 2.");
    }


    std::filesystem::path filename;
    switch (option)
    {
    case 1:
        std::println("Loading the districts of Portugal...");
        filename = "data/portugal.txt";
        break;
    case 2:
        std::println("Loading the United States of America...");
        filename = "data/usa.txt";
        break;
    default:
        std::println("Invalid option.");
        exit(1);
    }
    std::ifstream file(filename);
    if (!file)
    {
        throw std::filesystem::filesystem_error{
            "Could not read file.",
            filename,
            std::make_error_code(std::errc::io_error)
        };
    }

    std::vector<std::string> words;
    std::string line;
    while (std::getline(file, line))
    {
        words.push_back(line);
    }
    return words;
}
