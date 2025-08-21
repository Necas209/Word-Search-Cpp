#pragma once

#include <chrono>
#include <filesystem>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "board.hpp"
#include "player.hpp"

using json = nlohmann::json;

// ReSharper disable once CppRedundantNamespaceDefinition
namespace nlohmann
{
    template <typename Rep, typename Ratio>
    struct adl_serializer<std::chrono::duration<Rep, Ratio>>
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
}

class game final
{
    using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

    board board_;
    player player_;
    time_point_t start_{};
    time_point_t::duration elapsed_{};

public:
    constexpr game() noexcept = default;
    constexpr explicit game(board board, player player) noexcept;

    void play();
    void pause_game();
    void save_game(const std::filesystem::path& filename = "data/save.json") const;
    void find_word();

    static void new_game();
    static void load_game(const std::filesystem::path& filename = "data/save.json");

    friend void from_json(const json& j, game& game);
    friend void to_json(json& j, const game& game);
};

auto load_words() -> std::vector<std::string>;
