#include "Object/titleBG.h"
#include "System/textureManager.h"
#include "Engine/SpriteRenderer.h"

TitleBG::TitleBG(TextureManager* tm)
    : m_tm(tm)
{
}

void TitleBG::Initialize()
{
    m_rigidbody.SetPosition({ 0.0f, 0.0f });
}

void TitleBG::Finalize()
{
}

void TitleBG::Update(float dt)
{
    (void)dt;
}

void TitleBG::Draw(SpriteRenderer& renderer)
{
    if (m_tm) m_tm->Bind(renderer, "title_bg");
    Vector2 bg = m_rigidbody.GetPosition();
    renderer.Submit(bg.x, bg.y, m_w, m_h, 1, 1, 1, 1, -1, 0, 0.0f);
}