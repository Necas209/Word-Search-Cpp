#pragma once

#include <format>
#include "nlohmann/json.hpp"

template <typename T> requires std::is_integral_v<T>
class point;

namespace std
{
    template <typename T> requires std::is_integral_v<T>
    struct tuple_size<point<T>> : std::integral_constant<T, 2>
    {
    };

    template <typename T> requires std::is_integral_v<T>
    struct tuple_element<0, point<T>>
    {
        using type = T;
    };

    template <typename T> requires std::is_integral_v<T>
    struct tuple_element<1, point<T>>
    {
        using type = T;
    };
}

template <typename T> requires std::is_integral_v<T>
class point final
{
    T x;
    T y;

public:
    constexpr point() noexcept = default;

    constexpr explicit point(T x, T y) : x{x}, y{y}
    {
    }

    bool operator==(const point& point) const { return x == point.x && y == point.y; }

    bool operator!=(const point& point) const { return !(*this == point); }

    friend void to_json(nlohmann::json& j, const point& point)
    {
        j = nlohmann::json{
            {"x", point.x},
            {"y", point.y}
        };
    }

    friend void from_json(const nlohmann::json& j, point& point)
    {
        j.at("x").get_to(point.x);
        j.at("y").get_to(point.y);
    }

    template <std::size_t I>
    constexpr auto&& get() const &&
    {
        if (I == 0) return x;
        if (I == 1) return y;
    }
};

template <typename T> requires std::is_integral_v<T>
struct std::formatter<point<T>>
{
    template <typename FormatContext>
    auto format(const point<T>& point, FormatContext& ctx) const
    {
        return format_to(ctx.out(), "({}, {})", point.x, point.y);
    }
};
