#include "console.hpp"

#include <iostream>

auto console::clear() -> void
{
    std::print("\033[2J\033[H");
}

auto console::pause() -> void
{
    std::print("Press any key to continue...");
    std::cin.get();
}

auto console::set_foreground_color(const ansi_color foreground) -> void
{
    std::print("\033[{}m", std::to_underlying(foreground));
}

auto console::set_color(const ansi_color foreground, const ansi_bg_color background) -> void
{
    std::print("\033[{};{}m", std::to_underlying(foreground), std::to_underlying(background));
}

auto console::reset_color() -> void
{
    std::print("\033[0m");
}

auto console::gotoxy(const ustreamsize x, const ustreamsize y) -> void
{
    std::print("\033[{};{}H", y, x);
}

auto console::show_rectangle_at(const ustreamsize x, const ustreamsize y, const ustreamsize width,
                                const ustreamsize height) -> void
{
    if (width < 2 || height < 2)
        return;

    // Move the cursor to the starting position
    gotoxy(x, y);

    const auto inner_w = width - 2;
    const std::wstring hbar(inner_w, L'═');
    const std::wstring space(inner_w, L' ');

    // Top border
    std::wcout << std::format(L"╔{}╗\n", hbar);

    // Middle part
    for (ustreamsize i = 0; i < height - 2; i++)
    {
        gotoxy(x, y + i + 1);
        std::wcout << std::format(L"║{}║\n", space);
    }

    // Bottom border
    gotoxy(x, y + height - 1);
    std::wcout << std::format(L"╚{}╝\n", hbar);
}
