#include "functions.h"
#include "Windows.h"
#include <iostream>

//changes both foreground and background colors
void setColor(const int foreGround, const int backGround) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), backGround * 16 + foreGround);
}

//changes foreground color while keeping background color
void setForeColor(const int foreGround) {
    int backGround = MY_COLOR_BLACK;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
    backGround = csbiInfo.wAttributes / 16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), backGround * 16 + foreGround);
}

//changes background color while keeping foreground color
void setBackColor(const int backGround) {
    int foreGround = MY_COLOR_BLACK;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
    foreGround = csbiInfo.wAttributes % 16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), backGround * 16 + foreGround);
}

//Colors reset: foreground white, background black
void resetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MY_COLOR_BLACK * 16 + MY_COLOR_WHITE);
}

//allows to place the cursor at any given position
void gotoxy(int x, int y) {
    COORD c = {(SHORT) x, (SHORT) y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

//allows to draw a rectangle (double lines) at any given position
void showRectAt(const int x, const int y, const int width, const int height) {
    setlocale(LC_ALL, "C");
    gotoxy(x, y);
    std::cout << static_cast<char>(201);
    for (int i = 1; i < width; i++) {
        std::cout << static_cast<char>(205);
    }
    std::cout << static_cast<char>(187);
    gotoxy(x, height + y);
    std::cout << static_cast<char>(200);
    for (int i = 1; i < width; i++) {
        std::cout << static_cast<char>(205);
    }
    std::cout << static_cast<char>(188);
    for (int i = y + 1; i < height + y; i++) {
        gotoxy(x, i);
        std::cout << static_cast<char>(186);
        gotoxy(x + width, i);
        std::cout << static_cast<char>(186);
    }
    setlocale(LC_ALL, "");
}

//allows to draw a rectangle (double lines) at any given position, rotated by 90º
void show90RectAt(const int x, const int y, const int width, const int height) {
    showRectAt(x, y, height * 2, width / 2);
}

//show a char at a designated position
void showCharAt(const int x, const int y, const char c) {
    setlocale(LC_ALL, "C");
    gotoxy(x, y);
    std::cout << c;
    setlocale(LC_ALL, "");
}

//write a 'string' verticaly at a designated position
void showVerticalWordAt(const int x, const int y, const std::string &s) {
    setlocale(LC_ALL, "C");
    for (int i = 0; i < s.length(); i++) {
        gotoxy(x, y + i);
        std::cout << s[i];
    }
    setlocale(LC_ALL, "");
}

//write an horizontal 'string' at a designated position
void printfAt(const int x, const int y, const std::string &s) {
    setlocale(LC_ALL, "");
    gotoxy(x, y);
    std::cout << s;
    setlocale(LC_ALL, "");
}

//prevent loop when input doesn't match variable type
void PreventLoop() {
    std::cin.clear();
    std::string ignore;
    std::cin >> ignore;
    std::cout << std::endl << "Opção inválida." << std::endl;
}
