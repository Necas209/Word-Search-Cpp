#include "game.hpp"
#include "ui.hpp"

int main()
{
    auto screen = ftxui::ScreenInteractive::Fullscreen();
    while (true)
    {
        switch (ui::main_menu(screen))
        {
        case ui::menu_option::new_game:
            {
                auto game = word_search::game::new_game(screen);
                game.play(screen);
                break;
            }
        case ui::menu_option::continue_game:
            {
                const auto save_file = word_search::game::save_file();
                auto game = word_search::game::load_game(save_file);
                game.play(screen);
                break;
            }
        case ui::menu_option::exit:
            return 0;
        }
    }
}
