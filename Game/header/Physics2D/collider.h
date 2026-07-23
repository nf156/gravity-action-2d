#pragma once
#include "Physics2D/BoxCollider2D.h"
#include <cmath>

struct Manifold2D
{
    bool hit = false;
    Vector2 normal{ 0,0 };   // A -> B
    float penetration = 0.0f;
};

inline bool OverlapBoxBox(const BoxCollider2D& a, const BoxCollider2D& b)
{
    Vector2 ac = a.GetCenter(), ah = a.GetHalfExtents();
    Vector2 bc = b.GetCenter(), bh = b.GetHalfExtents();

    if (std::fabs(ac.x - bc.x) > (ah.x + bh.x)) return false;
    if (std::fabs(ac.y - bc.y) > (ah.y + bh.y)) return false;
    return true;
}

inline Manifold2D GetManifoldBoxBox(const BoxCollider2D& a, const BoxCollider2D& b)
{
    Manifold2D m{};

    Vector2 ac = a.GetCenter(), ah = a.GetHalfExtents();
    Vector2 bc = b.GetCenter(), bh = b.GetHalfExtents();

    const float dx = bc.x - ac.x;
    const float px = (ah.x + bh.x) - std::fabs(dx);
    if (px <= 0.0f) return m;

    const float dy = bc.y - ac.y;
    const float py = (ah.y + bh.y) - std::fabs(dy);
    if (py <= 0.0f) return m;

    m.hit = true;
    if (px < py)
    {
        m.penetration = px;
        m.normal = (dx >= 0.0f) ? Vector2{ 1,0 } : Vector2{ -1,0 };
    }
    else
    {
        m.penetration = py;
        m.normal = (dy >= 0.0f) ? Vector2{ 0,1 } : Vector2{ 0,-1 };
    }
    return m;
}