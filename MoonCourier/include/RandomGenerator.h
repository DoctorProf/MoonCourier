#pragma once

#include <random>
#include <type_traits>

class RandomGenerator
{
public:
    template<typename T>
    static T get(T min, T max)
    {
        static_assert(
            std::is_arithmetic_v<T> && !std::is_same_v<T, bool>,
            "RandomGenerator::get supports numeric types only"
            );

        if constexpr (std::is_integral_v<T>)
        {
            std::uniform_int_distribution<T> distribution(min, max);
            return distribution(generator);
        }
        else
        {
            std::uniform_real_distribution<T> distribution(min, max);
            return distribution(generator);
        }
    }

private:
    RandomGenerator() = delete;

    inline static std::mt19937 generator{ std::random_device{}() };
};