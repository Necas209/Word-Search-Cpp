#ifndef WORD_SEARCH_POINT_HPP
#define WORD_SEARCH_POINT_HPP

#include <format>
#include <tuple>
#include <type_traits>
#include "nlohmann/json.hpp"

namespace word_search
{
    template <std::integral T>
    class point final
    {
        T x_;
        T y_;

    public:
        constexpr point() noexcept = default;

        constexpr explicit point(T x, T y) : x_{x}, y_{y}
        {
        }

        [[nodiscard]] constexpr auto x() const noexcept -> T { return x_; }

        [[nodiscard]] constexpr auto y() const noexcept -> T { return y_; }

        constexpr auto operator==(const point& other) const -> bool { return x_ == other.x_ && y_ == other.y_; }

        constexpr auto operator!=(const point& other) const -> bool { return !(*this == other); }

        friend void to_json(nlohmann::json& j, const point& point)
        {
            j = nlohmann::json{
                {"x", point.x_},
                {"y", point.y_}
            };
        }

        friend void from_json(const nlohmann::json& j, point& point)
        {
            j.at("x").get_to(point.x_);
            j.at("y").get_to(point.y_);
        }

        template <std::size_t I>
        constexpr auto&& get() const &&
        {
            if constexpr (I == 0) return x_;
            if constexpr (I == 1) return y_;
        }
    };
}

namespace std
{
    template <std::integral T>
    struct tuple_size<word_search::point<T>> : std::integral_constant<T, 2>
    {
    };

    template <std::integral T>
    struct tuple_element<0, word_search::point<T>>
    {
        using type = T;
    };

    template <std::integral T>
    struct tuple_element<1, word_search::point<T>>
    {
        using type = T;
    };

    template <std::integral T>
    struct formatter<word_search::point<T>>
    {
        auto format(const word_search::point<T>& point, std::format_context& ctx) const
        {
            return std::format_to(ctx.out(), "({}, {})", point.x_, point.y_);
        }
    };
}

#endif //WORD_SEARCH_POINT_HPP
