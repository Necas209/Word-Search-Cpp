#ifndef WORD_SEARCH_PLAYER_HPP
#define WORD_SEARCH_PLAYER_HPP

#include <nlohmann/json.hpp>

#include "ftxui/component/screen_interactive.hpp"

namespace word_search
{
    enum class level : std::uint16_t
    {
        beginner = 1,
        expert = 2,
    };

    class player final
    {
        std::string name_;
        std::int64_t score_{};
        std::uint16_t age_{};
        level level_{};

    public:
        constexpr player() noexcept = default;

        constexpr explicit player(std::string name, const std::uint16_t age, const level level) noexcept
            : name_(std::move(name)), age_(age), level_(level)
        {
        }

        [[nodiscard]] auto name() const -> const std::string& { return name_; }
        [[nodiscard]] auto age() const -> std::uint16_t { return age_; }
        [[nodiscard]] auto level() const -> level { return level_; }
        [[nodiscard]] auto score() const -> std::int64_t { return score_; }

        auto update_score(std::int32_t points) -> void;

        [[nodiscard]] auto final_score(std::chrono::seconds elapsed, std::int64_t num_words) const -> std::int64_t;

        [[nodiscard]] static auto new_player(ftxui::ScreenInteractive& screen) -> player;

        friend void to_json(nlohmann::json& j, const player& player)
        {
            j = nlohmann::json{
                {"name", player.name_},
                {"age", player.age_},
                {"score", player.score_},
                {"level", player.level_}
            };
        }

        friend void from_json(const nlohmann::json& j, player& player)
        {
            j.at("name").get_to(player.name_);
            j.at("age").get_to(player.age_);
            j.at("score").get_to(player.score_);
            j.at("level").get_to(player.level_);
        }
    };
}

#endif //WORD_SEARCH_PLAYER_HPP
