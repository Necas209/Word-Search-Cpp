#include "Easy_Board.h"

bool Easy_Board::Ask_DimX() {
    int dim_x;
    std::cout << std::endl << " Insira a dimensão X da matriz: ";
    std::cin >> dim_x;
    if (!std::cin.good() or dim_x < 5) {
        PreventLoop();
        return false;
    }
    DimX = dim_x;
    return true;
}

bool Easy_Board::Ask_DimY() {
    int dim_y;
    std::cout << std::endl << " Insira a dimensão Y da matriz: ";
    std::cin >> dim_y;
    if (!std::cin.good() or dim_y < 5) {
        PreventLoop();
        return false;
    }
    DimY = dim_y;
    return true;
}

void Easy_Board::Create_matrix() {
    std::cout << std::endl << "\tDimensões da sopa de letras mínimas: 5 x 5" << std::endl;
    Board::Create_matrix();
}

void Easy_Board::Fill_matrix() {
    constexpr int orientation[] = {FRONT,DOWN,FRONT_DOWN,FRONT_UP};
    int k = 0;
    srand(static_cast<unsigned>(time(nullptr)));
    const auto start = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed{};
    for (int i = 0; i < num_words; i++) {
        if (k == 10)
            break;

        if (size_t l = words[i].length(); l <= DimX && l <= DimY) {
            do {
                words[i].Set_orientation(orientation[rand() % 4]);
                words[i].RandPoint(DimX, DimY);
                auto end = std::chrono::high_resolution_clock::now();
                elapsed = end - start;
            } while (!Check_Crossing(i) and elapsed.count() < 1);
            if (Check_Crossing(i)) {
                words[i].Set_state(NOT_FOUND);
                Insert_Word(i);
                k++;
            }
        }
        i++;
    }
    for (int i = 0; i < DimY; i++) {
        for (int j = 0; j < DimX; j++) {
            if (matrix[i][j] == ' ')
                matrix[i][j].Rand_letter();
        }
    }
}

void Easy_Board::Show_list() {
    int k = 0;
    Board::Show_list();
    gotoxy(2 * DimX + 50, 1);
    std::cout << "Palavras por encontrar:";
    for (int i = 0; i < num_words; i++) {
        if (words[i] != NOT_FOUND) continue;
        gotoxy(2 * DimX + 50, 3 + 2 * k);
        std::cout << words[i];
        k++;
    }
}

void Easy_Board::Save(std::ofstream &os) {
    os << "E: ";
    Board::Save(os);
}
