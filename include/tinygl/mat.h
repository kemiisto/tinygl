#ifndef LEARNOPENGL_MATRIX_H
#define LEARNOPENGL_MATRIX_H

#include "tinygl/vec.h"
#include <array>
#include <cassert>
#include <initializer_list>

namespace tinygl
{
    template<std::size_t N, typename T = float>
    requires(N >= 2 && N <= 4)
    class Mat
    {
    public:
        Mat(std::initializer_list<T> values);

        static Mat identity();
        static Mat translation(const Vec<N-1,T>& t);
        static Mat scale(const Vec<N-1,T>& s);

        constexpr T& operator[](std::size_t j, std::size_t i);
        constexpr T operator[](std::size_t j, std::size_t i) const;

        constexpr T* data() noexcept;
        constexpr const T* data() const noexcept;
    private:
        std::array<float, N*N> v;
    };

    using Mat4 = Mat<4, float>;
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
tinygl::Mat<N,T>::Mat(std::initializer_list<T> values)
{
    assert(values.size() == N*N);

    typename std::initializer_list<T>::iterator it = values.begin();
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            v[i + j * N] = *it++;
        }
    }
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
tinygl::Mat<N,T> tinygl::Mat<N,T>::identity()
{
    return tinygl::Mat<N,T> {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
tinygl::Mat<N,T> tinygl::Mat<N,T>::translation(const Vec<N-1,T>& t)
{
    return tinygl::Mat<N,T> {
        1.0f, 0.0f, 0.0f, t.x(),
        0.0f, 1.0f, 0.0f, t.y(),
        0.0f, 0.0f, 1.0f, t.z(),
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
tinygl::Mat<N,T> tinygl::Mat<N,T>::scale(const Vec<N-1,T>& s)
{
    return tinygl::Mat<N,T> {
        s.x(),  0.0f,  0.0f, 0.0f,
         0.0f, s.y(),  0.0f, 0.0f,
         0.0f,  0.0f, s.z(), 0.0f,
         0.0f,  0.0f,  0.0f, 1.0f,
    };
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
constexpr T& tinygl::Mat<N,T>::operator[](std::size_t j, std::size_t i)
{
    return v[i + j * N];
}

template<std::size_t N, typename T>
requires(N >= 2 && N <= 4)
constexpr T tinygl::Mat<N,T>::operator[](std::size_t j, std::size_t i) const
{
    return v[i + j * N];
}

template<std::size_t N, typename T>
requires (N >= 2 && N <= 4)
constexpr T* tinygl::Mat<N, T>::data() noexcept
{
    return v.data();
}

template<std::size_t N, typename T>
requires (N >= 2 && N <= 4)
constexpr const T* tinygl::Mat<N, T>::data() const noexcept
{
    return v.data();
}

#endif // LEARNOPENGL_MATRIX_H
