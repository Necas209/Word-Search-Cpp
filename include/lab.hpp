#pragma once

#include <format>
#include <iostream>
#include <random>
#include <type_traits>

#include "colors.hpp"

template <typename T,
          typename iterator_type = decltype(std::begin(std::declval<T>())),
          typename = decltype(std::end(std::declval<T>()))>
constexpr auto enumerate(T&& iterable)
{
    struct iterator
    {
        std::size_t i;
        iterator_type iter;
        bool operator !=(const iterator& other) const { return iter != other.iter; }

        void operator ++()
        {
            ++i;
            ++iter;
        }

        auto operator *() const { return std::tie(i, *iter); }
    };

    struct iterable_wrapper
    {
        T iterable;
        auto begin() { return iterator{0, std::begin(iterable)}; }
        auto end() { return iterator{0, std::end(iterable)}; }
    };
    return iterable_wrapper{std::forward<T>(iterable)};
}

/// Generate a random number between lower and upper
/// \tparam T The type of the number
/// \param lower The lower bound
/// \param upper The upper bound (inclusive)
/// \return The random number
template <typename T = int>
T random_inclusive(const T lower, const T upper)
{
    static std::random_device rd; // get a random number from hardware
    static std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> dist(lower, upper); // define the range

    return dist(eng); // generate numbers
}

/// Generate a random number between 0 and upper
/// \tparam T The type of the number
/// \param upper The upper bound (exclusive)
/// \return The random number
template <typename T = int>
T random(const T upper)
{
    return random_inclusive<T>(0, upper - 1);
}

namespace console
{
    using ustreamsize = std::make_unsigned_t<std::streamsize>;

    void clear();

    void pause();

    /// Set the text color
    /// \param foreground The text color
    void set_foreground_color(ansi_color foreground);

    /// Set both the text and background color
    /// \param foreground The text color
    /// \param background The background color
    void set_color(ansi_color foreground, ansi_bg_color background);

    /// Reset the text color
    void reset_color();

    /// Go to a specific location on the console
    /// \param x The x-coordinate
    /// \param y The y-coordinate
    void gotoxy(ustreamsize x, ustreamsize y);

    /// Show a rectangle at a specific location
    /// \param x The x-coordinate
    /// \param y The y-coordinate
    /// \param width The width of the rectangle
    /// \param height The height of the rectangle
    void show_rectangle_at(ustreamsize x, ustreamsize y, ustreamsize width, ustreamsize height);
};
