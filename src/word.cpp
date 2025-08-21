#include "word.hpp"

#include <locale>

namespace rv = std::ranges::views;

auto word::is_found_at(const point_t& point) const -> bool
{
    if (!found_)
        return false;

    const auto [x, y] = point;
    const auto [word_x, word_y] = point_;
    const auto len = length();

    switch (orientation_)
    {
    case orientation::front:
        return x >= word_x && x < word_x + len && y == word_y;
    case orientation::back:
        return x <= word_x && x > word_x - len && y == word_y;
    case orientation::down:
        return y >= word_y && y < word_y + len && x == word_x;
    case orientation::up:
        return y <= word_y && y > word_y - len && x == word_x;
    case orientation::front_down:
        return x >= word_x && x < word_x + len && y >= word_y && y < word_y + len && x - word_x == y - word_y;
    case orientation::back_up:
        return x <= word_x && x > word_x - len && y <= word_y && y > word_y - len && word_x - x == word_y - y;
    case orientation::back_down:
        return x <= word_x && x > word_x - len && y >= word_y && y < word_y + len && word_x - x == y - word_y;
    case orientation::front_up:
        return x >= word_x && x < word_x + len && y <= word_y && y > word_y - len && x - word_x == y - word_y;
    default:
        return false;
    }
}

auto word::cleanup(const std::string& str) -> std::string
{
    return str
        | rv::filter([](const char ch) { return std::isalpha(ch, std::locale::classic()); })
        | rv::transform([](const char ch) { return std::toupper(ch, std::locale::classic()); })
        | std::ranges::to<std::string>();
}

auto orientation_offsets() -> const std::unordered_map<orientation, point<int>>&
{
    static const std::unordered_map<orientation, point<int>> orientation_offsets = {
        {orientation::none, point{0, 0}},
        {orientation::front, point{1, 0}}, // front
        {orientation::back, point{-1, 0}}, // back
        {orientation::down, point{0, 1}}, // down
        {orientation::up, point{0, -1}}, // up
        {orientation::front_down, point{1, 1}}, // front_down
        {orientation::back_up, point{-1, -1}}, // back_up
        {orientation::back_down, point{-1, 1}}, // back_down
        {orientation::front_up, point{1, -1}} // front_up
    };
    return orientation_offsets;
}

void to_json(nlohmann::json& j, const word& word)
{
    j = nlohmann::json{
        {"string", word.str_},
        {"orientation", word.orientation_},
        {"initial_pt", word.point_},
        {"is_found", word.found_},
    };
}

void from_json(const nlohmann::json& j, word& word)
{
    j.at("string").get_to(word.str_);
    j.at("orientation").get_to(word.orientation_);
    j.at("initial_pt").get_to(word.point_);
    j.at("is_found").get_to(word.found_);
}

template <typename FormatContext>
auto std::formatter<orientation>::format(const orientation& orientation, FormatContext& ctx) const
{
    return format_to(ctx.out(), "{}", static_cast<int>(orientation));
}
