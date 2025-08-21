#pragma once

#include <chrono>
#include <string>
#include "nlohmann/json.hpp"

using level_t = std::uint16_t;

enum class level : level_t
{
    beginner = 1,
    expert = 2,
};

std::istream& operator>>(std::istream& is, level& level);

class player final
{
    using age_t = std::uint16_t;
    using final_score_t = std::uint64_t;

    std::string name_;
    age_t age_{};
    level level_{};
    int score_{};

public:
    constexpr player() = default;

    constexpr explicit player(std::string name, age_t age, level level);

    [[nodiscard]] auto name() const -> const std::string& { return name_; }
    [[nodiscard]] auto age() const -> age_t { return age_; }
    [[nodiscard]] auto level() const -> level { return level_; }
    [[nodiscard]] auto score() const -> int { return score_; }

    void update_score(int points);
    [[nodiscard]] auto final_score(std::chrono::seconds elapsed, std::size_t num_words) const -> final_score_t;

    static player new_player();

    friend void to_json(nlohmann::json& j, const player& player);
    friend void from_json(const nlohmann::json& j, player& player);
};
