#ifndef WORD_SEARCH_CONSOLE_HPP
#define WORD_SEARCH_CONSOLE_HPP

#include <ios>
#include <type_traits>

using ustreamsize = std::make_unsigned_t<std::streamsize>;

namespace console
{
    enum class ansi_color : std::uint16_t
    {
        black = 30,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white = 39,
    };

    enum class ansi_bg_color : std::uint16_t
    {
        black = 40,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white
    };

    auto clear() -> void;

    auto pause() -> void;

    /// Set the text color
    /// \param foreground The text color
    auto set_foreground_color(ansi_color foreground) -> void;

    /// Set both the text and background color
    /// \param foreground The text color
    /// \param background The background color
    auto set_color(ansi_color foreground, ansi_bg_color background) -> void;

    /// Reset the text color
    auto reset_color() -> void;

    /// Go to a specific location on the console
    /// \param x The x-coordinate
    /// \param y The y-coordinate
    auto gotoxy(ustreamsize x, ustreamsize y) -> void;

    /// Show a rectangle at a specific location
    /// \param x The x-coordinate
    /// \param y The y-coordinate
    /// \param width The width of the rectangle
    /// \param height The height of the rectangle
    auto show_rectangle_at(ustreamsize x, ustreamsize y, ustreamsize width, ustreamsize height) -> void;
}

#endif //WORD_SEARCH_CONSOLE_HPP
