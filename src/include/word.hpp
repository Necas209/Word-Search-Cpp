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

    auto orientation_names() -> std::span<const std::string_view>;

    auto orientation_offset(orientation orientation) -> std::pair<int, int>;

    auto cleanup(const std::string& str) -> std::string;

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

        [[nodiscard]] auto begin() const -> std::string::const_iterator { return str_.begin(); }
        [[nodiscard]] auto end() const -> std::string::const_iterator { return str_.end(); }

        auto operator[](const std::size_t idx) const -> char { return str_[idx]; }

        [[nodiscard]] auto operator==(const word& other) const -> bool
        {
            return str_ == other.str_ && point_ == other.point_ && orientation_ == other.orientation_;
        }

        [[nodiscard]] auto operator!=(const word& other) const -> bool { return !(*this == other); }

        friend void to_json(nlohmann::json& j, const word& word)
        {
            j = nlohmann::json{
                {"str", word.str_},
                {"orientation", word.orientation_},
                {"point", word.point_},
                {"found", word.found_},
            };
        }

        friend void from_json(const nlohmann::json& j, word& word)
        {
            j.at("str").get_to(word.str_);
            j.at("orientation").get_to(word.orientation_);
            j.at("point").get_to(word.point_);
            j.at("found").get_to(word.found_);
        }
    };
}

#endif //WORD_SEARCH_WORD_HPP
