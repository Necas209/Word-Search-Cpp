#ifndef EXPERT_H

#define EXPERT_H

#include "Player.h"

class Expert final : public Player {
public:
    Expert() = default;

    void Final_Score(const chrono::duration<double> elapsed, const int num_words) override {
        const int max_time = num_words * 60;
        const double bonus = (max_time - elapsed.count()) / 10;
        score += static_cast<int>(bonus);
    }

    void Expert::Save(ofstream &os) override {
        os << "E: ";
        Player::Save(os);
    }
};

#endif // EXPERT_H
