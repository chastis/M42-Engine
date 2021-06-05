#pragma once

#include "Ductility/Utility/CoreMath.hpp"
#include "Ductility/Body.hpp"

constexpr uint32_t MaxPolyVertexCount = 64;

using Math::Vec2;
using Math::Mat2;

struct Shape
{
    enum class Type
    {
        Circle = 0,
        Poly = 1,
        Count = 2
    };

    Shape() = default;
    ~Shape() = default;
    [[nodiscard]] virtual Shape* Clone() const = 0;
    [[nodiscard]] virtual Type GetType() const = 0;

    virtual void Initialize() = 0;
    virtual void ComputeMass(float density) = 0;
    virtual void SetOrient(float radians) = 0;
    virtual void Draw() const = 0;

    Body* body = nullptr;
};

struct Circle : public Shape
{
    Circle(float r);
    
    void Initialize() override;
    void ComputeMass(float density) override;
    void SetOrient(float radians) override;

    [[nodiscard]] Shape* Clone() const override;
    [[nodiscard]] Type GetType() const override;

    void Draw() const override;

    float radius = 0.f;
};

struct PolygonShape : public Shape
{
    void Initialize() override;

    void Set(Vec2* vertices, uint32_t count);
    void SetOrient(float radians) override;
    void SetBox(float hw, float hh);
    void ComputeMass(float density) override;

    [[nodiscard]] Shape* Clone() const override;
    [[nodiscard]] Type GetType() const override;
    [[nodiscard]] Vec2 GetSupport(const Vec2& dir);

    void Draw() const override;

    uint32_t m_vertexCount;
    Vec2 m_vertices[MaxPolyVertexCount];
    Vec2 m_normals[MaxPolyVertexCount];
    Mat2 u;
};
