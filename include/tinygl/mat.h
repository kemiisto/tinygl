#ifndef TINYGL_MATRIX_H
#define TINYGL_MATRIX_H

#include "tinygl/vec.h"
#include "tinygl/util.h"
#include <array>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>

namespace tinygl
{
    template<typename T>
    class Mat4x4;

    template<typename T>
    Mat4x4<T> operator*(const Mat4x4<T>& a, const Mat4x4<T>& b);

    template<typename T>
    class Mat4x4
    {
    public:
        explicit Mat4x4(bool identity = true);
        Mat4x4(std::initializer_list<T> values);

        constexpr T& operator()(std::size_t i, std::size_t j);
        constexpr T operator()(std::size_t i, std::size_t j) const;

        Mat4x4 operator*=(const Mat4x4& o);

        void setToIdentity();

        void scale(T s);
        void scale(const Vec<3,T>& s);
        void translate(const Vec<3,T>& t);
        void rotate(T angle, const Vec<3,T>& axis);

        constexpr T* data() noexcept;
        constexpr const T* data() const noexcept;

        void perspective(T verticalAngle, T aspectRatio, T nearPlane, T farPlane);

        bool closeTo(const Mat4x4& other);

        friend Mat4x4<T> operator* <>(const Mat4x4<T>& a, const Mat4x4<T>& b);
    private:
        float m[4][4]{};
        void rotateX(T angle);
        void rotateY(T angle);
        void rotateZ(T angle);
    };

    using Mat4 = Mat4x4<float>;
}

template<typename T>
tinygl::Mat4x4<T>::Mat4x4(bool identity)
{
    if (identity) {
        setToIdentity();
    }
}

template<typename T>
tinygl::Mat4x4<T>::Mat4x4(std::initializer_list<T> values)
{
    assert(values.size() == 4*4);

    typename std::initializer_list<T>::iterator it = values.begin();
    for (std::size_t i = 0; i < 4; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            m[j][i] = *it++;
        }
    }
}

template<typename T>
constexpr T& tinygl::Mat4x4<T>::operator()(std::size_t i, std::size_t j)
{
    return m[i][j];
}

template<typename T>
constexpr T tinygl::Mat4x4<T>::operator()(std::size_t i, std::size_t j) const
{
    return m[i][j];
}

template<typename T>
void tinygl::Mat4x4<T>::setToIdentity()
{
    for (size_t j = 0; j < 4; ++j) {
        for (size_t i = 0; i < 4; ++i) {
            if (j == i) {
                m[j][i] = T{1};
            } else {
                m[j][i] = T{0};
            }
        }
    }
}

template<typename T>
void tinygl::Mat4x4<T>::translate(const tinygl::Vec<3,T>& t)
{
    /**
     * | m00   m01   m02   m03 |   | 1  0  0  tx |   | m00   m01   m02   m00*tx + m01*ty + m02*tz + m03 |
     * | m10   m11   m12   m13 |   | 0  1  0  ty |   | m10   m11   m12   m10*tx + m11*ty + m12*tz + m13 |
     * | m20   m21   m22   m23 | * | 0  0  1  tz | = | m20   m21   m22   m20*tx + m21*ty + m22*tz + m23 |
     * | m30   m31   m32   m33 |   | 0  0  0  1  |   | m30   m31   m32   m30*tx + m31*ty + m32*tz + m33 |
     *
     * Only the last column is changing.
     * Remember the column-major order!
     */
    m[3][0] += m[0][0] * t.x() + m[1][0] * t.y() + m[2][0] * t.z();
    m[3][1] += m[0][1] * t.x() + m[1][1] * t.y() + m[2][1] * t.z();
    m[3][2] += m[0][2] * t.x() + m[1][2] * t.y() + m[2][2] * t.z();
    m[3][3] += m[0][3] * t.x() + m[1][3] * t.y() + m[2][3] * t.z();
}

template<typename T>
void tinygl::Mat4x4<T>::scale(T s)
{
    scale({s, s, s});
}

template<typename T>
void tinygl::Mat4x4<T>::scale(const Vec<3,T>& s)
{
    /**
     * | m00   m01   m02   m03 |   | sx  0   0   0 |   | m00*sx   m01*sy   m02*sz   m03 |
     * | m10   m11   m12   m13 |   | 0   sy  0   0 |   | m10*sx   m11*sy   m12*sz   m13 |
     * | m20   m21   m22   m23 | * | 0   0   sz  0 | = | m20*sx   m21*sy   m22*sz   m23 |
     * | m30   m31   m32   m33 |   | 0   0   0   1 |   | m30*sx   m31*sy   m32*sz   m33 |
     *
     * All the columns except the last one are changing.
     * Remember the column-major order!
     */
    m[0][0] *= s.x();   m[1][0] *= s.y();   m[2][0] *= s.z();
    m[0][1] *= s.x();   m[1][1] *= s.y();   m[2][1] *= s.z();
    m[0][2] *= s.x();   m[1][2] *= s.y();   m[2][2] *= s.z();
    m[0][3] *= s.x();   m[1][3] *= s.y();   m[2][3] *= s.z();
}

template<typename T>
void tinygl::Mat4x4<T>::rotateX(T angle)
{
    /**
     * | m00   m01   m02   m03 |   | 1   0   0   0 |   | m00    m01*c + m02*s    -m01*s + m02*c   m03 |
     * | m10   m11   m12   m13 |   | 0   c  -s   0 |   | m10    m11*c + m12*s    -m11*s + m12*c   m13 |
     * | m20   m21   m22   m23 | * | 0   s   c   0 | = | m20    m21*c + m22*s    -m21*s + m22*c   m23 |
     * | m30   m31   m32   m33 |   | 0   0   0   1 |   | m30    m31*c + m32*s    -m31*s + m32*c   m33 |
     *
     * Only the two middle columns are changing.
     * Remember the column-major order!
     */
    const T a = degreesToRadians(angle);
    const T c = std::cos(a);
    const T s = std::sin(a);

    T tmp;
    m[1][0] = (tmp = m[1][0]) * c + m[2][0] * s;   m[2][0] = -tmp * s + m[2][0] * c;
    m[1][1] = (tmp = m[1][1]) * c + m[2][1] * s;   m[2][1] = -tmp * s + m[2][1] * c;
    m[1][2] = (tmp = m[1][2]) * c + m[2][2] * s;   m[2][2] = -tmp * s + m[2][2] * c;
    m[1][3] = (tmp = m[1][3]) * c + m[2][3] * s;   m[2][3] = -tmp * s + m[2][3] * c;
}

template<typename T>
void tinygl::Mat4x4<T>::rotateY(T angle)
{
    /**
     * | m00   m01   m02   m03 |   |  c   0   s   0 |   | m00*c - m02*s   m01   m00*s + m02*c   m03 |
     * | m10   m11   m12   m13 |   |  0   1   0   0 |   | m10*c - m12*s   m11   m10*s + m12*c   m13 |
     * | m20   m21   m22   m23 | * | -s   0   c   0 | = | m20*c - m22*s   m21   m20*s + m22*c   m23 |
     * | m30   m31   m32   m33 |   |  0   0   0   1 |   | m30*c - m32*s   m31   m30*s + m32*c   m33 |
     *
     * Only the odd columns are changing.
     * Remember the column-major order!
     */
    const T a = degreesToRadians(angle);
    const T c = std::cos(a);
    const T s = std::sin(a);

    T tmp;
    m[0][0] = (tmp = m[0][0]) * c - m[2][0] * s;   m[2][0] = tmp * s + m[2][0] * c;
    m[0][1] = (tmp = m[0][1]) * c - m[2][1] * s;   m[2][1] = tmp * s + m[2][1] * c;
    m[0][2] = (tmp = m[0][2]) * c - m[2][2] * s;   m[2][2] = tmp * s + m[2][2] * c;
    m[0][3] = (tmp = m[0][3]) * c - m[2][3] * s;   m[2][3] = tmp * s + m[2][3] * c;
}

template<typename T>
void tinygl::Mat4x4<T>::rotateZ(T angle)
{
    /**
     * | m00   m01   m02   m03 |   | c   -s   0   0 |   | m00*c + m01*s   -m00*s - m01*c   m02   m03 |
     * | m10   m11   m12   m13 |   | s    c   0   0 |   | m10*c + m11*s   -m10*s - m11*c   m12   m13 |
     * | m20   m21   m22   m23 | * | 0    0   1   0 | = | m20*c + m21*s   -m20*s - m21*c   m22   m23 |
     * | m30   m31   m32   m33 |   | 0    0   0   1 |   | m30*c + m31*s   -m30*s - m31*c   m32   m33 |
     *
     * Only first two columns are changing.
     * Remember the column-major order!
     */
    const T a = degreesToRadians(angle);
    const T c = std::cos(a);
    const T s = std::sin(a);

    T tmp;
    m[0][0] = (tmp = m[0][0]) * c + m[1][0] * s;   m[1][0] = -tmp * s + m[1][0] * c;
    m[0][1] = (tmp = m[0][1]) * c + m[1][1] * s;   m[1][1] = -tmp * s + m[1][1] * c;
    m[0][2] = (tmp = m[0][2]) * c + m[1][2] * s;   m[1][2] = -tmp * s + m[1][2] * c;
    m[0][3] = (tmp = m[0][3]) * c + m[1][3] * s;   m[1][3] = -tmp * s + m[1][3] * c;
}

template<typename T>
void tinygl::Mat4x4<T>::rotate(T angle, const tinygl::Vec<3, T>& axis)
{
    auto x = axis.x();
    auto y = axis.y();
    auto z = axis.z();

    if (x == T{0}) {
        if (y == T{0}) {
            if (z != T{0}) {
                rotateZ(angle);
                return;
            }
        } else if (z == T{0}) {
            rotateY(angle);
            return;
        }
    } else if (y == T{0} && z == T{0}) {
        rotateX(angle);
        return;
    }

    const T a = degreesToRadians(angle);
    const T c = std::cos(a);
    const T s = std::sin(a);

    T len = x*x + y*y + z*z;
    if (!tinygl::close(len, T{1}) && !closeToZero(len)) {
        len = std::sqrt(len);
        x = x / len;
        y = y / len;
        z = z / len;
    }
    T ic = T{1} - c;

    auto r = Mat4x4(false);
    r.m[0][0] = x * x * ic + c;
    r.m[1][0] = x * y * ic - z * s;
    r.m[2][0] = x * z * ic + y * s;
    r.m[3][0] = T{0};

    r.m[0][1] = y * x * ic + z * s;
    r.m[1][1] = y * y * ic + c;
    r.m[2][1] = y * z * ic - x * s;
    r.m[3][1] = T{0};

    r.m[0][2] = x * z * ic - y * s;
    r.m[1][2] = y * z * ic + x * s;
    r.m[2][2] = z * z * ic + c;
    r.m[3][2] = T{0};

    r.m[0][3] = T{0};
    r.m[1][3] = T{0};
    r.m[2][3] = T{0};
    r.m[3][3] = T{1};

    *this *= r;
}

template<typename T>
constexpr T* tinygl::Mat4x4<T>::data() noexcept
{
    return *m;
}

template<typename T>
constexpr const T* tinygl::Mat4x4<T>::data() const noexcept
{
    return *m;
}

template<typename T>
void tinygl::Mat4x4<T>::perspective(T verticalAngle, T aspectRatio, T nearPlane, T farPlane)
{
    if (nearPlane == farPlane || aspectRatio == 0.0f) {
        return;
    }

    const T halfAngle = degreesToRadians(verticalAngle / 2.0);
    const T sine = std::sin(halfAngle);
    if (sine == 0.0f) {
        return;
    }
    T cotan = std::cos(halfAngle) / sine;
    T clip = farPlane - nearPlane;

    m[0][0] = cotan / aspectRatio;
    m[1][0] = T{0};
    m[2][0] = T{0};
    m[3][0] = T{0};

    m[0][1] = T{0};
    m[1][1] = cotan;
    m[2][1] = T{0};
    m[3][1] = T{0};

    m[0][2] = T{0};
    m[1][2] = T{0};
    m[2][2] = -(nearPlane + farPlane) / clip;
    m[3][2] = -(2.0f * nearPlane * farPlane) / clip;

    m[0][3] = T{0};
    m[1][3] = T{0};
    m[2][3] = T{-1};
    m[3][3] = T{0};
}

template<typename T>
tinygl::Mat4x4<T> tinygl::Mat4x4<T>::operator*=(const tinygl::Mat4x4<T>& other)
{
    const auto o = other; // prevent aliasing when &o == this
    *this = *this * o;
    return *this;
}

template<typename T>
bool tinygl::Mat4x4<T>::closeTo(tinygl::Mat4x4<T> const& other)
{
    // A simple iterative algorithm but for column-major order.
    for (size_t j = 0; j < 4; ++j) {
        for (size_t i = 0; i < 4; ++i) {
            if (!close(m[i][j], other.m[i][j])) {
                std::cout << '[' << i << ',' << j << "]: " << m[i][j] << ' ' << other.m[i][j] << std::endl;
                return false;
            }
        }
    }
    return true;
}

template<typename T>
tinygl::Mat4x4<T> tinygl::operator*(const Mat4x4<T>& a, const Mat4x4<T>& b)
{
    auto c = tinygl::Mat4x4<T>(false);
    for (size_t j = 0; j < 4; ++j) {
        for (size_t i = 0; i < 4; ++i) {
            c.m[j][i] = T{0};
            for (size_t k = 0; k < 4; ++k) {
                c.m[j][i] += a.m[k][i] * b.m[j][k];
            }
        }
    }
    return c;
}

#endif // TINYGL_MATRIX_H
