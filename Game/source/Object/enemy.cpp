#include "Object/enemy.h"
#include "System/textureManager.h"
#include "Engine/SpriteRenderer.h"
#include <windows.h>

Enemy::Enemy(TextureManager* tm)
    : m_tm(tm)
{
}

void Enemy::Initialize()
{
    SetLayer(1);

    // âºÇÃèâä˙à íu
    m_rigidbody.SetPosition({ 200.0f, 520.0f });
    m_rigidbody.SetVelocity({ m_speed, 0.0f });
    m_rigidbody.SetUseGravity(true);
    m_rigidbody.SetGravity(980.0f * 2);
}

void Enemy::Update(float dt)
{
    Vector2 v = m_rigidbody.GetVelocity();
    v.x = m_speed * (float)m_dir;
    m_rigidbody.SetVelocity(v);
    m_rigidbody.Integrate(dt);

    Vector2 p = m_rigidbody.GetPosition();
    Vector2 nv = m_rigidbody.GetVelocity();

    if (p.x < 0.0f)
    {
        p.x = 0.0f;
        m_dir = 1;
    }
    if (p.x > 1280.0f - m_w)
    {
        p.x = 1280.0f - m_w;
        m_dir = -1;
    }

    if (p.y > 520.0f) { p.y = 520.0f; nv.y = 0.0f; m_grounded = true; }

    m_rigidbody.SetPosition(p);
    m_rigidbody.SetVelocity(nv);

}

void Enemy::Draw(SpriteRenderer& renderer)
{
    if (m_tm) m_tm->Bind(renderer, "enemy");

    Vector2 p = m_rigidbody.GetPosition();
    renderer.Submit(p.x, p.y, m_w, m_h, 1, 1, 1, 1, 0.0f,0.0f);
}

void Enemy::Finalize()
{
}