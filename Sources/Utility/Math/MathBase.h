#pragma once

#include <cmath>
#include <algorithm>

namespace Math
{
    constexpr float PI = 3.141592741f;
    constexpr float EPSILON = 0.0001f;



    struct Vec2
    {
        float x = 0.f;
        float y = 0.f;

        Vec2() = default;
        Vec2(float x, float y);

        void Set(float inX, float inY);

        Vec2 operator-() const;
        Vec2 operator-(const Vec2& rhs) const;
        Vec2 operator+(float s) const;
        Vec2 operator+(const Vec2& rhs) const;
        Vec2 operator*(float s) const;
        Vec2 operator/(float s) const;
        void operator-=(const Vec2& rhs);
        void operator*=(float s);
        void operator+=(const Vec2& rhs);

        float LenSqr() const;
        float Len() const;
        void Rotate(float radians);
        void Normalize();
    };

    struct Mat2
    {
        union
        {
            struct
            {
                float m00 = 0.f, m01 = 0.f;
                float m10 = 0.f, m11 = 0.f;
            };

            float m[2][2]{};
            float v[4]{};
        };

        Mat2() = default;
        Mat2(float radians);
        Mat2(float a, float b, float c, float d);

        void Set(float radians);

        Mat2 Abs() const;
        Vec2 AxisX() const;
        Vec2 AxisY() const;
        Mat2 Transpose() const;
        Vec2 operator*(const Vec2& rhs) const;
        Mat2 operator*(const Mat2& rhs) const;
    };

    Vec2 operator*(float s, const Vec2& v);

    Vec2 Min(const Vec2& a, const Vec2& b);
    Vec2 Max(const Vec2& a, const Vec2& b);
    float Dot(const Vec2& a, const Vec2& b);
    float DistSqr(const Vec2& a, const Vec2& b);
    Vec2 Cross(const Vec2& v, float a);
    Vec2 Cross(float a, const Vec2& v);
    float Cross(const Vec2& a, const Vec2& b);
    float Sqr(float a);
    float Clamp(float min, float max, float a);
    int32_t Round(float a);
    float Random(float l, float h);
    bool Equal(float a, float b, float epsilon = Math::EPSILON);
    bool BiasGreaterThan(float a, float b, float biasRelative = 0.95f, float biasAbsolute = 0.01f);
}

