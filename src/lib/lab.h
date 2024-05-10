#ifndef LAB_H
#define LAB_H

#include <format>
#include <iostream>
#include <random>

#include "colors.h"

template<typename T,
    typename TIter = decltype(std::begin(std::declval<T>())),
    typename = decltype(std::end(std::declval<T>()))>
constexpr auto enumerate(T &&iterable) {
    struct iterator {
        size_t i;
        TIter iter;
        bool operator !=(const iterator &other) const { return iter != other.iter; }

        void operator ++() {
            ++i;
            ++iter;
        }

        auto operator *() const { return std::tie(i, *iter); }
    };
    struct iterable_wrapper {
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
template<typename T = int>
T random_inclusive(const T lower, const T upper) {
    static std::random_device rd; // obtain a random number from hardware
    static std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> dist(lower, upper); // define the range

    return dist(eng); // generate numbers
}

/// Generate a random number between 0 and upper
/// \tparam T The type of the number
/// \param upper The upper bound (exclusive)
/// \return The random number
template<typename T = int>
T random(const T upper) {
    return random_inclusive<T>(0, upper - 1);
}

namespace string {
    /// Convert a string to uppercase
    /// \param str The string to convert
    /// \return The uppercase string
    inline std::string upper(const std::string &str) {
        std::string new_str = str;
        std::ranges::transform(new_str, new_str.begin(), toupper);
        return new_str;
    }

    inline std::string strip(const std::string &str) {
        std::string new_str = str;
        std::erase_if(new_str, isspace);
        return new_str;
    }
}

class console {
public:
    static void clear();

    static void pause();

    /// Set the text color
    /// \param foreground The text color
    static void set_foreground_color(ansi_color foreground);

    /// Set both the text and background color
    /// \param foreground The text color
    /// \param background The background color
    static void set_color(ansi_color foreground, ansi_bg_color background);

    /// Reset the text color
    static void reset_color();

    /// Go to a specific location on the console
    /// \param x The x-coordinate
    /// \param y The y-coordinate
    static void gotoxy(uint32_t x, uint32_t y);

    /// Show a rectangle at a specific location
    /// \param x The x-coordinate
    /// \param y The y-coordinate
    /// \param width The width of the rectangle
    /// \param height The height of the rectangle
    static void show_rectangle_at(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    /// Sleep for a specific number of milliseconds
    /// \param milliseconds The number of milliseconds to sleep
    static void msleep(uint32_t milliseconds);
};

#endif // LAB_H
