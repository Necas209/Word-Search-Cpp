#include "player.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

std::istream& operator>>(std::istream& is, level& level)
{
    level_t level_value;
    is >> level_value;
    level = static_cast<::level>(level_value);
    return is;
}

constexpr player::player(std::string name, const age_t age, const ::level level)
    : name_(std::move(name)), age_(age), level_(level)
{
}

void player::update_score(int points)
{
    if (score_ >= points)
    {
        score_ -= points;
    }
    else
    {
        score_ = 0;
    }
}

auto player::final_score(const std::chrono::seconds elapsed, const std::size_t num_words) const -> final_score_t
{
    switch (level_)
    {
    case level::beginner:
        return score_;
    case level::expert:
        {
            const auto max_time = num_words * std::chrono::seconds{60};
            const auto bonus = (max_time - elapsed) / 10;
            return score_ + bonus.count();
        }
    default:
        throw std::invalid_argument("Invalid level.");
    }
}

player player::new_player()
{
    std::print("What is your name? ");
    std::string name;
    std::getline(std::cin, name);

    std::print("\nWhat is your age? ");
    age_t age;
    std::cin >> age;
    std::cin.ignore();

    std::println("\nWhat is your level?");
    std::println("\t1 - Beginner");
    std::println("\t2 - Expert");
    std::print("Option: ");
    ::level level;
    std::cin >> level;
    std::cin.ignore();

    return player{std::move(name), age, level};
}

void to_json(nlohmann::json& j, const player& player)
{
    j = nlohmann::json{
            {"name", player.name_},
            {"age", player.age_},
            {"score", player.score_},
            {"level", player.level_}
    };
}

void from_json(const nlohmann::json& j, player& player)
{
    j.at("name").get_to(player.name_);
    j.at("age").get_to(player.age_);
    j.at("score").get_to(player.score_);
    j.at("level").get_to(player.level_);
}
