#ifndef POINT_H

#define POINT_H

#include <fstream>
#include <iostream>

#include "functions.h"

class Point final {
    int x = -1;
    int y = -1;

public:
    Point() = default;

    Point(const int m_x, const int m_y) : x(m_x), y(m_y) {
    }

    ~Point() = default;

    bool Ask2Set_P() {
        int x;
        std::cout << std::endl << " X: ";
        std::cin >> x;
        if (!std::cin.good()) {
            PreventLoop();
            return false;
        }
        int y;
        std::cout << std::endl << " Y: ";
        std::cin >> y;
        if (!std::cin.good()) {
            PreventLoop();
            return false;
        }
        this->x = x;
        this->y = y;
        return IsValid();
    }

    [[nodiscard]] int Get_x() const { return x; }
    [[nodiscard]] int Get_y() const { return y; }

    [[nodiscard]] bool IsValid() const { return x >= 0 && y >= 0; }

    void ShowPoint() const { std::cout << "(x,y)" << '=' << '(' << x << ',' << y << ')'; }

    bool operator==(const Point &point) const { return x == point.x && y == point.y; }

    bool operator!=(const Point &point) const { return !(*this == point); }

    Point &operator=(const Point &point) = default;

    void Save(std::ofstream &os) const {
        os << '(' << x << ',' << y << ')';
    }

    void Read(std::ifstream &is) {
        char c;
        is >> c >> x >> c >> y >> c;
    }
};

#endif //POINT_H
