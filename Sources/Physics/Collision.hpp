#pragma once

struct Manifold;
struct Body;

void DispatchCollision(Manifold* m, Body* a, Body* b);

void CircleToCircle(Manifold* m, Body* a, Body* b);
void CircleToPolygon(Manifold* m, Body* a, Body* b);
void PolygonToCircle(Manifold* m, Body* a, Body* b);
void PolygonToPolygon(Manifold* m, Body* a, Body* b);
