#pragma once
#include "Physics2D/collider2D.h"

class BoxCollider2D : public Collider2D
{
public:
    BoxCollider2D() : Collider2D(Type::Box) {}

    void SetCenter(const Vector2& c) { m_center = c; }
    void SetHalfExtents(const Vector2& he) { m_halfExtents = he; }

    Vector2 GetCenter() const override { return m_center; }
    Vector2 GetHalfExtents() const { return m_halfExtents; }

    float Left()   const { return m_center.x - m_halfExtents.x; }
    float Right()  const { return m_center.x + m_halfExtents.x; }
    float Top()    const { return m_center.y - m_halfExtents.y; }
    float Bottom() const { return m_center.y + m_halfExtents.y; }

private:
    Vector2 m_center{ 0.0f, 0.0f };
    Vector2 m_halfExtents{ 0.5f, 0.5f };
};