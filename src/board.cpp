#include "board.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

#include "rnd.hpp"
#include "ui.hpp"
#include "ftxui/component/component.hpp"

namespace rv = std::ranges::views;

auto word_search::get_orientations(const difficulty_t difficulty) -> std::span<const orientation>
{
    static constexpr std::array easy{
        orientation::front,
        orientation::down
    };
    static constexpr std::array medium{
        orientation::front,
        orientation::down,
        orientation::front_down,
        orientation::front_up
    };
    static constexpr std::array hard{
        orientation::front,
        orientation::down,
        orientation::front_down,
        orientation::front_up,
        orientation::back,
        orientation::up,
        orientation::back_down,
        orientation::back_up
    };

    switch (difficulty)
    {
    case difficulty_t::easy:
        return easy;
    case difficulty_t::medium:
        return medium;
    case difficulty_t::hard:
        return hard;
    }
    throw std::invalid_argument("Invalid difficulty");
}

auto word_search::board::solved() const -> bool
{
    return std::ranges::all_of(words_, [](const word& word) { return word.found(); });
}

auto word_search::board::words_found() const -> std::int64_t
{
    return std::ranges::count_if(words_, [](const word& word) { return word.found(); });
}

auto word_search::board::find_word(const word& search_word) -> word_state
{
    const auto it = std::ranges::find_if(words_, [&search_word](const word& w) { return w == search_word; });
    if (it == words_.end())
        return word_state::not_found;

    if (it->found())
        return word_state::already_found;

    it->mark_as_found();
    return word_state::found;
}

auto word_search::board::load_words(const std::vector<std::string>& words) -> void
{
    const auto orientations = get_orientations(difficulty_);

    for (const auto& str : words | rv::transform(cleanup))
    {
        if (const auto l = str.length(); l > width_ || l > height_)
            continue;

        auto i = 0;
        while (++i < 20)
        {
            const auto orientation = orientations[rnd::next(orientations.size())];
            const auto point = random_pt(str, orientation);

            if (auto word = word_search::word{str, point, orientation}; word_fits(word))
            {
                add_word(word);
                break;
            }
        }
    }

    static auto isspace = [](const char ch) { return std::isspace(ch, std::locale::classic()); };
    for (auto& letter : letters_ | rv::filter(isspace))
    {
        letter = rnd::next<std::int8_t>('A', 'Z');
    }
}

auto word_search::board::random_pt(const std::string& word, const orientation orientation) const -> point_t
{
    const auto len = word.length();
    const auto [dx, dy] = orientation_offset(orientation);

    // Compute inclusive ranges for the starting coordinate based on direction sign.
    auto range_from = [len](const std::size_t size, const int d) -> std::pair<std::size_t, std::size_t>
    {
        switch (d)
        {
        case 0: return {0ul, size - 1}; // any column/row
        case 1: return {0ul, size - len}; // forward
        case -1: return {len - 1, size - 1}; // backward
        default: std::unreachable();
        }
    };

    const auto [x_lo, x_hi] = range_from(width_, dx);
    const auto [y_lo, y_hi] = range_from(height_, dy);

    const auto x = rnd::next(x_lo, x_hi);
    const auto y = rnd::next(y_lo, y_hi);
    return point_t{x, y};
}

auto word_search::board::word_fits(const word& word) const -> bool
{
    const auto [x, y] = word.point();
    const auto [dx, dy] = orientation_offset(word.orientation());

    for (auto i = 0ul; i < word.length(); ++i)
    {
        const auto nx = x + i * dx;
        const auto ny = y + i * dy;
        if (const char cell = (*this)[nx, ny]; cell != ' ' && cell != word[i])
            return false;
    }
    return true;
}

auto word_search::board::add_word(const word& word) -> void
{
    words_.emplace_back(word);
    const auto [x, y] = word.point();
    const auto [dx, dy] = orientation_offset(word.orientation());

    for (auto i = 0ul; i < word.length(); ++i)
    {
        const auto nx = x + i * dx;
        const auto ny = y + i * dy;
        (*this)[nx, ny] = word[i];
    }
}

auto word_search::board::new_board(ftxui::ScreenInteractive& screen) -> board
{
    using namespace ftxui;

    // UI state
    const std::vector<std::string> diff_labels = {"Easy", "Medium", "Hard"};
    int diff_index = 0; // 0: Easy, 1: Medium, 2: Hard
    std::string width_str, height_str;
    std::string error_msg;

    // Result holders
    auto width = 0ul, height = 0ul;
    auto difficulty = difficulty_t::easy;
    bool accepted = false;

    // Widgets
    auto rb_diff = Radiobox(&diff_labels, &diff_index);
    auto in_width = Input(&width_str, "width (e.g., 10)");
    auto in_height = Input(&height_str, "height (e.g., 10)");

    auto check_dimension = [&](const std::size_t dimension)
    {
        switch (difficulty)
        {
        case difficulty_t::easy:
            return dimension >= 10;
        case difficulty_t::medium:
            return dimension >= 15;
        case difficulty_t::hard:
            return dimension >= 20;
        }
        throw std::invalid_argument("Invalid difficulty");
    };

    auto btn_ok = Button("OK", [&]
    {
        difficulty = static_cast<word_search::difficulty_t>(diff_index + 1);
        try
        {
            width = std::stoul(width_str);
            height = std::stoul(height_str);
        }
        catch (...)
        {
            error_msg = "Width and height must be positive integers.";
            return;
        }
        if (!check_dimension(width))
        {
            error_msg = "Invalid width for the selected difficulty.";
            return;
        }
        if (!check_dimension(height))
        {
            error_msg = "Invalid height for the selected difficulty.";
            return;
        }

        accepted = true;
        screen.Exit();
    });

    auto btn_cancel = Button("Cancel", [&]
    {
        throw std::runtime_error("Board creation cancelled by user.");
    });

    const auto container = Container::Vertical({
        rb_diff,
        in_width,
        in_height,
        Container::Horizontal({btn_ok, btn_cancel})
    });

    constexpr std::array hints = {
        "The board size should be at least 10 x 10 for Easy.",
        "The board size should be at least 15 x 15 for Medium.",
        "The board size should be at least 20 x 20 for Hard.",
    };

    const auto view = Renderer(container, [&]
    {
        return vbox({
            text("Create Board") | bold | center,
            separator(),
            hbox(text("Difficulty: ") | bold, rb_diff->Render()) | center,
            separator(),
            text(hints[diff_index]) | dim | center, // only dynamic part (plus error below)
            separator(),
            hbox(text("Width:  ") | bold, in_width->Render()) | center,
            hbox(text("Height: ") | bold, in_height->Render()) | center,
            (error_msg.empty() ? filler() : (separator(), text(error_msg) | color(Color::Red) | center)),
            separator(),
            hbox({
                filler(),
                btn_ok->Render() | bold,
                text("  "),
                btn_cancel->Render(),
                filler(),
            }),
        }) | border | flex | center;
    });
    screen.Loop(view);

    if (!accepted)
    {
        throw std::runtime_error("Board creation aborted.");
    }

    return board{width, height, difficulty};
}

struct line : std::string
{
    using std::string::string;

    friend std::istream& operator>>(std::istream& s, line& l)
    {
        std::getline(s, l);
        return s;
    }
};

auto word_search::board::pick_theme(ftxui::ScreenInteractive& screen) -> void
{
    const std::vector<std::string> entries = {
        "Portugal",
        "United States of America",
    };
    const auto choice = ui::select(screen, "Pick the theme", entries);
    std::filesystem::path filename;
    switch (choice)
    {
    case 0:
        filename = "data/portugal.txt";
        break;
    case 1:
        filename = "data/usa.txt";
        break;
    default:
        throw std::runtime_error("No theme selected.");
    }

    std::ifstream file(filename);
    if (!file)
    {
        throw std::filesystem::filesystem_error{
            "Could not read file.",
            filename,
            std::make_error_code(std::errc::io_error)
        };
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    load_words(lines);
}

auto word_search::board::letters_panel() const -> ftxui::Table
{
    using namespace ftxui;
    namespace rv = std::ranges::views;

    auto cell = [](const std::string& s) { return text(s) | center | size(WIDTH, EQUAL, 2); };
    auto letter_cell = [&](const char ch) { return cell(std::string(1, ch)); };
    auto coord_cell = [&](const std::size_t dimension) { return cell(std::to_string(dimension)) | dim; };

    auto header = rv::iota(0ul, width_)
        | rv::transform(coord_cell)
        | std::ranges::to<Elements>();
    header.insert(header.begin(), text("Y\\X")); // Add empty column for index.

    auto rows = std::vector<Elements>{std::move(header)};

    for (const auto row : rv::iota(0ul, height_))
    {
        auto row_cells = rv::iota(0ul, width_)
            | rv::transform([this, row](const auto col) { return (*this)[col, row]; })
            | rv::transform(letter_cell)
            | std::ranges::to<Elements>();

        // Add row index cell
        row_cells.insert(row_cells.begin(), coord_cell(row));

        rows.push_back(row_cells);
    }

    for (auto& word : words_ | rv::filter([](const auto& w) { return w.found(); }))
    {
        const auto [dx, dy] = orientation_offset(word.orientation());
        const auto [x, y] = word.point();

        for (const auto i : rv::iota(0ul, word.length()))
        {
            const auto nx = x + i * dx;
            const auto ny = y + i * dy;
            rows[ny + 1][nx + 1] |= color(Color::Yellow);
        }
    }

    auto table = Table(rows);
    auto cells = table.SelectAll();
    cells.Separator(LIGHT);
    cells.Border(LIGHT);

    auto x_row = table.SelectRow(0);
    x_row.SeparatorHorizontal(LIGHT);
    x_row.Border(DOUBLE);

    auto y_col = table.SelectColumn(0);
    y_col.SeparatorVertical(LIGHT);
    y_col.Border(DOUBLE);

    return table;
}
