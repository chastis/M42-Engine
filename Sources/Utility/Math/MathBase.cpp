#include "MathBase.h"

namespace Math
{

    Vec2::Vec2(float x, float y) : x(x), y(y)
    {}

    void Vec2::Set(float inX, float inY)
    {
        x = inX;
        y = inY;
    }

    Vec2 Vec2::operator-() const
    {
        return { -x, -y };
    }

    Vec2 Vec2::operator-(const Vec2& rhs) const
    {
        return { x - rhs.x, y - rhs.y };
    }

    Vec2 Vec2::operator+(float s) const
    {
        return { x + s, y + s };
    }

    Vec2 Vec2::operator+(const Vec2& rhs) const
    {
        return { x + rhs.x, y + rhs.y };
    }

    Vec2 Vec2::operator*(float s) const
    {
        return { x * s, y * s };
    }

    Vec2 Vec2::operator/(float s) const
    {
        return { x / s, y / s };
    }

    void Vec2::operator-=(const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
    }

    void Vec2::operator*=(float s)
    {
        x *= s;
        y *= s;
    }

    void Vec2::operator+=(const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }

    float Vec2::LenSqr() const
    {
        return x * x + y * y;
    }

    float Vec2::Len() const
    {
        return std::sqrt(LenSqr());
    }

    void Vec2::Rotate(float radians)
    {
        const float c = std::cos(radians);
        const float s = std::sin(radians);

        const float xr = x * c - y * s;
        const float yr = x * s + y * c;

        x = xr;
        y = yr;
    }

    void Vec2::Normalize()
    {
        const float len = Len();

        if (len > Math::EPSILON)
        {
            const float invLen = 1.f / len;
            x *= invLen;
            y *= invLen;
        }
    }

    Vec2 operator*(float s, const Vec2& v)
    {
        return { s * v.x, s * v.y };
    }

    Mat2::Mat2(float radians)
    {
        const float c = std::cos(radians);
        const float s = std::sin(radians);

        m00 = c; m01 = -s;
        m10 = s; m11 = c;
    }

    Mat2::Mat2(float a, float b, float c, float d)
        : m00(a), m01(b), m10(c), m11(d)
    {}

    void Mat2::Set(float radians)
    {
        const float c = std::cos(radians);
        const float s = std::sin(radians);

        m00 = c; m01 = -s;
        m10 = s; m11 = c;
    }

    Mat2 Mat2::Abs() const
    {
        return { std::abs(m00), std::abs(m01), std::abs(m10), std::abs(m11) };
    }

    Vec2 Mat2::AxisX() const
    {
        return { m00, m10 };
    }

    Vec2 Mat2::AxisY() const
    {
        return { m01, m11 };
    }

    Mat2 Mat2::Transpose() const
    {
        return { m00, m10, m01, m11 };
    }

    Vec2 Mat2::operator*(const Vec2& rhs) const
    {
        return { m00 * rhs.x + m01 * rhs.y, m10 * rhs.x + m11 * rhs.y };
    }

    Mat2 Mat2::operator*(const Mat2& rhs) const
    {
        // [00 01]  [00 01]
        // [10 11]  [10 11]

        return {
            m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0],
            m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1],
            m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0],
            m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1]
        };
    }

    Vec2 Min(const Vec2& a, const Vec2& b)
    {
        return {std::min(a.x, b.x), std::min(a.y, b.y)};
    }

    Vec2 Max(const Vec2& a, const Vec2& b)
    {
        return {std::max(a.x, b.x), std::max(a.y, b.y)};
    }

    float Dot(const Vec2& a, const Vec2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    float DistSqr(const Vec2& a, const Vec2& b)
    {
        const Vec2 c = a - b;
        return Dot(c, c);
    }

    Vec2 Cross(const Vec2& v, float a)
    {
        return {a * v.y, -a * v.x};
    }

    Vec2 Cross(float a, const Vec2& v)
    {
        return {-a * v.y, a * v.x};
    }

    float Cross(const Vec2& a, const Vec2& b)
    {
        return a.x * b.y - a.y * b.x;
    }

    bool Equal(float a, float b, float epsilon)
    {
        return std::abs(a - b) <= epsilon;
    }

    float Sqr(float a)
    {
        return a * a;
    }

    float Clamp(float min, float max, float a)
    {
        if (a < min) return min;
        if (a > max) return max;
        return a;
    }

    int32_t Round(float a)
    {
        return std::lround(a);
    }

    float Random(float l, float h)
    {
        auto a = static_cast<float>(rand());
        a /= RAND_MAX;
        a = (h - l) * a + l;
        return a;
    }

    bool BiasGreaterThan(float a, float b, float biasRelative, float biasAbsolute)
    {
        return a >= b * biasRelative + a * biasAbsolute;
    }

}