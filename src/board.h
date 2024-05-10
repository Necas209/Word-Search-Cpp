#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "lib/json.hpp"
#include "word.h"

using json = nlohmann::json;

typedef size_t dimension_t;
typedef uint16_t difficulty_t;

enum class difficulty : difficulty_t {
    easy = 1,
    medium = 2,
    hard = 3
};

std::istream &operator>>(std::istream &in, difficulty &difficulty);

class board final {
    dimension_t width_ = 0;
    dimension_t height_ = 0;
    difficulty difficulty_{difficulty::easy};
    std::vector<word> words_{};
    std::vector<std::vector<char> > letters_{};

    [[nodiscard]] auto random_x(const std::string &word,
                                orientation orientation) const -> std::optional<point_t>;

    [[nodiscard]] auto random_y(const std::string &word,
                                orientation orientation) const -> std::optional<point_t>;

    [[nodiscard]] bool word_fits(const word &word) const;

    void insert_word(const word &word);

public:
    board() = default;

    board(const dimension_t width, const dimension_t height, const difficulty difficulty)
        : width_(width), height_(height), difficulty_(difficulty) {
        letters_.resize(height, std::vector(width, ' '));
    }

    board(const board &board) = default;

    board(board &&board) noexcept = default;

    ~board() = default;

    [[nodiscard]] auto width() const { return width_; }

    [[nodiscard]] auto height() const { return height_; }

    [[nodiscard]] auto words_count() const { return words_.size(); }

    [[nodiscard]] auto words_found() const {
        return std::ranges::count_if(words_, [](const word &word) { return word.found(); });
    }

    void fill_letters(std::vector<std::string> &words, size_t max_num_words);

    void show_words() const;

    void show_letters() const;

    bool find_word(const word &search_word);

    /// @brief Create a new board with the specified dimensions and difficulty.
    /// @return A new board.
    /// @throw std::invalid_argument if the dimensions are invalid.
    /// @throw std::invalid_argument if the difficulty is invalid.
    static auto new_board() -> board;

    /// @brief Check if the specified dimension is valid.
    /// @param dimension The dimension to check.
    /// @param difficulty The difficulty of the board.
    /// @return True if the dimension is valid, false otherwise.
    static auto check_dimension(dimension_t dimension, difficulty difficulty) -> bool;

    friend void to_json(json &j, const board &board) {
        j = json{
            {"width", board.width_},
            {"height", board.height_},
            {"difficulty", board.difficulty_},
            {"words", board.words_},
            {"letters", board.letters_}
        };
    }

    friend void from_json(const json &j, board &board) {
        j.at("width").get_to(board.width_);
        j.at("height").get_to(board.height_);
        j.at("difficulty").get_to(board.difficulty_);
        j.at("words").get_to(board.words_);
        j.at("letters").get_to(board.letters_);
    }
};

auto get_orientations(difficulty difficulty) -> std::vector<orientation>;

#endif // BOARD_H
