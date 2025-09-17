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
    const std::vector<std::string> levels = {"Beginner", "Expert"};
    int level_idx = 0; // 0 -> Beginner, 1 -> Expert

    bool cancelled = false;

    auto in_name = Input(&name, "your name");
    auto in_age = Input(&age_str, "age (number)");
    auto rb_level = Radiobox(&levels, &level_idx);

    auto btn_ok = Button("OK", [&]
    {
        screen.Exit();
    });
    auto btn_cancel = Button("Cancel", [&]
    {
        cancelled = true;
        screen.Exit();
    });

    const auto container = Container::Vertical({
        in_name,
        in_age,
        rb_level,
        Container::Horizontal({btn_ok, btn_cancel}),
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
            separator(),
            hbox({
                filler(),
                btn_ok->Render() | bold,
                text("  "),
                btn_cancel->Render(),
                filler(),
            }),
        }) | border | center | size(WIDTH, GREATER_THAN, 40);
    });
    screen.Loop(view);

    constexpr std::string default_name = "Jean Doe";
    constexpr std::uint16_t default_age = 99;

    if (cancelled)
    {
        return player{default_name, default_age, level_t::beginner};
    }

    if (name.empty())
    {
        name = default_name;
    }

    std::uint16_t age;
    try
    {
        age = static_cast<std::uint16_t>(std::stoul(age_str));
    }
    catch (...)
    {
        age = default_age;
    }

    // Map 0/1 -> 1/2 to match previous numeric level inputs.
    const auto level = static_cast<word_search::level_t>(level_idx + 1);

    return player{std::move(name), age, level};
}
