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

        constexpr Vec(T c1, T c2) requires(N == 2) : v{c1, c2} {}
        constexpr Vec(T c1, T c2, T c3) requires(N == 3) : v{c1, c2, c3} {}
        constexpr Vec(T c1, T c2, T c3, T c4) requires(N == 4) : v{c1, c2, c3, c4} {}

        constexpr T& operator[](std::size_t i) { return v[i]; }
        constexpr T operator[](std::size_t i) const { return v[i]; }

        // x, y, z, w components
        constexpr T& x() { return v[0]; }
        constexpr T x() const { return v[0]; }

        constexpr T& y() { return v[1]; }
        constexpr T y() const { return v[1]; }

        constexpr T& z() requires(N >= 3) { return v[2]; }
        constexpr T z() const requires(N >= 3) { return v[2]; }

        constexpr T& w() requires(N >= 4) { return v[3]; }
        constexpr T w() const requires(N >= 4) { return v[3]; }

        // r, g, b, a components
        constexpr T& r() { return v[0]; }
        constexpr T r() const { return v[0]; }

        constexpr T& g() { return v[1]; }
        constexpr T g() const { return v[1]; }

        constexpr T& b() requires(N >= 3) { return v[2]; }
        constexpr T b() const requires(N >= 3) { return v[2]; }

        constexpr T& a() requires(N >= 4) { return v[3]; }
        constexpr T a() const requires(N >= 4) { return v[3]; }

        constexpr T* data() noexcept { return v.data(); }
        constexpr const T* data() const noexcept { return v.data(); }

        // Element-wise binary operations
        constexpr friend inline Vec operator+(Vec vec1, Vec vec2) noexcept
        {
            Vec result;
            std::transform(vec1.v.begin(), vec1.v.end(), vec2.v.begin(), result.v.begin(), std::plus<T>());
            return result;
        }

        constexpr friend inline Vec operator-(Vec vec1, Vec vec2) noexcept
        {
            Vec result;
            std::transform(vec1.v.begin(), vec1.v.end(), vec2.v.begin(), result.v.begin(), std::minus<T>());
            return result;
        }

        constexpr friend inline Vec operator*(float a, Vec vec) noexcept
        {
            Vec result;
            std::transform(vec.v.begin(), vec.v.end(), result.v.begin(), [a](auto& c) { return a * c; });
            return result;
        }

        constexpr friend inline Vec operator*(Vec vec, float a) noexcept
        {
            Vec result;
            std::transform(vec.v.begin(), vec.v.end(), result.v.begin(), [a](auto& c) { return a * c; });
            return result;
        }

        constexpr friend inline Vec operator*(Vec vec1, Vec vec2) noexcept
        {
            Vec result;
            std::transform(vec1.v.begin(), vec1.v.end(), vec2.v.begin(), result.v.begin(), std::multiplies<T>());
            return result;
        }

        constexpr friend inline Vec operator/(Vec vec, float a) noexcept
        {
            Vec result;
            std::transform(vec.v.begin(), vec.v.end(), result.v.begin(), [a](auto& c) { return c / a; });
            return result;
        }

        constexpr friend inline Vec operator/(Vec vec1, Vec vec2) noexcept
        {
            Vec result;
            std::transform(vec1.v.begin(), vec1.v.end(), vec2.v.begin(), result.v.begin(), std::divides<T>());
            return result;
        }

        // Unary minus (negation)
        constexpr friend inline Vec operator-(Vec vec) noexcept
        {
            Vec result;
            std::transform(vec.v.begin(), vec.v.end(), result.v.begin(), std::negate<T>());
            return result;
        }
    private:
        std::array<T, N> v;
    };

    using Vec2 = Vec<2, float>;
    using Vec3 = Vec<3, float>;
    using Vec4 = Vec<4, float>;
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
