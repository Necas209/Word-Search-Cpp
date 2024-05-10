#ifndef COLORS_H
#define COLORS_H

#include <cstdint>
#include <format>

typedef uint8_t ansi_color_t;
typedef uint8_t ansi_bg_color_t;

enum class ansi_color : ansi_color_t {
    black = 30,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white = 39,
};

enum class ansi_bg_color : ansi_bg_color_t {
    black = 40,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white
};

template<>
struct std::formatter<ansi_color> : formatter<int> {
    template<class FormatContext>
    auto format(const ansi_color &f, FormatContext &ctx) const {
        return formatter<int>::format(std::to_underlying(f), ctx);
    }
};

template<>
struct std::formatter<ansi_bg_color> : formatter<int> {
    template<class FormatContext>
    auto format(const ansi_bg_color &f, FormatContext &ctx) const {
        return formatter<int>::format(std::to_underlying(f), ctx);
    }
};

#endif //COLORS_H
