#ifndef WORD_H

#define WORD_H

#include <iostream>
#include <fstream>
#include "Letter.h"

// States
#define NOT_USED 0
#define NOT_FOUND 1
#define FOUND 2
// Orientations
#define NONE 0
#define FRONT 1
#define BACK 2
#define DOWN 3
#define UP 4
#define FRONT_DOWN 5
#define BACK_UP 6
#define BACK_DOWN 7
#define FRONT_UP 8

class Word final {
    std::string str;
    int orientation = NONE;
    int state = NOT_USED;
    Point initial_point;

public:
    Word() = default;

    explicit Word(std::string str) : str(std::move(str)) {
    }

    ~Word() = default;

    void Set_orientation(const int orientation) { this->orientation = orientation; };
    void Set_state(const int state) { this->state = state; };
    [[nodiscard]] std::string Get_str() const { return str; };
    [[nodiscard]] int Get_orientation() const { return orientation; };
    [[nodiscard]] int Get_state() const { return state; };

    [[nodiscard]] int Get_initial_x() const { return initial_point.Get_x(); }

    [[nodiscard]] int Get_initial_y() const { return initial_point.Get_y(); }

    void RandPoint(int DimX, int DimY);

    void RandOrientation();

    void Ask2Set_W();

    void Upper_Case() {
        transform(str.begin(), str.end(), str.begin(), ::toupper);
    }

    [[nodiscard]] size_t length() const { return str.length(); }

    void Read(std::ifstream &is);

    void Save(std::ofstream &os) const;

    void Show() const;

    bool operator==(const Word &word) const { return str == word.Get_str() && initial_point == word.initial_point; }

    bool operator!=(const Word &word) const { return *this == word; }

    bool operator==(const int state) const { return state == this->state; }

    bool operator!=(const int state) const { return state != this->state; }

    Word &operator=(const Word &word) {
        str = word.Get_str();
        orientation = word.Get_orientation();
        state = word.Get_state();
        initial_point = word.initial_point;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Word &word) {
        os << word.Get_str();
        return os;
    }
};

#endif // !WORD_H
