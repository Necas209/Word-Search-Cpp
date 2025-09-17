#include "game.hpp"
#include "ui.hpp"

int main()
{
    auto screen = ftxui::ScreenInteractive::Fullscreen();
    while (true)
    {
        word_search::game game;
        switch (ui::main_menu(screen))
        {
        case ui::menu_option::new_game:
            game = word_search::game::new_game(screen);
            game.play(screen);
            break;
        case ui::menu_option::continue_game:
            game = word_search::game::load_game();
            game.play(screen);
            break;
        case ui::menu_option::exit:
            return 0;
        }
    }
}
