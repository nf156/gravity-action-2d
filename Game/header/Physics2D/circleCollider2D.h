#pragma once
#include "Physics2D/collider2D.h"

class CircleCollider2D : public Collider2D
{
public:
    CircleCollider2D() : Collider2D(Type::Circle) {}

    void SetCenter(const Vector2& c) { m_center = c; }
    void SetRadius(float r) { m_radius = (r < 0.0f) ? 0.0f : r; }

    Vector2 GetCenter() const override { return m_center; }
    float GetRadius() const { return m_radius; }

private:
    Vector2 m_center{ 0.0f, 0.0f };
    float m_radius = 0.5f;
};