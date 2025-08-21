#pragma once

#include <vector>
#include "word.hpp"
#include "nlohmann/json.hpp"

enum class difficulty : std::uint16_t
{
    easy = 1,
    medium = 2,
    hard = 3
};

enum class word_state : std::uint8_t
{
    not_found = 0,
    found = 1,
    already_found = 2,
};

std::istream& operator>>(std::istream& in, difficulty& difficulty);

class board
{
    using dimension_t = std::size_t;

    dimension_t width_{};
    dimension_t height_{};
    difficulty difficulty_{};
    std::string letters_;
    std::vector<word> words_{};

    [[nodiscard]] auto random_x(const std::string& word, orientation orientation) const -> std::optional<std::size_t>;
    [[nodiscard]] auto random_y(const std::string& word, orientation orientation) const -> std::optional<std::size_t>;
    [[nodiscard]] auto word_fits(const std::string& word, orientation orientation, const point_t& point) const -> bool;
    void insert_word(const word& word);

public:
    constexpr board() noexcept = default;
    constexpr explicit board(dimension_t width, dimension_t height, difficulty difficulty) noexcept;

    [[nodiscard]] auto width() const -> dimension_t { return width_; }
    [[nodiscard]] auto height() const -> dimension_t { return height_; }
    [[nodiscard]] auto words() const -> const std::vector<word>& { return words_; }
    [[nodiscard]] auto words() -> std::vector<word>& { return words_; }

    [[nodiscard]] auto operator[](size_t x, size_t y) const -> char;
    [[nodiscard]] auto operator[](size_t x, size_t y) -> char&;

    [[nodiscard]] auto solved() const -> bool;
    [[nodiscard]] auto words_found() const -> std::size_t;
    [[nodiscard]] auto find_word(const word& search_word) -> word_state;

    void fill_letters(std::vector<std::string>& words, size_t max_num_words);
    void show_words() const;
    void show_letters() const;

    static auto new_board() -> board;
    static auto check_dimension(dimension_t dimension, ::difficulty difficulty) -> bool;

    friend void from_json(const nlohmann::json& j, board& board);
    friend void to_json(nlohmann::json& j, const board& board);
};

 constexpr auto get_orientations(difficulty difficulty) -> std::span<const orientation>;
