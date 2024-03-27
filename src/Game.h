#ifndef GAME_H

#define GAME_H

#include "Easy_Board.h"
#include "Expert.h"

class Game final {
    std::unique_ptr<Board> board;
    std::unique_ptr<Player> player;

public:
    Game() = default;

    ~Game() = default;

    void Menu();

    bool Choose_Level();

    void New_Game();

    void Run_Game();

    void Save_Game() const;

    void Load_Game();

    void Clear_Game();

    void Play() const;
};

#endif // GAME_H
