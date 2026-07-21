#pragma once
#include "Math/Vector2.h"

class Rigidbody2D
{
public:
    void SetPosition(const Vector2& p) { m_position = p; }
    Vector2 GetPosition() const { return m_position; }

    void SetVelocity(const Vector2& v) { m_velocity = v; }
    Vector2 GetVelocity() const { return m_velocity; }

    void SetUseGravity(bool v) { m_useGravity = v; }
    bool GetUseGravity() const { return m_useGravity; }

    void SetGravity(float g) { m_gravity = g; } // +Y‰ºŒü‚«‘z’è
    float GetGravity() const { return m_gravity; }

    void Update(float dt)
    {
        if (m_useGravity)
            m_velocity.y += m_gravity * dt;

        m_position.x += m_velocity.x * dt;
        m_position.y += m_velocity.y * dt;
    }

private:
    Vector2 m_position{ 0.0f, 0.0f };
    Vector2 m_velocity{ 0.0f, 0.0f };
    float m_gravity = 980.0f; // px/s^2
    bool m_useGravity = true;
};