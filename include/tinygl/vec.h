#ifndef TINYGL_VEC_H
#define TINYGL_VEC_H

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <initializer_list>

namespace tinygl
{
    template<std::size_t N, typename T = float>
    requires(N >= 2)
    class Vec
    {
    public:
        constexpr Vec() = default;
        constexpr Vec(std::initializer_list<T> values);

        constexpr T& operator[](std::size_t i) { return v[i]; }
        constexpr T operator[](std::size_t i) const { return v[i]; }

        constexpr T* data() noexcept { return v.data(); }
        constexpr const T* data() const noexcept { return v.data(); }

        std::array<T, N> v;
    };

    // Specializations for n = 2, 3, 4.
    // Defining a structure/union that contains other structures/unions without names is not allowed in C++.
    // But most compilers support this feature as extension and until C++ comes with a better method for type-punning...
    template<typename T>
    class Vec<2,T>
    {
    public:
        constexpr Vec() = default;
        constexpr Vec(T c1, T c2) : v{c1, c2} {}

        constexpr T& operator[](std::size_t i) { return v[i]; }
        constexpr T operator[](std::size_t i) const { return v[i]; }

        constexpr T* data() noexcept { return v.data(); }
        constexpr const T* data() const noexcept { return v.data(); }

        union
        {
            std::array<T,2> v;
            struct { T x, y; };
            struct { T r, g; };
            struct { T s, t; };
        };
    };

    template<typename T>
    class Vec<3,T>
    {
    public:
        constexpr Vec() = default;
        constexpr Vec(T c1, T c2, T c3) : v{c1, c2, c3} {}

        constexpr T& operator[](std::size_t i) { return v[i]; }
        constexpr T operator[](std::size_t i) const { return v[i]; }

        constexpr T* data() noexcept { return v.data(); }
        constexpr const T* data() const noexcept { return v.data(); }

        union
        {
            std::array<T,3> v;
            struct { T x, y, z; };
            struct { T r, g, b; };
            struct { T s, t, p; };
        };
    };

    template<typename T>
    class Vec<4,T>
    {
    public:
        constexpr Vec() = default;
        constexpr Vec(T c1, T c2, T c3, T c4) : v{c1, c2, c3, c4} {}

        constexpr T& operator[](std::size_t i) { return v[i]; }
        constexpr T operator[](std::size_t i) const { return v[i]; }

        constexpr T* data() noexcept { return v.data(); }
        constexpr const T* data() const noexcept { return v.data(); }

        union
        {
            std::array<T,4> v;
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };
            struct { T s, t, p, q; };
        };
    };

    using Vec2 = Vec<2, float>;
    using Vec3 = Vec<3, float>;
    using Vec4 = Vec<4, float>;

    // Element-wise binary operations
    template<std::size_t N, typename T = float>
    requires(N >= 2)
    constexpr inline Vec<N,T> operator+(Vec<N,T> vec1, Vec<N,T> vec2) noexcept
    {
        Vec<N,T> result;
        std::transform(vec1.v.begin(), vec1.v.end(), vec2.v.begin(), result.v.begin(), std::plus<T>());
        return result;
    }

    template<std::size_t N, typename T = float>
    requires(N >= 2)
    constexpr inline Vec<N,T> operator-(Vec<N,T> vec1, Vec<N,T> vec2) noexcept
    {
        Vec<N,T> result;
        std::transform(vec1.v.begin(), vec1.v.end(), vec2.v.begin(), result.v.begin(), std::minus<T>());
        return result;
    }

    template<std::size_t N, typename T = float>
    requires(N >= 2)
    constexpr inline Vec<N,T> operator*(float a, Vec<N,T> vec) noexcept
    {
        Vec<N,T> result;
        std::transform(vec.v.begin(), vec.v.end(), result.v.begin(), [a](auto& c) { return a * c; });
        return result;
    }

    template<std::size_t N, typename T = float>
    requires(N >= 2)
    constexpr inline Vec<N,T> operator*(Vec<N,T> vec, float a) noexcept
    {
        Vec<N,T> result;
        std::transform(vec.v.begin(), vec.v.end(), result.v.begin(), [a](auto& c) { return a * c; });
        return result;
    }

    template<std::size_t N, typename T = float>
    requires(N >= 2)
    constexpr inline Vec<N,T> operator*(Vec<N,T> vec1, Vec<N,T> vec2) noexcept
    {
        Vec<N,T> result;
        std::transform(vec1.v.begin(), vec1.v.end(), vec2.v.begin(), result.v.begin(), std::multiplies<T>());
        return result;
    }

    template<std::size_t N, typename T = float>
    requires(N >= 2)
    constexpr inline Vec<N,T> operator/(Vec<N,T> vec, float a) noexcept
    {
        Vec<N,T> result;
        std::transform(vec.v.begin(), vec.v.end(), result.v.begin(), [a](auto& c) { return c / a; });
        return result;
    }

    template<std::size_t N, typename T = float>
    requires(N >= 2)
    constexpr inline Vec<N,T> operator/(Vec<N,T> vec1, Vec<N,T> vec2) noexcept
    {
        Vec<N,T> result;
        std::transform(vec1.v.begin(), vec1.v.end(), vec2.v.begin(), result.v.begin(), std::divides<T>());
        return result;
    }

    // Unary minus (negation)
    template<std::size_t N, typename T = float>
    requires(N >= 2)
    constexpr inline Vec<N,T> operator-(Vec<N,T> vec) noexcept
    {
        Vec<N,T> result;
        std::transform(vec.v.begin(), vec.v.end(), result.v.begin(), std::negate<T>());
        return result;
    }
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr tinygl::Vec<N,T>::Vec(std::initializer_list<T> values)
{
    assert(values.size() == N);

    typename std::initializer_list<T>::iterator it = values.begin();
    for (std::size_t i = 0; i < N; ++i) {
        v[i] = *it++;
    }
}

#endif // TINYGL_VEC_H
