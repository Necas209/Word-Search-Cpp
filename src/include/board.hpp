#ifndef WORD_SEARCH_BOARD_HPP
#define WORD_SEARCH_BOARD_HPP

#include "word.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/table.hpp"

namespace word_search
{
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

    auto get_orientations(difficulty difficulty) -> std::span<const orientation>;

    class board final
    {
        using dimension_t = std::size_t;
        dimension_t width_{};
        dimension_t height_{};
        std::string letters_;
        std::vector<word> words_{};
        difficulty difficulty_{};

    public:
        constexpr board() noexcept = default;

        constexpr explicit board(const dimension_t width, const dimension_t height, const difficulty difficulty)
            noexcept : width_(width), height_(height), letters_(width * height, ' '), difficulty_(difficulty)
        {
        }

        [[nodiscard]] auto width() const -> dimension_t { return width_; }
        [[nodiscard]] auto height() const -> dimension_t { return height_; }
        [[nodiscard]] auto words() const -> const std::vector<word>& { return words_; }
        [[nodiscard]] auto difficulty() const -> difficulty { return difficulty_; }

        [[nodiscard]] auto operator[](const dimension_t x, const dimension_t y) const -> char
        {
            return letters_[y * width_ + x];
        }

        [[nodiscard]] auto operator[](const dimension_t x, const dimension_t y) -> char&
        {
            return letters_[y * width_ + x];
        }

        [[nodiscard]] auto solved() const -> bool;

        [[nodiscard]] auto words_found() const -> std::int64_t;

        [[nodiscard]] auto find_word(const word& search_word) -> word_state;

        auto load_words(const std::vector<std::string>& words) -> void;

        friend void from_json(const nlohmann::json& j, board& board)
        {
            j.at("width").get_to(board.width_);
            j.at("height").get_to(board.height_);
            j.at("difficulty").get_to(board.difficulty_);
            j.at("letters").get_to(board.letters_);
            j.at("words").get_to(board.words_);
        }

        friend void to_json(nlohmann::json& j, const board& board)
        {
            j = nlohmann::json{
                {"width", board.width_},
                {"height", board.height_},
                {"difficulty", board.difficulty_},
                {"letters", board.letters_},
                {"words", board.words_}
            };
        }

        [[nodiscard]] static auto new_board(ftxui::ScreenInteractive& screen) -> board;

        auto pick_theme(ftxui::ScreenInteractive& screen) -> void;

        [[nodiscard]] auto letters_panel() const -> ftxui::Table;

    private:
        [[nodiscard]] auto random_pt(const std::string& word, orientation orientation) const -> point_t;

        [[nodiscard]] auto word_fits(const word& word) const -> bool;

        auto add_word(const word& word) -> void;
    };
}

#endif //WORD_SEARCH_BOARD_HPP
