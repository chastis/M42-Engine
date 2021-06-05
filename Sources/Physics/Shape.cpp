#include "Shape.hpp"
#include "core.hpp"
#include "cassert"

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
    body->im = (body->m) ? 1.0f / body->m : 0.0f;
    body->I = body->m * radius * radius;
    body->iI = (body->I) ? 1.0f / body->I : 0.0f;
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
    // Calculate centroid and moment of interia
    Vec2 c(0.0f, 0.0f); // centroid
    float area = 0.0f;
    float I = 0.0f;
    const float k_inv3 = 1.0f / 3.0f;

    for (uint32_t i1 = 0; i1 < m_vertexCount; ++i1)
    {
        // Triangle vertices, third vertex implied as (0, 0)
        Vec2 p1(m_vertices[i1]);
        const uint32_t i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
        Vec2 p2(m_vertices[i2]);

        float D = Cross(p1, p2);
        const float triangleArea = 0.5f * D;

        area += triangleArea;

        // Use area to weight the centroid average, not just vertex position
        c += triangleArea * k_inv3 * (p1 + p2);

        const float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
        const float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
        I += (0.25f * k_inv3 * D) * (intx2 + inty2);
    }

    c *= 1.0f / area;

    // Translate vertices to centroid (make the centroid (0, 0)
    // for the polygon in model space)
    // Not floatly necessary, but I like doing this anyway
    for (uint32_t i = 0; i < m_vertexCount; ++i)
        m_vertices[i] -= c;

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

// Half width and half height
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
    // No hulls with less than 3 vertices (ensure actual polygon)
    assert (count > 2 && count <= MaxPolyVertexCount);

    count = std::min(count, MaxPolyVertexCount);

    // Find the right most point on the hull
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

        // If matching x then take farthest negative y
        else if (x == highestXCoord)
            if (vertices[i].y < vertices[rightMost].y)
                rightMost = i;
    }

    int32_t hull[MaxPolyVertexCount];
    int32_t outCount = 0;
    int32_t indexHull = rightMost;

    for (;;)
    {
        hull[outCount] = indexHull;

        // Search for next index that wraps around the hull
        // by computing cross products to find the most counter-clockwise
        // vertex in the set, given the previos hull index
        int32_t nextHullIndex = 0;
        for (int32_t i = 1; i < static_cast<int32_t>(count); ++i)
        {
            // Skip if same coordinate as we need three unique
            // points in the set to perform a cross product
            if (nextHullIndex == indexHull)
            {
                nextHullIndex = i;
                continue;
            }

            // Cross every set of three unique vertices
            // Record each counter clockwise third vertex and add
            // to the output hull
            // See : http://www.oocities.org/pcgpe/math2d.html
            Vec2 e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
            Vec2 e2 = vertices[i] - vertices[hull[outCount]];
            float c = Cross(e1, e2);
            if (c < 0.0f)
                nextHullIndex = i;

            // Cross product is zero then e vectors are on same line
            // therefor want to record vertex farthest along that line
            if (c == 0.0f && e2.LenSqr() > e1.LenSqr())
                nextHullIndex = i;
        }

        ++outCount;
        indexHull = nextHullIndex;

        // Conclude algorithm upon wrap-around
        if (nextHullIndex == rightMost)
        {
            m_vertexCount = outCount;
            break;
        }
    }

    // Copy vertices into shape's vertices
    for (uint32_t i = 0; i < m_vertexCount; ++i)
        m_vertices[i] = vertices[hull[i]];

    // Compute face normals
    for (uint32_t i1 = 0; i1 < m_vertexCount; ++i1)
    {
        uint32_t i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
        Vec2 face = m_vertices[i2] - m_vertices[i1];

        // Ensure no zero-length edges, because that's bad
        assert(face.LenSqr() > Math::EPSILON * Math::EPSILON);

        // Calculate normal with 2D cross product between vector and scalar
        m_normals[i1] = Vec2(face.y, -face.x);
        m_normals[i1].Normalize();
    }
}

// The extreme point along a direction within a polygon
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

