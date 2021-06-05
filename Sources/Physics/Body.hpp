#pragma once 

#include "Utility/CoreMath.hpp"

using Math::Vec2;

struct Shape;

struct Body
{
    Body(Shape* shape_, uint32_t x, uint32_t y);

    void ApplyForce(const Vec2& f);
    void ApplyImpulse(const Vec2& impulse, const Vec2& contactVector);

    void SetStatic();
    void SetOrient(float radians);

    Vec2 position;
    Vec2 velocity;

    float angularVelocity = 0.f;
    float torque = 0.f;
    float orient = 0.f; // in radians

    Vec2 force;

    float I = 0.f; 
    float iI = 0.f;
    float m = 0.f; 
    float im = 0.f;

    float staticFriction = 0.f;
    float dynamicFriction = 0.f;
    float restitution = 0.f;

    Shape* shape = nullptr;

    float r = 0.f, g = 0.f, b = 0.f;
};
