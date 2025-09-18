#include "game.hpp"

#include <fstream>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

#include <ftxui/component/component.hpp>

namespace rv = std::ranges::views;

auto word_search::game::play(ftxui::ScreenInteractive& screen) -> void
{
    start_ = std::chrono::high_resolution_clock::now();
    while (!board_.solved())
    {
        const auto word = main_loop(screen);
        if (!word)
            return;

        switch (board_.find_word(word.value()))
        {
        case word_state::not_found:
            player_.update_score(-5);
            break;
        case word_state::found:
            player_.update_score(10);
            break;
        case word_state::already_found:
            break;
        }
    }
    end_screen(screen);
}

auto word_search::game::save_game(const std::filesystem::path& filename) const -> void
{
    std::ofstream file{filename};
    if (!file)
    {
        throw std::filesystem::filesystem_error{
            "Could not write file.",
            filename,
            std::make_error_code(std::errc::io_error)
        };
    }

    const nlohmann::json j = *this;
    file << std::setw(2) << j;
}

auto word_search::game::new_game(ftxui::ScreenInteractive& screen) -> game
{
    auto player = player::new_player(screen);
    auto board = board::new_board(screen);
    board.pick_theme(screen);

    return game{std::move(board), std::move(player)};
}

auto word_search::game::load_game(const std::filesystem::path& filename) -> game
{
    if (!std::filesystem::exists(filename))
    {
        throw std::filesystem::filesystem_error{
            "File does not exist.",
            filename,
            std::make_error_code(std::errc::no_such_file_or_directory)
        };
    }

    std::ifstream file{filename};
    if (!file)
    {
        throw std::filesystem::filesystem_error{
            "Could not read file.",
            filename,
            std::make_error_code(std::errc::io_error)
        };
    }

    nlohmann::json j;
    file >> j;
    return j.get<game>();
}

auto word_search::game::end_screen(ftxui::ScreenInteractive& screen) const -> void
{
    using namespace ftxui;
    const auto summary = Renderer([this]
    {
        const auto elapsed_seconds = this->elapsed_seconds();
        const auto final_score = player_.final_score(elapsed_seconds, board_.words_found());
        const auto score_text = std::format("{}'s final score: {}.", player_.name(), final_score);

        return vbox({
            text("Summary") | bold | center,
            separator(),
            hbox(text("Words found: ") | dim, text(std::to_string(board_.words_found())) | bold),
            hbox(text("Time: ") | dim, text(std::format("{}", elapsed_seconds)) | bold),
            separator(),
            text(score_text) | bold,
            separator(),
            text("Press Enter to continue.") | dim | center,
        }) | border | center;
    });

    const auto component = CatchEvent(summary, [&](const Event& e)
    {
        if (e == Event::Return || e == Event::Character(' '))
        {
            screen.Exit();
            return true;
        }
        return false;
    });
    screen.Loop(component);
}

auto word_search::game::main_loop(ftxui::ScreenInteractive& screen) -> std::optional<word>
{
    using namespace ftxui;
    using namespace std::chrono_literals;

    word word;
    const auto find_word_panel = game::find_word_panel(screen, word);

    bool running = true;
    bool quit_requested = false;
    auto screenRedraw = std::thread([&]
    {
        while (running)
        {
            screen.PostEvent(Event::Custom);
            std::this_thread::sleep_for(50ms); // Prevent High CPU Usage.
        }
    });

    const auto quit_button = Button("Save & Quit", [&, this]
    {
        const auto save_file = game::save_file();
        save_game(save_file);
        quit_requested = true;
        screen.Exit();
    });

    const auto left_container = Container::Vertical({
        find_word_panel,
        quit_button,
    });

    // Compose letters and words into a single screen.
    const auto renderer = Renderer(left_container, [=, this]
    {
        const auto end = std::chrono::high_resolution_clock::now();
        elapsed_ += end - start_;
        start_ = end;

        return hbox({
            vbox({
                find_word_panel->Render(),
                filler(),
                quit_button->Render() | center,
            }),
            separator(),
            vbox({
                text("Board") | bold | center,
                separator(),
                board_.letters_panel().Render() | flex | size(WIDTH, GREATER_THAN, 50)
            }),
            separator(),
            progress_panel(),
        }) | border | center;
    });

    screen.Loop(renderer);
    running = false;
    screenRedraw.join();

    if (quit_requested)
        return std::nullopt;

    return word;
}

auto word_search::game::progress_panel() const -> ftxui::Element
{
    using namespace ftxui;

    const auto num_found = board_.words_found();

    auto label = [](std::string s) { return text(std::move(s)) | bold | align_right; };

    const auto& words = board_.words();
    auto items = Elements{
        text("Progress") | bold | center,
        separator(),
        gridbox({
            {label("Player: "), text(player_.name())},
            {label("Age: "), text(std::to_string(player_.age()))},
            {label("Score: "), text(std::to_string(player_.score()))},
            {label("Time: "), text(std::format("{}", elapsed_seconds()))},
        }),
        separator(),
        hbox({
            text("Words: ") | bold,
            text(std::to_string(num_found)) | bold,
            text(" of ") | dim,
            text(std::to_string(words.size())) | bold,
        }),
    };

    // List words: found -> green; in easy mode, show unfound in red; otherwise hide unfound.
    for (const auto& word : words)
    {
        if (word.found())
        {
            items.push_back(text(word.str()) | color(Color::Green));
        }
        else if (board_.difficulty() == difficulty_t::easy)
        {
            items.push_back(text(word.str()) | color(Color::Red));
        }
    }

    return vbox(items) | flex | size(WIDTH, GREATER_THAN, 30);
}

auto word_search::game::find_word_panel(ftxui::ScreenInteractive& screen, word& word) -> ftxui::Component
{
    using namespace ftxui;

    struct State
    {
        std::string str;
        std::string sx;
        std::string sy;
        int orientation = 0;
    };
    auto state = std::make_shared<State>();

    const auto in_word = Input(&state->str, "enter word");
    const auto in_x = Input(&state->sx, "x coordinate");
    const auto in_y = Input(&state->sy, "y coordinate");

    auto option = MenuOption::Vertical();
    option.on_enter = [&, state]
    {
        try
        {
            const auto x = std::stoul(state->sx);
            const auto y = std::stoul(state->sy);
            word = word_search::word{
                cleanup(state->str),
                point_t{x, y},
                static_cast<orientation_t>(state->orientation)
            };
            screen.Exit();
        }
        catch (...)
        {
        }
    };
    const auto entries = orientation_names() | std::ranges::to<std::vector<std::string>>();
    const auto orientation_menu = Menu(entries, &state->orientation, option);

    const auto container = Container::Vertical({
        in_word,
        in_x,
        in_y,
        orientation_menu,
    });

    return Renderer(container, [=]
    {
        return vbox({
            text("Find Word") | bold | center,
            separator(),
            gridbox({
                {text("Word: ") | align_right | bold, in_word->Render()},
                {text("Coord. X: ") | align_right | bold, in_x->Render()},
                {text("Coord. Y: ") | align_right | bold, in_y->Render()},
                {text("Orientation: ") | align_right | bold, orientation_menu->Render()},
            })
        }) | flex | size(WIDTH, GREATER_THAN, 30);
    });
}
