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

        inline constexpr T& operator[](std::size_t i);
        inline constexpr T operator[](std::size_t i) const;

        inline constexpr T& x();
        inline constexpr T x() const;

        inline constexpr T& y();
        inline constexpr T y() const;

        inline constexpr T& z() requires(N >= 3);
        inline constexpr T z() const requires(N >= 3);

        inline constexpr T& w() requires(N >= 4);
        inline constexpr T w() const requires(N >= 4);

        inline constexpr T& r();
        inline constexpr T r() const;

        inline constexpr T& g();
        inline constexpr T g() const;

        inline constexpr T& b() requires(N >= 3);
        inline constexpr T b() const requires(N >= 3);

        inline constexpr T& a() requires(N >= 4);
        inline constexpr T a() const requires(N >= 4);

        inline constexpr T* data() noexcept;
        inline constexpr const T* data() const noexcept;

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

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T& tinygl::Vec<N,T>::operator[](std::size_t i)
{
    return v[i];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T tinygl::Vec<N,T>::operator[](std::size_t i) const
{
    return v[i];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T& tinygl::Vec<N, T>::x()
{
    return v[0];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T tinygl::Vec<N, T>::x() const
{
    return v[0];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T& tinygl::Vec<N, T>::y()
{
    return v[1];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T tinygl::Vec<N, T>::y() const
{
    return v[1];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T& tinygl::Vec<N, T>::z() requires(N >= 3)
{
    return v[2];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T tinygl::Vec<N, T>::z() const requires(N >= 3)
{
    return v[2];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T& tinygl::Vec<N, T>::w() requires(N >= 4)
{
    return v[3];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T tinygl::Vec<N, T>::w() const requires(N >= 4)
{
    return v[3];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T& tinygl::Vec<N, T>::r()
{
    return v[0];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T tinygl::Vec<N, T>::r() const
{
    return v[0];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T& tinygl::Vec<N, T>::g()
{
    return v[1];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T tinygl::Vec<N, T>::g() const
{
    return v[1];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T& tinygl::Vec<N, T>::b() requires(N >= 3)
{
    return v[2];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T tinygl::Vec<N, T>::b() const requires(N >= 3)
{
    return v[2];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T& tinygl::Vec<N, T>::a() requires(N >= 4)
{
    return v[3];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T tinygl::Vec<N, T>::a() const requires(N >= 4)
{
    return v[3];
}

template<std::size_t N, typename T>
requires (N >= 2)
constexpr T* tinygl::Vec<N, T>::data() noexcept
{
    return v.data();
}

template<std::size_t N, typename T>
requires (N >= 2)
constexpr const T* tinygl::Vec<N, T>::data() const noexcept
{
    return v.data();
}

#endif // TINYGL_VEC_H
