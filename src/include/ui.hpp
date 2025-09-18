#ifndef WORD_SEARCH_UI_HPP
#define WORD_SEARCH_UI_HPP

#include <string>
#include <vector>

#include "ftxui/component/screen_interactive.hpp"

namespace ui
{
    enum class menu_option
    {
        new_game,
        continue_game,
        exit
    };

    auto main_menu(ftxui::ScreenInteractive& screen) -> menu_option;

    [[nodiscard]] auto select(ftxui::ScreenInteractive& screen,
                              const std::string& title,
                              const std::vector<std::string>& entries) -> int;
}

#endif //WORD_SEARCH_UI_HPP
