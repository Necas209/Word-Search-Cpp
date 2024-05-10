#include "board.h"

#include <print>
#include <ranges>

#include "lib/lab.h"

namespace rv = std::ranges::views;

auto get_orientations(const difficulty difficulty) -> std::vector<orientation> {
    switch (difficulty) {
        case difficulty::easy:
            return {orientation::front, orientation::down};
        case difficulty::medium:
            return {
                orientation::front, orientation::down,
                orientation::front_down, orientation::front_up
            };
        case difficulty::hard:
            return {
                orientation::front, orientation::down,
                orientation::front_down, orientation::front_up,
                orientation::back, orientation::up,
                orientation::back_down, orientation::back_up
            };
        default:
            throw std::invalid_argument("Invalid difficulty");
    }
}

auto board::new_board() -> board {
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
    if (!check_dimension(width, difficulty)) {
        throw std::invalid_argument("Invalid width");
    }

    std::print("\nWhat is the height of the board? ");
    std::cin >> height;
    std::cin.ignore();
    if (!check_dimension(height, difficulty)) {
        throw std::invalid_argument("Invalid height");
    }

    return {width, height, difficulty};
}

std::istream &operator>>(std::istream &in, difficulty &difficulty) {
    difficulty_t value;
    in >> value;
    difficulty = static_cast<enum difficulty>(value);
    return in;
}

void board::fill_letters(std::vector<std::string> &words, const size_t max_num_words) {
    const auto orientations = get_orientations(difficulty_);
    int k = 0;

    for (const auto &str: words
                          | rv::transform(string::strip)
                          | rv::transform(string::upper)) {
        if (k == max_num_words)
            break;

        if (const size_t l = str.length(); l > width_ || l > height_) {
            continue;
        }

        auto i = 0;
        while (i < 20) {
            i++;
            const auto orientation = orientations[random(orientations.size())];

            const auto x = random_x(str, orientation);
            if (!x.has_value()) {
                continue;
            }

            const auto y = random_y(str, orientation);
            if (!y.has_value()) {
                continue;
            }

            if (const word word{str, {x.value(), y.value()}, orientation}; word_fits(word)) {
                insert_word(word);
                k++;
                break;
            }
        }
    }
    for (auto &row: letters_) {
        for (char &letter: row) {
            if (letter != ' ')
                continue;
            letter = random_inclusive('A', 'Z');
        }
    }
}

void board::show_words() const {
    const auto num_found = words_found();
    console::gotoxy(2 * width_ + 20, 1);
    std::print("Words found: {} of {}", num_found, words_.size());
    if (difficulty_ == difficulty::easy) {
        console::gotoxy(2 * width_ + 50, 1);
        std::print("Words to find:");
    }
    for (auto [i, word]: enumerate(words_)) {
        if (word.found()) {
            console::set_foreground_color(ansi_color::green);
            console::gotoxy(2 * width_ + 20, 3 + 2 * i);
            std::cout << word.str();
        } else if (difficulty_ == difficulty::easy) {
            console::set_foreground_color(ansi_color::red);
            console::gotoxy(2 * width_ + 50, 3 + 2 * i);
            std::cout << word.str();
        }
        console::reset_color();
    }
}

void board::show_letters() const {
    constexpr auto xstart = 8;
    constexpr auto ystart = 3;

    console::gotoxy(xstart - 1, ystart - 1);
    for (const auto i: rv::iota(0ul, width_)) {
        std::print("{} ", i % 10);
    }
    std::cout << std::endl;
    for (const auto i: rv::iota(0ul, height_)) {
        std::print("    {} ", i % 10);
        for (const auto j: rv::iota(0ul, width_)) {
            if (std::ranges::any_of(words_, [j, i](const word &word) { return word.is_found_at(j, i); })) {
                console::set_foreground_color(ansi_color::yellow);
            }
            std::print("{} ", letters_[i][j]);
            console::reset_color();
        }
        std::cout << std::endl;
        console::reset_color();
    }
}

bool board::word_fits(const word &word) const {
    auto check_letter = [&](const char ch, const point_t x, const point_t y) {
        return letters_[y][x] == ' ' or letters_[y][x] == ch;
    };

    const auto [x, y] = word.point();
    for (const auto [i, ch]: enumerate(word)) {
        switch (word.orientation()) {
            case orientation::front:
                if (!check_letter(ch, x + i, y)) {
                    return false;
                }
                break;
            case orientation::back:
                if (!check_letter(ch, x - i, y)) {
                    return false;
                }
                break;
            case orientation::down:
                if (!check_letter(ch, x, y + i)) {
                    return false;
                }
                break;
            case orientation::up:
                if (!check_letter(ch, x, y - i)) {
                    return false;
                }
                break;
            case orientation::front_down:
                if (!check_letter(ch, x + i, y + i)) {
                    return false;
                }
                break;
            case orientation::back_up:
                if (!check_letter(ch, x - i, y - i)) {
                    return false;
                }
                break;
            case orientation::back_down:
                if (!check_letter(ch, x - i, y + i)) {
                    return false;
                }
                break;
            case orientation::front_up:
                if (!check_letter(ch, x + i, y - i)) {
                    return false;
                }
                break;
            default:
                throw std::invalid_argument("Invalid orientation");
        }
    }
    return true;
}

void board::insert_word(const word &word) {
    const auto [x, y] = word.point();
    words_.push_back(word);

    for (const auto [i, ch]: enumerate(word)) {
        switch (word.orientation()) {
            case orientation::front:
                letters_[y][x + i] = ch;
                break;
            case orientation::back:
                letters_[y][x - i] = ch;
                break;
            case orientation::down:
                letters_[y + i][x] = ch;
                break;
            case orientation::up:
                letters_[y - i][x] = ch;
                break;
            case orientation::front_down:
                letters_[y + i][x + i] = ch;
                break;
            case orientation::back_up:
                letters_[y - i][x - i] = ch;
                break;
            case orientation::back_down:
                letters_[y + i][x - i] = ch;
                break;
            case orientation::front_up:
                letters_[y - i][x + i] = ch;
                break;
            default:
                break;
        }
    }
}

bool board::find_word(const word &search_word) {
    const auto word_iter = std::ranges::find(words_, search_word);
    if (word_iter == words_.end()) {
        std::println("The word is not present in the word search.");
        return false;
    }
    if (word_iter->found()) {
        std::println("The word has already been found.");
        return false;
    }
    word_iter->mark_as_found();
    return true;
}

std::optional<point_t> board::random_x(const std::string &word, const orientation orientation) const {
    const size_t len = word.length();
    if (len > width_) {
        return std::nullopt;
    }

    point_t x;
    switch (orientation) {
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

std::optional<point_t>
board::random_y(const std::string &word, const orientation orientation) const {
    const size_t len = word.length();
    if (len > width_ or len > height_) {
        return std::nullopt;
    }

    point_t y;
    switch (orientation) {
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

auto board::check_dimension(const dimension_t dimension, const difficulty difficulty) -> bool {
    switch (difficulty) {
        case difficulty::easy:
            return dimension >= 10;
        case difficulty::medium:
            return dimension >= 15;
        case difficulty::hard:
            return dimension >= 20;
    }
    return false;
}
