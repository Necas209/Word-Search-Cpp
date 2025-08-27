#ifndef WORD_SEARCH_GAME_HPP
#define WORD_SEARCH_GAME_HPP

#include <chrono>

#include "board.hpp"
#include "player.hpp"

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
    auto load_words() -> std::vector<std::string>;

    class game final
    {
        using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

        board board_;
        player player_;
        time_point_t start_{};
        time_point_t::duration elapsed_{};

    public
    :
        constexpr game() noexcept = default;

        constexpr explicit game(board board, player player) noexcept
            : board_(std::move(board)), player_(std::move(player))
        {
        }

        auto play() -> void;

        auto pause_game() -> void;

        auto save_game(const std::filesystem::path& filename = "data/save.json") const -> void;

        auto find_word() -> void;

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
    };

    auto new_game() -> void;

    auto load_game(const std::filesystem::path& filename = "data/save.json") -> void;
}

#endif //WORD_SEARCH_GAME_HPP
