#include "Game.h"

#include "Beginner.h"
#include "Hard_Board.h"

#include <thread>

using hr_clock = std::chrono::high_resolution_clock;

void Game::Menu() {
    int option;
    setlocale(LC_ALL, "");
    system("cls");
    std::cout << std::endl << " Diogo Medeiros n.o 70633" << std::endl;
    std::cout << std::endl << "\t\t\t\tSopa de letras" << std::endl << std::endl;
    std::cout << std::endl << "\tMenu:" << std::endl;
    std::cout << std::endl << " 1 -> Come�ar um novo jogo" << std::endl;
    std::cout << std::endl << " 2 -> Carregar um jogo" << std::endl;
    std::cout << std::endl << " 3 -> Sair" << std::endl;
    std::cout << std::endl << " Op��o: ";
    std::cin >> option;
    if (!std::cin.good())
        PreventLoop();
    else {
        switch (option) {
            case 1:
                New_Game();
                Run_Game();
                break;
            case 2:
                Load_Game();
                break;
            case 3:
                exit(0);
            default:
                std::cout << " Op��o inv�lida.";
                system("cls");
                Menu();
        }
    }
}

bool Game::Choose_Level() {
    int option;
    std::cout << std::endl << " N�vel do jogador: " << std::endl;
    std::cout << std::endl << "\t1 -> Principiante" << std::endl;
    std::cout << std::endl << "\t2 -> Experiente" << std::endl;
    std::cout << std::endl << "\tOp��o: ";
    std::cin >> option;
    if (!std::cin.good() or (option != 1 and option != 2)) {
        PreventLoop();
        return false;
    }
    switch (option) {
        case 1:
            player = std::make_unique<Beginner>();
            board = std::make_unique<Easy_Board>();
            break;
        case 2:
            player = std::make_unique<Expert>();
            board = std::make_unique<Hard_Board>();
            break;
        default:
            break;
    }
    player->Ask_name();
    player->Ask_age();
    return true;
}

void Game::New_Game() {
    system("cls");
    Choose_Level();
    system("cls");
    board->Create_matrix();
    board->Load_list();
    board->Fill_matrix();
}

void Game::Run_Game() {
    const auto start = hr_clock::now();
    while (board->Number_NOT_FOUND() > 0) {
        system("CLS");
        board->Show_matrix();
        board->Show_list();
        Play();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    const auto end = hr_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    system("cls");
    board->Show_matrix();
    board->Show_list();
    player->Final_Score(elapsed, board->Number_USED());
    gotoxy(0, board->Get_DimY() + 3);
    player->Show();
    std::cout << std::endl << " Ganhaste!!!" << std::endl;
    delete player;
    delete board;
    system("pause");
}

void Game::Save_Game() const {
    std::ofstream save("save.txt");
    if (!save) {
        std::cout << std::endl << "Erro ao abrir o ficheiro." << std::endl;
        return;
    }
    player->Save(save);
    save << "\n";
    board->Save(save);
}

void Game::Load_Game() {
    std::ifstream load("save.txt");
    if (!load) {
        std::cout << std::endl << "Erro ao abrir o ficheiro." << std::endl;
        return;
    }
    char c;
    std::string s;
    load >> c;
    if (c == 'B') {
        player = std::make_unique<Beginner>();
    } else {
        player = std::make_unique<Expert>();
    }
    load >> c;
    player->Read(load);
    getline(load, s);
    load >> c;
    if (c == 'E') {
        board = std::make_unique<Easy_Board>();
    } else {
        board = std::make_unique<Hard_Board>();
    }
    board->Read(load);
    Run_Game();
}

void Game::Clear_Game() {
    delete player;
    delete board;
}

void Game::Play() const {
    int option = 0;
    const int l = board->Get_DimY() + 2;
    for (int i = l; i < l + 20; i++) {
        for (int j = 1; j < 30; j++) {
            showCharAt(j, i, ' ');
        }
    }
    gotoxy(1, l);
    std::cout << std::endl << "\t1 -> Introduzir uma palavra" << std::endl;
    std::cout << std::endl << "\t2 -> Salvar o jogo" << std::endl;
    std::cout << std::endl << "\tOp��o: ";
    std::cin >> option;
    if (!std::cin.good())
        PreventLoop();
    else {
        bool flag;
        Word word;
        switch (option) {
            case 1:
                word.Ask2Set_W();
                flag = board->Check_If_Word_Is_Present(word);
                if (flag) {
                    std::cout << std::endl << "\tCerto!!" << std::endl;
                }
                player->Update_Score(flag);
                break;
            case 2:
                Save_Game();
                break;
            default:
                std::cout << std::endl << "\t Op��o inv�lida." << std::endl;
        }
    }
}
