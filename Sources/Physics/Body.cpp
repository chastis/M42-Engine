#include "core.hpp"

Body::Body(Shape* shape_, std::uint32_t x, std::uint32_t y)
    : shape(shape_->Clone())
{
    shape->body = this;
    position.Set(static_cast<float>(x), static_cast<float>(y));
    velocity.Set(0.f, 0.f);
    angularVelocity = 0.f;
    torque = 0.f;
    orient = Math::Random(-Math::PI, Math::PI);
    force.Set(0.f, 0.f);
    staticFriction = 0.5f;
    dynamicFriction = 0.3f;
    restitution = 0.2f;
    shape->Initialize();
    r = Math::Random(0.2f, 1.0f);
    g = Math::Random(0.2f, 1.0f);
    b = Math::Random(0.2f, 1.0f);
}

void Body::SetOrient(float radians)
{
    orient = radians;
    shape->SetOrient(radians);
}

void Body::ApplyForce(const Vec2& f)
{
    force += f;
}

void Body::ApplyImpulse(const Vec2& impulse, const Vec2& contactVector)
{
    velocity += im * impulse;
    angularVelocity += iI * Cross(contactVector, impulse);
}

void Body::SetStatic()
{
    I = 0.f;
    iI = 0.f;
    m = 0.f;
    im = 0.f;
}