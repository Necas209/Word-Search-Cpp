#ifndef WORD_SEARCH_GAME_HPP
#define WORD_SEARCH_GAME_HPP

#include <chrono>

#include "board.hpp"
#include "player.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

template <typename Rep, typename Ratio>
struct nlohmann::adl_serializer<std::chrono::duration<Rep, Ratio>>
{
    template <typename JsonType>
    static void to_json(JsonType& json, const std::chrono::duration<Rep, Ratio>& duration)
    {
        json = duration.count();
    }

    template <typename JsonType>
    static void from_json(JsonType& json, std::chrono::duration<Rep, Ratio>& duration)
    {
        duration = std::chrono::duration<Rep, Ratio>(json.template get<Rep>());
    }
};

namespace word_search
{
    class game final
    {
        using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

        board board_;
        player player_;
        time_point_t start_{};
        time_point_t::duration elapsed_{};

    public:
        constexpr game() noexcept = default;

        constexpr explicit game(board board, player player) noexcept
            : board_(std::move(board)), player_(std::move(player))
        {
        }

        auto play(ftxui::ScreenInteractive& screen) -> void;

        auto save_game(const std::filesystem::path& filename = save_file()) const -> void;

        [[nodiscard]] static auto new_game(ftxui::ScreenInteractive& screen) -> game;

        [[nodiscard]] static auto load_game(const std::filesystem::path& filename = save_file()) -> game;

        friend void from_json(const nlohmann::json& j, game& game)
        {
            j.at("elapsed").get_to(game.elapsed_);
            j.at("board").get_to(game.board_);
            j.at("player").get_to(game.player_);
        }

        friend void to_json(nlohmann::json& j, const game& game)
        {
            j = nlohmann::json{
                {"elapsed", game.elapsed_},
                {"board", game.board_},
                {"player", game.player_},
            };
        }

        [[nodiscard]] static constexpr auto save_file() noexcept -> std::filesystem::path
        {
            return "data/save.json";
        }

    private:
        [[nodiscard]] auto elapsed_seconds() const -> std::chrono::seconds
        {
            return std::chrono::duration_cast<std::chrono::seconds>(elapsed_);
        }

        [[nodiscard]] auto main_loop(ftxui::ScreenInteractive& screen) -> std::optional<word>;

        auto end_screen(ftxui::ScreenInteractive& screen) const -> void;

        [[nodiscard]] auto progress_panel() const -> ftxui::Element;

        [[nodiscard]] static auto find_word_panel(ftxui::ScreenInteractive& screen, word& word) -> ftxui::Component;
    };
}

#endif //WORD_SEARCH_GAME_HPP
