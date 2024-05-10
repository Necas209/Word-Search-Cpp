#ifndef POINT_H
#define POINT_H

#include <format>
#include "lib/json.hpp"

using json = nlohmann::json;

typedef size_t point_t;

class point;

namespace std {
    template<>
    struct tuple_size<point> : std::integral_constant<point_t, 2> {
    };

    template<>
    struct tuple_element<0, point> {
        using type = point_t;
    };

    template<>
    struct tuple_element<1, point> {
        using type = point_t;
    };
}

class point final {
    point_t x_{};
    point_t y_{};

public:
    point() = default;

    point(const point_t x, const point_t y) : x_(x), y_(y) {
    }

    ~point() = default;

    [[nodiscard]] auto x() const { return x_; }

    [[nodiscard]] auto y() const { return y_; }

    [[nodiscard]] std::string to_string() const {
        return std::format("({}, {})", x_, y_);
    }

    bool operator==(const point &point) const { return x_ == point.x_ && y_ == point.y_; }

    bool operator!=(const point &point) const { return !(*this == point); }

    point &operator=(const point &point) = default;

    friend void to_json(json &j, const point &point) {
        j = json{
            {"x", point.x_},
            {"y", point.y_}
        };
    }

    friend void from_json(const json &j, point &point) {
        j.at("x").get_to(point.x_);
        j.at("y").get_to(point.y_);
    }

    template<size_t I>
    auto &&get() const && {
        if constexpr (I == 0) return x_;
        else if constexpr (I == 1) return y_;
    }
};

template<>
struct std::formatter<point> {
    template<typename FormatContext>
    auto format(const point &point, FormatContext &ctx) const {
        return format_to(ctx.out(), "({}, {})", point.x(), point.y());
    }
};

#endif //POINT_H
