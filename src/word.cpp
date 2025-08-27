#include "word.hpp"

auto word_search::word::is_found_at(const point_t& point) const -> bool
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
