#include "player.hpp"

#include <iostream>

auto word_search::player::update_score(const std::int32_t points) -> void
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

auto word_search::player::final_score(const std::chrono::seconds elapsed,
                                      const std::int64_t num_words) const -> std::int64_t
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
    }
    throw std::invalid_argument("Invalid level.");
}

auto word_search::new_player() -> player
{
    std::print("What is your name? ");
    std::string name;
    std::getline(std::cin, name);

    std::print("\nWhat is your age? ");
    std::uint16_t age;
    std::cin >> age;
    std::cin.ignore();

    std::println("\nWhat is your level?");
    std::println("\t1 - Beginner");
    std::println("\t2 - Expert");
    std::print("Option: ");
    std::underlying_type_t<level> level;
    std::cin >> level;
    std::cin.ignore();

    return player{std::move(name), age, static_cast<word_search::level>(level)};
}
