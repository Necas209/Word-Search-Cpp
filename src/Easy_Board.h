#ifndef EASY_BOARD_H

#define EASY_BOARD_H

#include "Board.h"

#include <fstream>

class Easy_Board final : public Board {
public:
    Easy_Board() = default;

    ~Easy_Board() override = default;

    bool Ask_DimX() override;

    bool Ask_DimY() override;

    void Create_matrix() override;

    void Fill_matrix() override;

    void Show_list() override;

    void Save(std::ofstream &os) override;
};

#endif // EASY_BOARD_H
