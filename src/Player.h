#ifndef PLAYER_H

#define PLAYER_H

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

class Player {
protected:
    std::string name;
    int age = 0;
    int score = 0;

public:
    Player() = default;

    virtual ~Player() = default;

    void Ask_name() {
        std::cout << std::endl << " Insira o seu nome: ";
        std::cin.ignore();
        getline(std::cin, name);
    }

    void Ask_age() {
        int age;
        std::cout << std::endl << " Insira a sua idade: ";
        std::cin >> age;
        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            Ask_age();
        } else {
            this->age = age;
        }
    }

    void Update_Score(const bool flag) {
        if (!flag and score >= 5) {
            score -= 5;
        } else if (flag) {
            score += 10;
        }
    }

    virtual void Final_Score(std::chrono::duration<double> elapsed, int num_words) = 0;

    void Show() const {
        std::cout << *this;
    }

    virtual void Read(std::ifstream &is) {
        std::string s;
        is >> age >> score;
        is.ignore();
        getline(is, name, ';');
    }

    virtual void Save(std::ofstream &os) const {
        os << age << ' ' << score << ' ' << name << ';';
    }

    friend std::ostream &operator<<(std::ostream &os, const Player &player) {
        os << std::endl << " Nome: " << player.name << std::endl;
        os << std::endl << " Idade: " << player.age << std::endl;
        os << std::endl << " Pontuação: " << player.score << std::endl;
        return os;
    }
};

#endif // PLAYER_H
