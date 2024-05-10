#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include "lib/json.hpp"

using json = nlohmann::json;

namespace chr = std::chrono;

// ReSharper disable once CppRedundantNamespaceDefinition
namespace nlohmann {
    template<typename Rep, typename Ratio>
    struct adl_serializer<std::chrono::duration<Rep, Ratio> > {
        template<typename JsonType>
        static void to_json(JsonType &json, const std::chrono::duration<Rep, Ratio> &duration) {
            json = duration.count();
        }

        template<typename JsonType>
        static void from_json(JsonType &json, std::chrono::duration<Rep, Ratio> &duration) {
            duration = std::chrono::duration<Rep, Ratio>(json.template get<Rep>());
        }
    };
}

class word_search final {
    chr::time_point<chr::high_resolution_clock> start_{};
    chr::duration<double> elapsed_{};
    board board_;
    player player_;

public:
    word_search() = default;

    word_search(board &board, player &player)
        : board_(std::move(board)), player_(std::move(player)) {
    }

    ~word_search() = default;

    void play();

    void exit_game();

    void save_game(const std::string &filename = "save.json") const;

    static auto load_game(const std::string &filename = "save.json") -> word_search;

    void find_word();

    void round();

    friend void from_json(const json &j, word_search &game) {
        j.at("elapsed").get_to(game.elapsed_);
        j.at("board").get_to(game.board_);
        j.at("player").get_to(game.player_);
    }

    friend void to_json(json &j, const word_search &game) {
        j = json{
            {"elapsed", game.elapsed_},
            {"board", game.board_},
            {"player", game.player_},
        };
    }
};

auto load_words() -> std::vector<std::string>;

#endif // GAME_H
