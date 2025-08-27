#ifndef WORD_SEARCH_WORD_HPP
#define WORD_SEARCH_WORD_HPP

#include "nlohmann/json.hpp"
#include "point.hpp"

namespace word_search
{
    using point_t = point<std::size_t>;

    enum class orientation : std::uint8_t
    {
        front,
        back,
        down,
        up,
        front_down,
        back_up,
        back_down,
        front_up
    };

    constexpr auto orientation_offsets() -> std::span<const point<int>>;

    class word final
    {
        std::string str_;
        point_t point_{};
        orientation orientation_{};
        bool found_{};

    public:
        constexpr word() noexcept = default;

        constexpr explicit word(std::string str, const point_t point, const orientation orientation) noexcept
            : str_(std::move(str)), point_(point), orientation_(orientation)
        {
        }

        [[nodiscard]] auto point() const -> const point_t& { return point_; }
        [[nodiscard]] auto str() const -> const std::string& { return str_; }
        [[nodiscard]] auto orientation() const -> orientation { return orientation_; }
        [[nodiscard]] auto found() const -> bool { return found_; }

        [[nodiscard]] auto length() const -> std::size_t { return str_.length(); }

        auto mark_as_found() -> void { found_ = true; }

        [[nodiscard]] auto is_found_at(const point_t& point) const -> bool;

        [[nodiscard]] auto begin() const -> std::string::const_iterator { return str_.begin(); }
        [[nodiscard]] auto end() const -> std::string::const_iterator { return str_.end(); }

        auto operator[](const std::size_t idx) const -> char { return str_[idx]; }

        constexpr static auto cleanup(const std::string& str) -> std::string;

        friend void to_json(nlohmann::json& j, const word& word)
        {
            j = nlohmann::json{
                {"string", word.str_},
                {"orientation", word.orientation_},
                {"initial_pt", word.point_},
                {"is_found", word.found_},
            };
        }

        friend void from_json(const nlohmann::json& j, word& word)
        {
            j.at("string").get_to(word.str_);
            j.at("orientation").get_to(word.orientation_);
            j.at("initial_pt").get_to(word.point_);
            j.at("is_found").get_to(word.found_);
        }
    };
}

constexpr auto word_search::orientation_offsets() -> std::span<const point<int>>
{
    static constexpr std::array orientation_offsets = {
        point{0, 0}, // none
        point{1, 0}, // front
        point{-1, 0}, // back
        point{0, 1}, // down
        point{0, -1}, // up
        point{1, 1}, // front_down
        point{-1, -1}, // back_up
        point{-1, 1}, // back_down
        point{1, -1} // front_up
    };
    return orientation_offsets;
}

constexpr auto word_search::word::cleanup(const std::string& str) -> std::string
{
    return str
        | std::views::filter([](const char ch) { return std::isalpha(ch, std::locale::classic()); })
        | std::views::transform([](const char ch) { return std::toupper(ch, std::locale::classic()); })
        | std::ranges::to<std::string>();
}

template <>
struct std::formatter<word_search::orientation> : std::formatter<std::underlying_type_t<word_search::orientation>>
{
    auto format(const word_search::orientation& orientation, std::format_context& ctx) const
    {
        return std::formatter<std::underlying_type_t<word_search::orientation>>::format(
            std::to_underlying(orientation), ctx);
    }
};

#endif //WORD_SEARCH_WORD_HPP
