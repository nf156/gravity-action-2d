#include "Object/player.h"
#include "System/textureManager.h"
#include "System/input.h"
#include "Engine/SpriteRenderer.h"
#include <windows.h>

Player::Player(TextureManager* tm)
    : m_tm(tm)
{
}

void Player::Initialize()
{
	SetLayer(1);
    m_rigidbody.SetPosition({ 640.0f, 520.0f });
    m_rigidbody.SetVelocity({ 0.0f, 0.0f });
    m_rigidbody.SetUseGravity(true);
    m_rigidbody.SetGravity(980.0f * 2);
    m_grounded = true;
}

void Player::Update(float dt)
{
    float dir = 0.0f;
    if (Input::GetKey('A') || Input::GetKey(VK_LEFT))  dir -= 1.0f;
    if (Input::GetKey('D') || Input::GetKey(VK_RIGHT)) dir += 1.0f;

    Vector2 v = m_rigidbody.GetVelocity();
    v.x = dir * m_moveSpeed;

    if (m_grounded && (Input::GetKeyDown(VK_SPACE) || Input::GetKeyDown('W') || Input::GetKeyDown(VK_UP)))
    {
        v.y = m_jumpPower;
        m_grounded = false;
    }

    m_rigidbody.SetVelocity(v);
    m_rigidbody.Integrate(dt);

    Vector2 p = m_rigidbody.GetPosition();
    Vector2 nv = m_rigidbody.GetVelocity();

    if (p.y > 520.0f) { p.y = 520.0f; nv.y = 0.0f; m_grounded = true; }
    if (p.x < 0.0f) p.x = 0.0f;
    if (p.x > 1280.0f - m_w) p.x = 1280.0f - m_w;

    m_rigidbody.SetPosition(p);
    m_rigidbody.SetVelocity(nv);
}

void Player::Draw(SpriteRenderer& renderer)
{
    if (m_tm) m_tm->Bind(renderer, "player");
    Vector2 p = m_rigidbody.GetPosition();
    renderer.Submit(p.x, p.y, m_w, m_h, 1, 1, 1, 1, 0, 0.0f);
}

void Player::Finalize()
{
}