#ifndef PLAYER_H

#define PLAYER_H

#include <chrono>
#include <iostream>
#include <print>
#include <string>

#include "lib/json.hpp"

using json = nlohmann::json;

typedef uint16_t level_t;

enum class level : level_t {
    beginner = 1,
    expert = 2,
};

inline std::istream &operator>>(std::istream &is, level &level) {
    level_t level_value;
    is >> level_value;
    level = static_cast<::level>(level_value);
    return is;
}

class player final {
    std::string name_{};
    uint16_t age_{};
    int score_{};
    level level_{level::beginner};

public:
    player() = default;

    player(std::string &name, const uint16_t age, const level level)
        : name_(std::move(name)), age_(age), level_(level) {
    }

    player(const player &player) = default;

    player(player &&player) noexcept = default;

    ~player() = default;

    [[nodiscard]] auto name() const { return name_; }

    [[nodiscard]] auto age() const { return age_; }

    void update_score(const int points) {
        score_ += points;
    }

    [[nodiscard]] auto final_score(const std::chrono::seconds elapsed, const uint32_t num_words) const -> long long {
        switch (level_) {
            case level::beginner:
                return score_;
            case level::expert: {
                const auto max_time = num_words * std::chrono::seconds{60};
                const auto bonus = (max_time - elapsed) / 10;
                return score_ + bonus.count();
            }
            default:
                throw std::invalid_argument("Invalid level.");
        }
    }

    static auto new_player() -> player {
        std::print("What is your name? ");
        std::string name;
        std::getline(std::cin, name);

        std::print("\nWhat is your age? ");
        uint16_t age;
        std::cin >> age;
        std::cin.ignore();

        std::println("\nWhat is your level?");
        std::println("\t1 - Beginner");
        std::println("\t2 - Expert");
        std::print("Option: ");
        level level;
        std::cin >> level;
        std::cin.ignore();

        return {name, age, level};
    }

    friend void to_json(json &j, const player &player) {
        j = json{
            {"name", player.name_},
            {"age", player.age_},
            {"score", player.score_},
            {"level", player.level_}
        };
    }

    friend void from_json(const json &j, player &player) {
        j.at("name").get_to(player.name_);
        j.at("age").get_to(player.age_);
        j.at("score").get_to(player.score_);
        j.at("level").get_to(player.level_);
    }
};

#endif // PLAYER_H
