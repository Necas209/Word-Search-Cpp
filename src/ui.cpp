#include "ui.hpp"

#include "game.hpp"

auto ui::main_menu(ftxui::ScreenInteractive& screen) -> menu_option
{
    using namespace ftxui;
    const auto has_save = std::filesystem::exists(word_search::game::save_file());

    auto entries = std::vector<std::string>{"Start new game", "Exit"};
    if (has_save)
    {
        entries.insert(entries.begin() + 1, "Continue last game");
    }

    int selected = 0;
    auto option = MenuOption::Vertical();
    option.on_enter = screen.ExitLoopClosure();
    const auto menu = Menu(entries, &selected, option);

    const auto component = Renderer(menu, [=]
    {
        return vbox({
            text("Welcome to the Word Search Game!") | bold | center,
            filler() | size(HEIGHT, EQUAL, 1),
            text("Author: Diogo Medeiros") | dim | center,
            separator(),
            menu->Render() | center,
        }) | border | center;
    });
    screen.Loop(component);
    switch (selected)
    {
    case 0:
        return menu_option::new_game;
    case 1:
        return has_save ? menu_option::continue_game : menu_option::exit;
    case 2:
        return menu_option::exit;
    default:
        throw std::runtime_error("Invalid menu option");
    }
}

auto ui::select(ftxui::ScreenInteractive& screen,
                const std::string& title,
                const std::vector<std::string>& entries) -> int
{
    using namespace ftxui;
    int selected = 0;

    auto option = MenuOption::Vertical();
    option.on_enter = screen.ExitLoopClosure();
    const auto menu = Menu(entries, &selected, option);

    const auto layout = Renderer(menu, [&title, menu]
    {
        return vbox({
                text(title) | bold | center,
                separator(),
                menu->Render(),
                separator(),
                text("Use Up/Down, Enter to select.") | dim | center,
            }) |
            border | size(WIDTH, LESS_THAN, 30) | center;
    });
    screen.Loop(layout);

    return selected;
}
