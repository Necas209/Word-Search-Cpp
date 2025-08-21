#pragma once

#include "nlohmann/json.hpp"
#include "point.hpp"

enum class orientation : std::uint8_t
{
    none,
    front,
    back,
    down,
    up,
    front_down,
    back_up,
    back_down,
    front_up
};

using point_t = point<std::size_t>;

class word final
{
    std::string str_;
    orientation orientation_{};
    point_t point_{};
    bool found_{};

public:
    constexpr word() noexcept = default;

    constexpr explicit word(std::string str, orientation orientation, point_t point) noexcept
        : str_(std::move(str)), orientation_(orientation), point_(point)
    {
    }

    [[nodiscard]] auto point() const -> const point_t& { return point_; }
    [[nodiscard]] auto str() const -> const std::string& { return str_; }
    [[nodiscard]] auto orientation() const -> orientation { return orientation_; }
    [[nodiscard]] auto found() const -> bool { return found_; }

    [[nodiscard]] auto length() const -> std::size_t { return str_.length(); }
    [[nodiscard]] auto is_found_at(const point_t& point) const -> bool;
    void mark_as_found() { found_ = true; }

    [[nodiscard]] auto begin() const -> std::string::const_iterator { return str_.begin(); }
    [[nodiscard]] auto end() const -> std::string::const_iterator { return str_.end(); }

    auto operator[](const size_t idx) const -> char { return str_[idx]; }

    static auto cleanup(const std::string& str) -> std::string;

    friend void to_json(nlohmann::json& j, const word& word);
    friend void from_json(const nlohmann::json& j, word& word);
};

auto orientation_offsets() -> const std::unordered_map<orientation, point<int>>&;

template <>
struct std::formatter<orientation>
{
    template <typename FormatContext>
    auto format(const orientation& orientation, FormatContext& ctx) const;
};
