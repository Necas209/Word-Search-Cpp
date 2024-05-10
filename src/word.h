#ifndef WORD_H
#define WORD_H

#include <fstream>

#include "point.h"
#include "lib/json.hpp"

using json = nlohmann::json;

typedef uint8_t state_t;

enum class orientation {
    none,
    front,
    back,
    down,
    up,
    front_down,
    back_up,
    back_down,
    front_up
};

class word final {
    std::string str_{};
    point point_{};
    orientation orientation_{orientation::none};
    bool found_{false};

public:
    word() = default;

    word(std::string str, const point &point, const orientation orientation)
        : str_(std::move(str)), point_(point), orientation_(orientation) {
    }

    ~word() = default;

    [[nodiscard]] auto str() const { return str_; }

    [[nodiscard]] auto orientation() const { return orientation_; }

    [[nodiscard]] auto point() const { return point_; }

    [[nodiscard]] auto found() const { return found_; }

    [[nodiscard]] auto length() const { return str_.length(); }

    auto mark_as_found() { found_ = true; }

    void orientation(const ::orientation &orientation) { orientation_ = orientation; }

    [[nodiscard]] bool is_found_at(const size_t x, const size_t y) const {
        if (!found_)
            return false;

        const auto [x_, y_] = point_;
        const auto len = str_.length();
        switch (orientation_) {
            case orientation::front:
                return x >= x_ && x < x_ + len && y == y_;
            case orientation::back:
                return x <= x_ && x > x_ - len && y == y_;
            case orientation::down:
                return y >= y_ && y < y_ + len && x == x_;
            case orientation::up:
                return y <= y_ && y > y_ - len && x == x_;
            case orientation::front_down:
                return x >= x_ && x < x_ + len && y >= y_ && y < y_ + len && x - x_ == y - y_;
            case orientation::back_up:
                return x <= x_ && x > x_ - len && y <= y_ && y > y_ - len && x_ - x == y_ - y;
            case orientation::back_down:
                return x <= x_ && x > x_ - len && y >= y_ && y < y_ + len && x_ - x == y - y_;
            case orientation::front_up:
                return x >= x_ && x < x_ + len && y <= y_ && y > y_ - len && x - x_ == y - y_;
            default:
                return false;
        }
    }

    [[nodiscard]] auto begin() const { return str_.begin(); }

    [[nodiscard]] auto end() const { return str_.end(); }

    const char &operator[](const size_t i) const { return str_[i]; }

    bool operator==(const word &word) const { return str_ == word.str() && point_ == word.point_; }

    bool operator!=(const word &word) const { return *this == word; }

    friend void to_json(json &j, const word &word) {
        j = json{
            {"string", word.str_},
            {"orientation", word.orientation_},
            {"initial_pt", word.point_},
            {"found", word.found_},
        };
    }

    friend void from_json(const json &j, word &word) {
        j.at("string").get_to(word.str_);
        j.at("orientation").get_to(word.orientation_);
        j.at("initial_pt").get_to(word.point_);
        j.at("found").get_to(word.found_);
    }
};

template<>
struct std::formatter<orientation> {
    template<typename FormatContext>
    auto format(const orientation &orientation, FormatContext &ctx) const {
        return format_to(ctx.out(), "{}", static_cast<int>(orientation));
    }
};

#endif // !WORD_H
