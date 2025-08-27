#ifndef WORD_SEARCH_RND_HPP
#define WORD_SEARCH_RND_HPP

#include <random>

namespace rnd
{
    /// Generate a random number between lower and upper
    /// \tparam T The type of the number
    /// \param lower The lower bound
    /// \param upper The upper bound (inclusive)
    /// \return The random number

    template <std::integral T>
    T next(const T lower, const T upper)
    {
        static std::random_device rd; // get a random number from hardware
        static std::mt19937 eng(rd()); // seed the generator
        std::uniform_int_distribution<T> dist(lower, upper); // define the range

        return dist(eng); // generate numbers
    }

    /// Generate a random number between 0 and upper
    /// \tparam T The type of the number
    /// \param upper The upper bound (exclusive)
    /// \return The random number
    template <std::integral T>
    T next(const T upper)
    {
        return next<T>(0, upper - 1);
    }
}

#endif //WORD_SEARCH_RND_HPP
