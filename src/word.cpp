#include "word.hpp"

#include <ranges>
#include <span>
#include <utility>

auto word_search::orientation_names() -> std::span<const std::string_view>
{
    static constexpr std::array<std::string_view, 8> orientation_names = {
        "Front",
        "Back",
        "Down",
        "Up",
        "Front/Down",
        "Back/Up",
        "Back/Down",
        "Front/Up"
    };
    return orientation_names;
}

auto word_search::orientation_offset(const orientation_t orientation) -> std::pair<int, int>
{
    static constexpr std::array orientation_offsets = {
        std::pair{1, 0}, // front
        std::pair{-1, 0}, // back
        std::pair{0, 1}, // down
        std::pair{0, -1}, // up
        std::pair{1, 1}, // front_down
        std::pair{-1, -1}, // back_up
        std::pair{-1, 1}, // back_down
        std::pair{1, -1} // front_up
    };
    return orientation_offsets[std::to_underlying(orientation)];
}

auto word_search::cleanup(const std::string& str) -> std::string
{
    auto cleaned = str
        | std::views::filter([](const char ch) { return std::isalpha(ch, std::locale::classic()); })
        | std::views::transform([](const char ch) { return std::toupper(ch, std::locale::classic()); });
    return std::ranges::to<std::string>(cleaned);
}
