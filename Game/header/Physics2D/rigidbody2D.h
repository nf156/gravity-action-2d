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

    void SetGravity(float g) { m_gravity = g; } // +Y下向き想定
    float GetGravity() const { return m_gravity; }

    void Update(float dt)
    {
        if (m_useGravity)
            m_velocity.y += m_gravity * dt;

        m_position.x += m_velocity.x * dt;
        m_position.y += m_velocity.y * dt;
    }

	// 反発係数の設定（0=跳ねない, 1=完全反射）
    void SetRestitution(float e) { m_restitution = e; }   // 0..1
    float GetRestitution() const { return m_restitution; }

	// 摩擦係数の設定（0..1 推奨）
    void SetFriction(float f) { m_friction = f; }   // 0..1 推奨
    float GetFriction() const { return m_friction; }

private:
    Vector2 m_position{ 0.0f, 0.0f };
    Vector2 m_velocity{ 0.0f, 0.0f };
    float m_gravity = 980.0f; // px/s^2
    bool m_useGravity = true;
    float m_restitution = 0.0f; // 0=跳ねない, 1=完全反射
    float m_friction = 0.2f;
};