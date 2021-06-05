#include "Shape.hpp"
#include "core.hpp"
#include "cassert"
#include <cmath>

Circle::Circle(float r)
{
    radius = r;
}

Shape* Circle::Clone() const
{
    return new Circle(radius);
}

void Circle::Initialize()
{
    ComputeMass(1.0f);
}

void Circle::ComputeMass(float density)
{
    body->m = Math::PI * radius * radius * density;
    body->im = static_cast<bool>(body->m) ? 1.0f / body->m : 0.0f;
    body->I = body->m * radius * radius;
    body->iI = static_cast<bool>(body->I) ? 1.0f / body->I : 0.0f;
}

void Circle::SetOrient(float radians)
{}

void Circle::Draw() const
{
    const uint32_t k_segments = 20;

    glColor3f(body->r, body->g, body->b);
    glBegin(GL_LINE_LOOP);
    float theta = body->orient;
    const float inc = Math::PI * 2.0f / static_cast<float>(k_segments);
    for (uint32_t i = 0; i < k_segments; ++i)
    {
        theta += inc;
        Vec2 p(std::cos(theta), std::sin(theta));
        p *= radius;
        p += body->position;
        glVertex2f(p.x, p.y);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    Vec2 r(0, 1.0f);
    const float c = std::cos(body->orient);
    const float s = std::sin(body->orient);
    r.Set(r.x * c - r.y * s, r.x * s + r.y * c);
    r *= radius;
    r = r + body->position;
    glVertex2f(body->position.x, body->position.y);
    glVertex2f(r.x, r.y);
    glEnd();
}

Shape::Type Circle::GetType() const
{
    return Shape::Type::Circle;
}

void PolygonShape::Initialize()
{
    ComputeMass(1.0f);
}

Shape* PolygonShape::Clone() const
{
    auto* poly = new PolygonShape();
    poly->u = u;
    for (uint32_t i = 0; i < m_vertexCount; ++i)
    {
        poly->m_vertices[i] = m_vertices[i];
        poly->m_normals[i] = m_normals[i];
    }
    poly->m_vertexCount = m_vertexCount;
    return poly;
}

void PolygonShape::ComputeMass(float density)
{
    Vec2 centroid = { 0.0f, 0.0f };
    float area = 0.0f;
    float I = 0.0f;
    const float k_inv3 = 1.0f / 3.0f;

    for (uint32_t i1 = 0; i1 < m_vertexCount; ++i1)
    {
        Vec2 p1(m_vertices[i1]);
        const uint32_t i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
        Vec2 p2(m_vertices[i2]);

        float D = Cross(p1, p2);
        const float triangleArea = 0.5f * D;

        area += triangleArea;

        centroid += triangleArea * k_inv3 * (p1 + p2);

        const float intX2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
        const float intY2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
        I += (0.25f * k_inv3 * D) * (intX2 + intY2);
    }

    centroid *= 1.0f / area;

    for (uint32_t i = 0; i < m_vertexCount; ++i)
    {
        m_vertices[i] -= centroid;
    }

    body->m = density * area;
    body->im = static_cast<bool>(body->m) ? 1.0f / body->m : 0.0f;
    body->I = I * density;
    body->iI = static_cast<bool>(body->I) ? 1.0f / body->I : 0.0f;
}

void PolygonShape::SetOrient(float radians)
{
    u.Set(radians);
}

void PolygonShape::Draw() const
{
    glColor3f(body->r, body->g, body->b);
    glBegin(GL_LINE_LOOP);
    for (uint32_t i = 0; i < m_vertexCount; ++i)
    {
        const Vec2 v = body->position + u * m_vertices[i];
        glVertex2f(v.x, v.y);
    }
    glEnd();
}

Shape::Type PolygonShape::GetType() const
{
    return Shape::Type::Poly;
}

void PolygonShape::SetBox(float hw, float hh)
{
    m_vertexCount = 4;
    m_vertices[0].Set(-hw, -hh);
    m_vertices[1].Set(hw, -hh);
    m_vertices[2].Set(hw, hh);
    m_vertices[3].Set(-hw, hh);
    m_normals[0].Set(0.0f, -1.0f);
    m_normals[1].Set(1.0f, 0.0f);
    m_normals[2].Set(0.0f, 1.0f);
    m_normals[3].Set(-1.0f, 0.0f);
}

void PolygonShape::Set(Vec2* vertices, uint32_t count)
{
    assert(count > 2 && count <= MaxPolyVertexCount);

    count = std::min(count, MaxPolyVertexCount);

    int32_t rightMost = 0;
    float highestXCoord = vertices[0].x;
    for (uint32_t i = 1; i < count; ++i)
    {
        const float x = vertices[i].x;
        if (x > highestXCoord)
        {
            highestXCoord = x;
            rightMost = i;
        }
        else if (x == highestXCoord && vertices[i].y < vertices[rightMost].y)
        {
            rightMost = i;
        }
    }

    int32_t hull[MaxPolyVertexCount];
    int32_t outCount = 0;
    int32_t indexHull = rightMost;

    for (;;)
    {
        hull[outCount] = indexHull;

        int32_t nextHullIndex = 0;
        for (int32_t i = 1; i < static_cast<int32_t>(count); ++i)
        {
            if (nextHullIndex == indexHull)
            {
                nextHullIndex = i;
                continue;
            }

            const Vec2 e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
            const Vec2 e2 = vertices[i] - vertices[hull[outCount]];
            const float c = Cross(e1, e2);
            if (c < 0.f)
            {
                nextHullIndex = i;
            }

            if (Math::IsNearlyZero(c) && e2.LenSqr() > e1.LenSqr())
            {
                nextHullIndex = i;
            }
        }

        ++outCount;
        indexHull = nextHullIndex;

        if (nextHullIndex == rightMost)
        {
            m_vertexCount = outCount;
            break;
        }
    }

    for (uint32_t i = 0; i < m_vertexCount; ++i)
    {
        m_vertices[i] = vertices[hull[i]];
    }

    for (uint32_t i1 = 0; i1 < m_vertexCount; ++i1)
    {
        uint32_t i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
        const Vec2 face = m_vertices[i2] - m_vertices[i1];

        assert(face.LenSqr() > Math::EPSILON * Math::EPSILON);

        m_normals[i1] = { face.y, -face.x };
        m_normals[i1].Normalize();
    }
}

Vec2 PolygonShape::GetSupport(const Vec2& dir)
{
    float bestProjection = -FLT_MAX;
    Vec2 bestVertex;

    for (uint32_t i = 0; i < m_vertexCount; ++i)
    {
        Vec2 v = m_vertices[i];
        const float projection = Dot(v, dir);

        if (projection > bestProjection)
        {
            bestVertex = v;
            bestProjection = projection;
        }
    }

    return bestVertex;
}

