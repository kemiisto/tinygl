#ifndef TINYGL_VEC_H
#define TINYGL_VEC_H

#include <array>
#include <cassert>
#include <initializer_list>

namespace tinygl
{
    template<std::size_t N, typename T = float>
    requires(N >= 2 && N <= 4)
    class Vec {
    public:
        Vec(std::initializer_list<T> values);

        constexpr T& operator[](std::size_t i);
        constexpr T operator[](std::size_t i) const;

        constexpr T x() const;
        constexpr T y() const;
        constexpr T z() const requires(N >= 3);
        constexpr T w() const requires(N >= 4);

        constexpr T* data() noexcept;
        constexpr const T* data() const noexcept;
    private:
        std::array<T, N> v;
    };

    using Vec3 = Vec<3, float>;
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
tinygl::Vec<N,T>::Vec(std::initializer_list<T> values)
{
    assert(values.size() == N);

    typename std::initializer_list<T>::iterator it = values.begin();
    for (std::size_t i = 0; i < N; ++i) {
        v[i] = *it++;
    }
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
constexpr T& tinygl::Vec<N,T>::operator[](std::size_t i)
{
    return v[i];
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
constexpr T tinygl::Vec<N,T>::operator[](std::size_t i) const
{
    return v[i];
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
constexpr T tinygl::Vec<N, T>::x() const
{
    return v[0];
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
constexpr T tinygl::Vec<N, T>::y() const
{
    return v[1];
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
constexpr T tinygl::Vec<N, T>::z() const requires(N >= 3)
{
    return v[2];
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
constexpr T tinygl::Vec<N, T>::w() const requires(N >= 4)
{
    return v[3];
}

template<std::size_t N, typename T>
requires (N >= 2 && N <= 4)
constexpr T* tinygl::Vec<N, T>::data() noexcept
{
    return v.data();
}

template<std::size_t N, typename T>
requires (N >= 2 && N <= 4)
constexpr const T* tinygl::Vec<N, T>::data() const noexcept
{
    return v.data();
}

#endif // TINYGL_VEC_H
