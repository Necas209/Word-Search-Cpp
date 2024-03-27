#ifndef BOARD_H

#define BOARD_H

#include <vector>
#include <fstream>

#include "Letter.h"
#include "Word.h"

class Board {
protected:
    Letter **matrix{};
    std::vector<Word> words;
    int DimX = 0;
    int DimY = 0;
    size_t num_words = 0;

public:
    Board() = default;

    virtual ~Board() = default;

    [[nodiscard]] int Get_DimX() const { return DimX; };
    [[nodiscard]] int Get_DimY() const { return DimY; };

    [[nodiscard]] int Number_USED() const;

    [[nodiscard]] int Number_NOT_FOUND() const;

    virtual bool Ask_DimX() = 0;

    virtual bool Ask_DimY() = 0;

    virtual void Create_matrix();

    virtual void Fill_matrix() = 0;

    virtual void Show_list();

    void Show_matrix() const;

    bool Load_list();

    [[nodiscard]] bool Check_Crossing(int i) const;

    [[nodiscard]] bool Check_Letter(int i, int x, int y, int k) const;

    void Insert_Word(int i) const;

    void Read(std::ifstream &is);

    virtual void Save(std::ofstream &os);

    bool Check_If_Word_Is_Present(Word word);
};

#endif // BOARD_H
