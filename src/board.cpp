#include "board.hpp"

#include "lab.hpp"
#include <iostream>
#include <locale>
#include <ranges>
#include <stdexcept>
#include <string>

namespace rv = std::ranges::views;

std::istream& operator>>(std::istream& in, difficulty& difficulty)
{
    std::uint16_t value;
    in >> value;
    difficulty = static_cast<::difficulty>(value);
    return in;
}

constexpr board::board(const dimension_t width, const dimension_t height, const difficulty difficulty) noexcept
    : width_(width), height_(height), difficulty_(difficulty), letters_(width * height, ' ')
{
}

auto board::operator[](const size_t x, const size_t y) const -> char
{
    return letters_[y * width_ + x];
}

auto board::operator[](const size_t x, const size_t y) -> char&
{
    return letters_[y * width_ + x];
}

auto board::solved() const -> bool
{
    return std::ranges::all_of(words_, [](const word& word) { return word.found(); });
}

auto board::words_found() const -> std::size_t
{
    return std::ranges::count_if(words_, [](const word& word) { return word.found(); });
}

auto board::find_word(const word& search_word) -> word_state
{
    const auto it = std::ranges::find_if(words_, [&search_word](const word& w)
    {
        return w.str() == search_word.str()
            && w.point() == search_word.point()
            && w.orientation() == search_word.orientation();
    });

    if (it != words_.end())
        return word_state::not_found;

    if (it->found())
        return word_state::already_found;

    it->mark_as_found();
    return word_state::found;
}

void to_json(nlohmann::json& j, const board& board)
{
    j = nlohmann::json{
        {"width", board.width_},
        {"height", board.height_},
        {"difficulty", board.difficulty_},
        {"letters", board.letters_},
        {"words", board.words_}
    };
}

void from_json(const nlohmann::json& j, board& board)
{
    j.at("width").get_to(board.width_);
    j.at("height").get_to(board.height_);
    j.at("difficulty").get_to(board.difficulty_);
    j.at("letters").get_to(board.letters_);
    j.at("words").get_to(board.words_);
}

constexpr auto get_orientations(const difficulty difficulty) -> std::span<const orientation>
{
    static constexpr std::array easy{
        orientation::front,
        orientation::down
    };
    static constexpr std::array medium{
        orientation::front,
        orientation::down,
        orientation::front_down,
        orientation::front_up
    };
    static constexpr std::array hard{
        orientation::front,
        orientation::down,
        orientation::front_down,
        orientation::front_up,
        orientation::back,
        orientation::up,
        orientation::back_down,
        orientation::back_up
    };

    switch (difficulty)
    {
    case difficulty::easy:
        return easy;
    case difficulty::medium:
        return medium;
    case difficulty::hard:
        return hard;
    default:
        throw std::invalid_argument("Invalid difficulty");
    }
}

auto board::new_board() -> board
{
    std::println("Now, let's create the board.");
    std::println("What is the difficulty?");
    std::println("\t1 - Easy");
    std::println("\t2 - Medium");
    std::println("\t3 - Hard");
    std::print("Option: ");
    difficulty difficulty;
    std::cin >> difficulty;
    std::cin.ignore();

    std::println("\nThe board size should be at least 10 x 10 for easy mode,"
        " 15 x 15 for medium mode, and 20 x 20 for hard mode.");

    dimension_t width, height;
    std::print("\nWhat is the width of the board? ");
    std::cin >> width;
    std::cin.ignore();
    if (!check_dimension(width, difficulty))
    {
        throw std::invalid_argument("Invalid width");
    }

    std::print("\nWhat is the height of the board? ");
    std::cin >> height;
    std::cin.ignore();
    if (!check_dimension(height, difficulty))
    {
        throw std::invalid_argument("Invalid height");
    }

    return board{width, height, difficulty};
}

void board::fill_letters(std::vector<std::string>& words, const size_t max_num_words)
{
    const auto orientations = get_orientations(difficulty_);
    size_t k = 0;

    for (auto str : rv::transform(words, word::cleanup))
    {
        if (k == max_num_words)
            break;

        if (const size_t l = str.length(); l > width_ || l > height_)
        {
            continue;
        }

        auto i = 0;
        while (i < 20)
        {
            i++;
            const auto orientation = orientations[random(orientations.size())];

            const auto x = random_x(str, orientation);
            if (!x.has_value())
                continue;

            const auto y = random_y(str, orientation);
            if (!y.has_value())
                continue;

            if (point_t point{x.value(), y.value()}; word_fits(str, orientation, point))
            {
                const auto& word = words_.emplace_back(str, orientation, point);
                insert_word(word);
                k++;
                break;
            }
        }
    }
    for (auto& letter : letters_ | rv::filter([](const auto ch) { return std::isspace(ch); }))
    {
        letter = random_inclusive('A', 'Z');
    }
}

void board::show_words() const
{
    const auto num_found = words_found();
    console::gotoxy(2 * width_ + 20, 1);
    std::print("Words found: {} of {}", num_found, words_.size());
    if (difficulty_ == difficulty::easy)
    {
        console::gotoxy(2 * width_ + 50, 1);
        std::print("Words to find:");
    }
    for (auto [i, word] : enumerate(words_))
    {
        if (word.found())
        {
            console::set_foreground_color(ansi_color::green);
            console::gotoxy(2 * width_ + 20, 3 + 2 * i);
            std::cout << word.str();
        }
        else if (difficulty_ == difficulty::easy)
        {
            console::set_foreground_color(ansi_color::red);
            console::gotoxy(2 * width_ + 50, 3 + 2 * i);
            std::cout << word.str();
        }
        console::reset_color();
    }
}

void board::show_letters() const
{
    constexpr auto x_start = 8;
    constexpr auto y_start = 3;

    console::gotoxy(x_start - 1, y_start - 1);
    for (const auto i : rv::iota(0ul, width_))
    {
        std::print("{} ", i % 10);
    }
    std::cout << std::endl;
    for (const auto i : rv::iota(0ul, height_))
    {
        std::print("    {} ", i % 10);
        for (const auto j : rv::iota(0ul, width_))
        {
            if (const point_t point{j, i};
                std::ranges::any_of(words_, [&point](const word& word) { return word.is_found_at(point); }))
            {
                console::set_foreground_color(ansi_color::yellow);
            }
            std::print("{} ", (*this)[j, i]);
            console::reset_color();
        }
        std::cout << std::endl;
        console::reset_color();
    }
}

auto board::word_fits(const std::string& word, const orientation orientation, const point_t& point) const -> bool
{
    auto check_letter = [this](const char ch, const size_t x, const size_t y)
    {
        return (*this)[x, y] == ' ' or (*this)[x, y] == ch;
    };

    const auto [x, y] = point;
    for (const auto [i, ch] : enumerate(word))
    {
        switch (orientation)
        {
        case orientation::front:
            if (!check_letter(ch, x + i, y))
            {
                return false;
            }
            break;
        case orientation::back:
            if (!check_letter(ch, x - i, y))
            {
                return false;
            }
            break;
        case orientation::down:
            if (!check_letter(ch, x, y + i))
            {
                return false;
            }
            break;
        case orientation::up:
            if (!check_letter(ch, x, y - i))
            {
                return false;
            }
            break;
        case orientation::front_down:
            if (!check_letter(ch, x + i, y + i))
            {
                return false;
            }
            break;
        case orientation::back_up:
            if (!check_letter(ch, x - i, y - i))
            {
                return false;
            }
            break;
        case orientation::back_down:
            if (!check_letter(ch, x - i, y + i))
            {
                return false;
            }
            break;
        case orientation::front_up:
            if (!check_letter(ch, x + i, y - i))
            {
                return false;
            }
            break;
        default:
            throw std::invalid_argument("Invalid orientation");
        }
    }
    return true;
}

void board::insert_word(const word& word)
{
    const auto [x, y] = word.point();
    const auto [dx, dy] = orientation_offsets().at(word.orientation());

    auto px = x;
    auto py = y;
    for (const auto ch : word)
    {
        (*this)[px, py] = ch;
        px += dx;
        py += dy;
    }
}

auto board::random_x(const std::string& word, const orientation orientation) const -> std::optional<std::size_t>
{
    const size_t len = word.length();
    if (len > width_)
    {
        return std::nullopt;
    }

    std::size_t x;
    switch (orientation)
    {
    case orientation::down:
    case orientation::up:
        x = random(width_);
        break;
    case orientation::front:
    case orientation::front_down:
    case orientation::front_up:
        x = random(width_ - len + 1);
        break;
    case orientation::back:
    case orientation::back_down:
    case orientation::back_up:
        x = len - 1 + random(width_ - len + 1);
        break;
    default:
        throw std::invalid_argument("Invalid orientation");
    }
    return x;
}

auto board::random_y(const std::string& word, const orientation orientation) const -> std::optional<std::size_t>
{
    const size_t len = word.length();
    if (len > width_ or len > height_)
    {
        return std::nullopt;
    }

    std::size_t y;
    switch (orientation)
    {
    case orientation::front:
    case orientation::back:
        y = random(height_);
        break;
    case orientation::down:
    case orientation::front_down:
    case orientation::back_down:
        y = random(height_ - len + 1);
        break;
    case orientation::up:
    case orientation::front_up:
    case orientation::back_up:
        y = len - 1 + random(height_ - len + 1); // Adjusted the range
        break;
    default:
        throw std::invalid_argument("Invalid orientation");
    }
    return y;
}

auto board::check_dimension(const dimension_t dimension, const difficulty difficulty) -> bool
{
    switch (difficulty)
    {
    case difficulty::easy:
        return dimension >= 10;
    case difficulty::medium:
        return dimension >= 15;
    case difficulty::hard:
        return dimension >= 20;
    }
    return false;
}
