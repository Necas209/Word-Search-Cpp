#ifndef WORD_SEARCH_POINT_HPP
#define WORD_SEARCH_POINT_HPP

#include <format>
#include "nlohmann/json.hpp"

namespace word_search
{
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
}

namespace std
{
    template <typename T> requires std::is_integral_v<T>
    struct tuple_size<word_search::point<T>> : std::integral_constant<T, 2>
    {
    };

    template <typename T> requires std::is_integral_v<T>
    struct tuple_element<0, word_search::point<T>>
    {
        using type = T;
    };

    template <typename T> requires std::is_integral_v<T>
    struct tuple_element<1, word_search::point<T>>
    {
        using type = T;
    };

    template <typename T> requires std::is_integral_v<T>
    struct formatter<word_search::point<T>>
    {
        auto format(const word_search::point<T>& point, std::format_context& ctx) const
        {
            return std::format_to(ctx.out(), "({}, {})", point.x, point.y);
        }
    };
}

#endif //WORD_SEARCH_POINT_HPP