#include "Word.h"
#include <string>

void Word::RandPoint(const int DimX, const int DimY) {
    int x = 0, y = 0;
    const size_t h = str.length() - 1;
    switch (orientation) {
        case FRONT:
            x = rand() % (DimX - h);
            y = rand() % DimY;
            break;
        case BACK:
            x = h + rand() % (DimX - h);
            y = rand() % DimY;
            break;
        case DOWN:
            x = rand() % DimX;
            y = rand() % (DimY - h);
            break;
        case UP:
            x = rand() % DimX;
            y = h + rand() % (DimY - h);
            break;
        case FRONT_DOWN:
            x = rand() % (DimX - h);
            y = rand() % (DimY - h);
            break;
        case BACK_UP:
            x = h + rand() % (DimX - h);
            y = h + rand() % (DimY - h);
            break;
        case BACK_DOWN:
            x = h + rand() % (DimX - h);
            y = rand() % (DimY - h);
            break;
        case FRONT_UP:
            x = rand() % (DimX - h);
            y = h + rand() % (DimY - h);
            break;
        default: break;
    }
    initial_point = Point(x, y);
}

void Word::RandOrientation() {
    orientation = rand() % 8 + 1;
}

void Word::Ask2Set_W() {
    std::cout << std::endl << " Palavra: ";
    std::cin.ignore();
    getline(std::cin, str);
    initial_point.Ask2Set_P();
}

void Word::Read(std::ifstream &is) {
    is >> orientation >> state;
    initial_point.Read(is);
    std::getline(is, str, ';');
    str.erase(str.begin());
}

void Word::Save(std::ofstream &os) const {
    os << orientation << ' ';
    os << state << ' ';
    initial_point.Save(os);
    os << ' ' << str << ';';
}

void Word::Show() const {
    std::cout << "Palavra: " << str << std::endl;
    std::cout << "Orientação: " << orientation << std::endl;
    std::cout << "Estado: " << state << std::endl;
    initial_point.ShowPoint();
}
