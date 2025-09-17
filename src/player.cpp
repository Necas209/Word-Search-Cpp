#include "player.hpp"

#include "ui.hpp"
#include "ftxui/component/component.hpp"

auto word_search::player::update_score(const std::int32_t points) -> void
{
    score_ += points;
    if (score_ < 0 && level_ == level_t::beginner)
    {
        score_ = 0;
    }
}

auto word_search::player::final_score(const std::chrono::seconds elapsed,
                                      const std::int64_t num_words) const -> std::int64_t
{
    using namespace std::chrono_literals;
    switch (level_)
    {
    case level_t::beginner:
        return score_;
    case level_t::expert:
        {
            const auto max_time = num_words * 60s;
            const auto bonus = (max_time - elapsed) / 10;
            return score_ + bonus.count();
        }
    }
    throw std::invalid_argument("Invalid level.");
}

auto word_search::player::new_player(ftxui::ScreenInteractive& screen) -> player
{
    using namespace ftxui;

    std::string name;
    std::string age_str;
    int level_idx = 0;

    auto in_name = Input(&name, "your name");
    auto in_age = Input(&age_str, "age (number)");
    const std::vector<std::string> levels = {"Beginner", "Expert"};
    auto rb_level = Radiobox(&levels, &level_idx);

    std::uint16_t age;
    level_t level;
    std::string error_msg;

    auto btn_ok = Button("OK", [&]
    {
        if (name.empty())
        {
            error_msg = "Name cannot be empty.";
            return;
        }
        try
        {
            age = static_cast<std::uint16_t>(std::stoul(age_str));
        }
        catch (...)
        {
            error_msg = "Age must be a positive integer.";
            return;
        }
        level = static_cast<level_t>(level_idx);
        screen.Exit();
    });

    const auto container = Container::Vertical({
        in_name,
        in_age,
        rb_level,
        btn_ok,
    });

    const auto view = Renderer(container, [&]
    {
        return vbox({
            text("New Player") | bold | center,
            separator(),
            hbox(text("Name: ") | bold, in_name->Render()) | center,
            hbox(text("Age:  ") | bold, in_age->Render()) | center,
            separator(),
            hbox(text("Level:") | bold, rb_level->Render()) | center,
            error_msg.empty() ? filler() : (separator(), text(error_msg) | color(Color::Red) | center),
            separator(),
            btn_ok->Render() | bold | center,
        }) | border | center | size(WIDTH, GREATER_THAN, 40);
    });
    screen.Loop(view);

    return player{std::move(name), age, level};
}
