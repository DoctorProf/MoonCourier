#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <type_traits>

namespace utils
{
    constexpr float PI = 3.14159265358979323846f;

    inline int getIndexToCoord(int x, int y, int len_line)
    {
        return y * len_line + x;
    }

    inline std::pair<int, int> getCoordToIndex(int index, int len_line)
    {
        int y = index / len_line;
        int x = index % len_line;

        return { x, y };
    }

    inline bool checkBounds(int y, int x, int max_y, int max_x)
    {
        return (x >= 0 && x < max_x) &&
            (y >= 0 && y < max_y);
    }

    inline bool checkBounds(int index, int min, int max)
    {
        return index >= min && index < max;
    }

    template<typename T>
    bool parseNumber(const std::string& s, T& out)
    {
        try
        {
            if constexpr (std::is_same_v<T, int>)
            {
                out = std::stoi(s);
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                out = std::stof(s);
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                out = std::stod(s);
            }
            else
            {
                static_assert(sizeof(T) == 0, "Type not supported");
            }

            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    template<typename T>
    T writeData(std::string name_param, T default_value)
    {
        std::string buffer;

        std::cout
            << "Write "
            << name_param
            << " [" << default_value << "]: ";

        std::getline(std::cin, buffer);

        T value;

        value = parseNumber<T>(buffer, value)
            ? value
            : default_value;

        return value;
    }

    template<typename T>
    bool in(const T& value, const std::vector<T>& vec)
    {
        return std::find(vec.begin(), vec.end(), value) != vec.end();
    }


    template<typename T>
    T clamp(T value, T min, T max)
    {
        return std::clamp(value, min, max);
    }

    template<typename T>
    T lerp(T a, T b, float t)
    {
        return a + (b - a) * t;
    }

    inline float degToRad(float degrees)
    {
        return degrees * PI / 180.f;
    }

    inline float radToDeg(float radians)
    {
        return radians * 180.f / PI;
    }

    inline float normalizeAngle(float angle)
    {
        while (angle >= 360.f)
            angle -= 360.f;

        while (angle < 0.f)
            angle += 360.f;

        return angle;
    }
}