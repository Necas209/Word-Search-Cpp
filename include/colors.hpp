#pragma once

#include <format>

enum class ansi_color : std::uint8_t
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

enum class ansi_bg_color : std::uint8_t
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

template <>
struct std::formatter<ansi_color> : formatter<int>
{
    template <class FormatContext>
    auto format(const ansi_color& f, FormatContext& ctx) const
    {
        return formatter<int>::format(std::to_underlying(f), ctx);
    }
};

template <>
struct std::formatter<ansi_bg_color> : formatter<int>
{
    template <class FormatContext>
    auto format(const ansi_bg_color& f, FormatContext& ctx) const
    {
        return formatter<int>::format(std::to_underlying(f), ctx);
    }
};
