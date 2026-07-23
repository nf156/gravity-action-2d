#pragma once
#include "Math/Vector2.h"

class Rigidbody2D
{
public:
    void SetPosition(const Vector2& p) { m_pos = p; }
    Vector2 GetPosition() const { return m_pos; }

    void SetVelocity(const Vector2& v) { m_vel = v; }
    Vector2 GetVelocity() const { return m_vel; }

    void SetUseGravity(bool v) { m_useGravity = v; }
    bool GetUseGravity() const { return m_useGravity; }

    void SetGravity(float g) { m_gravity = g; }

    void SetRestitution(float e) { m_restitution = e; } // 0..1
    float GetRestitution() const { return m_restitution; }

    void SetFriction(float f) { m_friction = f; } // 0..1
    float GetFriction() const { return m_friction; }

    void Integrate(float dt)
    {
        if (m_useGravity) m_vel.y += m_gravity * dt;
        m_pos.x += m_vel.x * dt;
        m_pos.y += m_vel.y * dt;
    }

private:
    Vector2 m_pos{ 0,0 };
    Vector2 m_vel{ 0,0 };
    float m_gravity = 980.0f;
    bool m_useGravity = true;
    float m_restitution = 0.0f;
    float m_friction = 0.2f;
};