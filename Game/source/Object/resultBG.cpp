#include "Object/resultBG.h"
#include "System/textureManager.h"
#include "Engine/SpriteRenderer.h"

ResultBG::ResultBG(TextureManager* tm)
    : m_tm(tm)
{
}

void ResultBG::Initialize()
{
    m_rigidbody.SetPosition({ 0.0f, 0.0f });
}

void ResultBG::Finalize()
{
}

void ResultBG::Update(float dt)
{
    (void)dt;
}

void ResultBG::Draw(SpriteRenderer& renderer)
{
    if (m_tm) m_tm->Bind(renderer, "result_bg");
    Vector2 bg = m_rigidbody.GetPosition();
    renderer.Submit(bg.x, bg.y, m_w, m_h, 1, 1, 1, 1, 0, 0.0f);
}