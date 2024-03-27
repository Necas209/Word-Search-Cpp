#ifndef BEGINNER_H

#define BEGINNER_H

#include "Player.h"

class Beginner final : public Player {
public:
    Beginner() = default;

    void Final_Score(chrono::duration<double> elapsed, int num_words) override {
    }

    void Save(ofstream &os) override {
        os << "B: ";
        Player::Save(os);
    }
};

#endif // BEGINNER_H
