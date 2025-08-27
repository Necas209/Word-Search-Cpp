#include "board.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

#include "console.hpp"
#include "rnd.hpp"

namespace rv = std::ranges::views;

auto word_search::get_orientations(const difficulty difficulty) -> std::span<const orientation>
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
    }
    throw std::invalid_argument("Invalid difficulty");
}

auto word_search::board::solved() const -> bool
{
    return std::ranges::all_of(words_, [](const word& word) { return word.found(); });
}

auto word_search::board::words_found() const -> std::int64_t
{
    return std::ranges::count_if(words_, [](const word& word) { return word.found(); });
}

auto word_search::board::find_word(const word& search_word) -> word_state
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

auto word_search::board::fill_letters(std::vector<std::string>& words, const std::size_t max_num_words) -> void
{
    const auto orientations = get_orientations(difficulty_);
    std::size_t k = 0;

    for (auto str : words | rv::transform(word::cleanup))
    {
        if (k == max_num_words)
            break;

        if (const auto l = str.length(); l > width_ || l > height_)
            continue;

        auto i = 0;
        while (i < 20)
        {
            i++;
            const auto orientation = orientations[rnd::next(orientations.size())];

            const auto x = random_x(str, orientation);
            if (!x.has_value())
                continue;

            const auto y = random_y(str, orientation);
            if (!y.has_value())
                continue;

            if (word word{str, point_t{x.value(), y.value()}, orientation}; word_fits(word))
            {
                words_.emplace_back(word);
                insert_word(word);
                k++;
                break;
            }
        }
    }

    for (const auto isspace = [](const char ch) { return std::isspace(ch, std::locale::classic()); };
         auto& letter : letters_ | rv::filter(isspace))
    {
        letter = rnd::next<std::int8_t>('A', 'Z');
    }
}

auto word_search::board::show_words() const -> void
{
    const auto num_found = words_found();
    console::gotoxy(2 * width_ + 20, 1);
    std::print("Words found: {} of {}", num_found, words_.size());
    if (difficulty_ == difficulty::easy)
    {
        console::gotoxy(2 * width_ + 50, 1);
        std::print("Words to find:");
    }
    for (std::size_t i = 0; i < words_.size(); i++)
    {
        if (const auto& word = words_[i]; word.found())
        {
            console::set_foreground_color(console::ansi_color::green);
            console::gotoxy(2 * width_ + 20, 3 + 2 * i);
        }
        else if (difficulty_ == difficulty::easy)
        {
            console::set_foreground_color(console::ansi_color::red);
            console::gotoxy(2 * width_ + 50, 3 + 2 * i);
            std::print("{}", word.str());
        }
        console::reset_color();
    }
}

auto word_search::board::show_letters() const -> void
{
    constexpr auto x_start = 8;
    constexpr auto y_start = 3;

    console::gotoxy(x_start - 1, y_start - 1);
    for (const auto i : rv::iota(0ul, width_))
    {
        std::print("{} ", i % 10);
    }
    std::println();
    for (const auto i : rv::iota(0ul, height_))
    {
        std::print("    {} ", i % 10);
        for (const auto j : rv::iota(0ul, width_))
        {
            if (const point_t point{j, i};
                std::ranges::any_of(words_, [&point](const word& word) { return word.is_found_at(point); }))
            {
                console::set_foreground_color(console::ansi_color::yellow);
            }
            std::print("{} ", (*this)[j, i]);
            console::reset_color();
        }
        std::println();
        console::reset_color();
    }
}

auto word_search::board::random_x(const std::string& word,
                                  const orientation orientation) const -> std::optional<std::size_t>
{
    const auto len = word.length();
    if (len > width_)
        return std::nullopt;

    std::size_t x;
    switch (orientation)
    {
    case orientation::down:
    case orientation::up:
        x = rnd::next(width_);
        break;
    case orientation::front:
    case orientation::front_down:
    case orientation::front_up:
        x = rnd::next(width_ - len + 1);
        break;
    case orientation::back:
    case orientation::back_down:
    case orientation::back_up:
        x = len - 1 + rnd::next(width_ - len + 1);
        break;
    default:
        throw std::invalid_argument("Invalid orientation");
    }
    return x;
}

auto word_search::board::random_y(const std::string& word,
                                  const orientation orientation) const -> std::optional<std::size_t>
{
    const auto len = word.length();
    if (len > width_ or len > height_)
    {
        return std::nullopt;
    }

    std::size_t y;
    switch (orientation)
    {
    case orientation::front:
    case orientation::back:
        y = rnd::next(height_);
        break;
    case orientation::down:
    case orientation::front_down:
    case orientation::back_down:
        y = rnd::next(height_ - len + 1);
        break;
    case orientation::up:
    case orientation::front_up:
    case orientation::back_up:
        y = len - 1 + rnd::next(height_ - len + 1); // Adjusted the range
        break;
    default:
        throw std::invalid_argument("Invalid orientation");
    }
    return y;
}

auto word_search::board::word_fits(const word& word) const -> bool
{
    auto check_letter = [this](const char ch, const std::size_t x, const std::size_t y)
    {
        return (*this)[x, y] == ' ' or (*this)[x, y] == ch;
    };

    const auto [x, y] = word.point();
    for (std::size_t i = 0; i < word.length(); i++)
    {
        const auto ch = word[i];
        switch (word.orientation())
        {
        case orientation::front:
            if (!check_letter(ch, x + i, y))
                return false;
            break;
        case orientation::back:
            if (!check_letter(ch, x - i, y))
                return false;
            break;
        case orientation::down:
            if (!check_letter(ch, x, y + i))
                return false;
            break;
        case orientation::up:
            if (!check_letter(ch, x, y - i))
                return false;
            break;
        case orientation::front_down:
            if (!check_letter(ch, x + i, y + i))
                return false;
            break;
        case orientation::back_up:
            if (!check_letter(ch, x - i, y - i))
                return false;
            break;
        case orientation::back_down:
            if (!check_letter(ch, x - i, y + i))
                return false;
            break;
        case orientation::front_up:
            if (!check_letter(ch, x + i, y - i))
                return false;
            break;
        default:
            throw std::invalid_argument("Invalid orientation");
        }
    }
    return true;
}

auto word_search::board::insert_word(const word& word) -> void
{
    const auto [x, y] = word.point();
    const auto [dx, dy] = orientation_offsets()[std::to_underlying(word.orientation())];

    auto px = x;
    auto py = y;
    for (const auto ch : word)
    {
        (*this)[px, py] = ch;
        px += dx;
        py += dy;
    }
}

auto word_search::check_dimension(const dimension_t dimension, const difficulty difficulty) -> bool
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
    throw std::invalid_argument("Invalid difficulty");
}

auto word_search::new_board() -> board
{
    std::println("Now, let's create the board.");
    std::println("What is the difficulty?");
    std::println("\t1 - Easy");
    std::println("\t2 - Medium");
    std::println("\t3 - Hard");
    std::print("Option: ");
    std::underlying_type_t<difficulty> diff;
    std::cin >> diff;
    std::cin.ignore();
    const auto difficulty = static_cast<word_search::difficulty>(diff);

    std::println(
        "\nThe board size should be at least 10 x 10 for easy mode,"
        " 15 x 15 for medium mode,"
        " and 20 x 20 for hard mode."
    );

    std::print("\nWhat is the width of the board? ");
    dimension_t width;
    std::cin >> width;
    std::cin.ignore();
    if (!check_dimension(width, difficulty))
        throw std::invalid_argument("Invalid width");

    std::print("\nWhat is the height of the board? ");
    dimension_t height;
    std::cin >> height;
    std::cin.ignore();
    if (!check_dimension(height, difficulty))
        throw std::invalid_argument("Invalid height");

    return board{width, height, difficulty};
}
