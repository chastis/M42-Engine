#include "core.hpp"
#include "Ductility/Shape.hpp"
#include <cassert>
#include <cmath>

void DispatchCollision(Manifold* m, Body* a, Body* b)
{
    assert(m && a && b);
    assert(a->shape && b->shape);
    switch (a->shape->GetType())
    {
    case Shape::Type::Circle:
    {
        switch (b->shape->GetType())
        {
        case Shape::Type::Circle:
        {
            CircleToCircle(m, a, b);
            break;
        }
        case Shape::Type::Poly:
        {
            CircleToPolygon(m, a, b);
            break;
        }
        case Shape::Type::Count:
        default:
        {
            assert(false);
        }
        }
        break;
    }
    case Shape::Type::Poly:
    {
        switch (b->shape->GetType())
        {
        case Shape::Type::Circle:
        {
            PolygonToCircle(m, a, b);
            break;
        }
        case Shape::Type::Poly:
        {
            PolygonToPolygon(m, a, b);
            break;
        }
        case Shape::Type::Count:
        default:
        {
            assert(false);
        }
        }
        break;
    }
    case Shape::Type::Count:
    default:
    {
        assert(false);
    }
    }
}

void CircleToCircle(Manifold* m, Body* a, Body* b)
{
    Circle* A = reinterpret_cast<Circle*>(a->shape);
    Circle* B = reinterpret_cast<Circle*>(b->shape);

    const Math::Vec2 normal = b->position - a->position;

    const float distSqr = normal.LenSqr();
    const float radius = A->radius + B->radius;

    // Are not contacting
    if (distSqr >= radius * radius)
    {
        m->contact_count = 0;
        return;
    }

    const float distance = std::sqrt(distSqr);

    m->contact_count = 1;

    if (Math::IsNearlyZero(distance))
    {
        m->penetration = A->radius;
        m->normal = Math::Vec2(1, 0);
        m->contacts[0] = a->position;
    }
    else
    {
        m->penetration = radius - distance;
        m->normal = normal / distance;
        m->contacts[0] = m->normal * A->radius + a->position;
    }
}

void CircleToPolygon(Manifold* m, Body* a, Body* b)
{
    Circle* A = reinterpret_cast<Circle*>(a->shape);
    PolygonShape* B = reinterpret_cast<PolygonShape*>(b->shape);

    m->contact_count = 0;

    const Math::Vec2 center = B->u.Transpose() * (a->position - b->position);

    float separation = -FLT_MAX;
    uint32_t faceNormal = 0;
    for (uint32_t i = 0; i < B->m_vertexCount; ++i)
    {
        const float s = Dot(B->m_normals[i], center - B->m_vertices[i]);

        if (s > A->radius)
        {
            return;
        }

        if (s > separation)
        {
            separation = s;
            faceNormal = i;
        }
    }

    Math::Vec2 v1 = B->m_vertices[faceNormal];
    const uint32_t i2 = faceNormal + 1 < B->m_vertexCount ? faceNormal + 1 : 0;
    Math::Vec2 v2 = B->m_vertices[i2];

    if (separation < Math::EPSILON)
    {
        m->contact_count = 1;
        m->normal = -(B->u * B->m_normals[faceNormal]);
        m->contacts[0] = m->normal * A->radius + a->position;
        m->penetration = A->radius;
        return;
    }

    const float dot1 = Dot(center - v1, v2 - v1);
    const float dot2 = Dot(center - v2, v1 - v2);
    m->penetration = A->radius - separation;

    if (dot1 <= 0.f)
    {
        if (DistSqr(center, v1) > A->radius * A->radius)
            return;

        m->contact_count = 1;
        Math::Vec2 n = v1 - center;
        n = B->u * n;
        n.Normalize();
        m->normal = n;
        v1 = B->u * v1 + b->position;
        m->contacts[0] = v1;
    }
    else if (dot2 <= 0.f)
    {
        if (DistSqr(center, v2) > A->radius * A->radius)
            return;

        m->contact_count = 1;
        Math::Vec2 n = v2 - center;
        v2 = B->u * v2 + b->position;
        m->contacts[0] = v2;
        n = B->u * n;
        n.Normalize();
        m->normal = n;
    }
    else
    {
        Math::Vec2 n = B->m_normals[faceNormal];
        if (Dot(center - v1, n) > A->radius)
        {
            return;
        }

        n = B->u * n;
        m->normal = -n;
        m->contacts[0] = m->normal * A->radius + a->position;
        m->contact_count = 1;
    }
}

void PolygonToCircle(Manifold* m, Body* a, Body* b)
{
    CircleToPolygon(m, b, a);
    m->normal = -m->normal;
}

float FindAxisLeastPenetration(PolygonShape* A, PolygonShape* B, uint32_t& faceIndex)
{
    float bestDistance = -FLT_MAX;
    std::uint32_t bestIndex = 0;

    for (std::uint32_t i = 0; i < A->m_vertexCount; ++i)
    {
        Math::Vec2 n = A->m_normals[i];
        Math::Vec2 nw = A->u * n;

        Math::Mat2 buT = B->u.Transpose();
        n = buT * nw;

        Math::Vec2 s = B->GetSupport(-n);

        Math::Vec2 v = A->m_vertices[i];
        v = A->u * v + A->body->position;
        v -= B->body->position;
        v = buT * v;

        const float d = Dot(n, s - v);

        if (d > bestDistance)
        {
            bestDistance = d;
            bestIndex = i;
        }
    }

    faceIndex = bestIndex;
    return bestDistance;
}

void FindIncidentFace(Math::Vec2* v, PolygonShape* RefPoly, PolygonShape* IncPoly, std::uint32_t referenceIndex)
{
    Math::Vec2 referenceNormal = RefPoly->m_normals[referenceIndex];

    referenceNormal = RefPoly->u * referenceNormal;
    referenceNormal = IncPoly->u.Transpose() * referenceNormal;

    std::int32_t incidentFace = 0;
    float minDot = FLT_MAX;
    for (std::uint32_t i = 0; i < IncPoly->m_vertexCount; ++i)
    {
        const float dot = Dot(referenceNormal, IncPoly->m_normals[i]);
        if (dot < minDot)
        {
            minDot = dot;
            incidentFace = i;
        }
    }

    v[0] = IncPoly->u * IncPoly->m_vertices[incidentFace] + IncPoly->body->position;
    incidentFace = incidentFace + 1 >= static_cast<int32_t>(IncPoly->m_vertexCount) ? 0 : incidentFace + 1;
    v[1] = IncPoly->u * IncPoly->m_vertices[incidentFace] + IncPoly->body->position;
}

std::int32_t Clip(Math::Vec2 n, float c, Math::Vec2* face)
{
    std::uint32_t sp = 0;
    Math::Vec2 out[2] = {
      face[0],
      face[1]
    };

    const float d1 = Dot(n, face[0]) - c;
    const float d2 = Dot(n, face[1]) - c;

    if (d1 <= 0.0f) out[sp++] = face[0];
    if (d2 <= 0.0f) out[sp++] = face[1];

    if (d1 * d2 < 0.f)
    {
        const float alpha = d1 / (d1 - d2);
        out[sp] = face[0] + alpha * (face[1] - face[0]);
        ++sp;
    }

    face[0] = out[0];
    face[1] = out[1];

    assert(sp != 3);

    return sp;
}

void PolygonToPolygon(Manifold* m, Body* a, Body* b)
{
    PolygonShape* A = reinterpret_cast<PolygonShape*>(a->shape);
    PolygonShape* B = reinterpret_cast<PolygonShape*>(b->shape);
    m->contact_count = 0;

    uint32_t faceA;
    const float penetrationA = FindAxisLeastPenetration(A, B, faceA);
    if (penetrationA >= 0.f)
    {
        return;
    }

    uint32_t faceB = 0;
    const float penetrationB = FindAxisLeastPenetration(B, A, faceB);
    if (penetrationB >= 0.f)
    {
        return;
    }

    uint32_t referenceIndex = 0;
    bool flip = false;

    PolygonShape* RefPoly;
    PolygonShape* IncPoly; 

    if (Math::BiasGreaterThan(penetrationA, penetrationB))
    {
        RefPoly = A;
        IncPoly = B;
        referenceIndex = faceA;
        flip = false;
    }
    else
    {
        RefPoly = B;
        IncPoly = A;
        referenceIndex = faceB;
        flip = true;
    }

    Vec2 incidentFace[2];
    FindIncidentFace(incidentFace, RefPoly, IncPoly, referenceIndex);

    Math::Vec2 v1 = RefPoly->m_vertices[referenceIndex];
    referenceIndex = referenceIndex + 1 == RefPoly->m_vertexCount ? 0 : referenceIndex + 1;
    Math::Vec2 v2 = RefPoly->m_vertices[referenceIndex];

    v1 = RefPoly->u * v1 + RefPoly->body->position;
    v2 = RefPoly->u * v2 + RefPoly->body->position;

    Math::Vec2 sidePlaneNormal = (v2 - v1);
    sidePlaneNormal.Normalize();

    const Math::Vec2 refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

    const float refC = Dot(refFaceNormal, v1);
    const float negSide = -Dot(sidePlaneNormal, v1);
    const float posSide = Dot(sidePlaneNormal, v2);

    if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
    {
        return;
    }

    if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
    {
        return;
    }

    m->normal = flip ? -refFaceNormal : refFaceNormal;

    uint32_t cp = 0;
    float separation = Dot(refFaceNormal, incidentFace[0]) - refC;
    if (separation <= 0.0f)
    {
        m->contacts[cp] = incidentFace[0];
        m->penetration = -separation;
        ++cp;
    }
    else
    {
        m->penetration = 0;
    }

    separation = Dot(refFaceNormal, incidentFace[1]) - refC;
    if (separation <= 0.0f)
    {
        m->contacts[cp] = incidentFace[1];

        m->penetration += -separation;
        ++cp;

        m->penetration /= static_cast<float>(cp);
    }

    m->contact_count = cp;
}
