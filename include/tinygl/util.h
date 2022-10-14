#ifndef TINYGL_UTIL_H
#define TINYGL_UTIL_H

#include <numbers>

namespace tinygl
{
    template<typename T>
    constexpr inline T degreesToRadians(T degrees)
    {
        return degrees * (std::numbers::pi_v<T> / 180);
    }

    template<typename T>
    constexpr inline T radiansToDegrees(T radians)
    {
        return radians * (180 / std::numbers::pi_v<T>);
    }
}

#endif // TINYGL_UTIL_H
