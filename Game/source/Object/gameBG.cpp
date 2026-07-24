#include "Object/gameBG.h"
#include "System/textureManager.h"
#include "Engine/SpriteRenderer.h"

GameBG::GameBG(TextureManager* tm)
    : m_tm(tm)
{
}

void GameBG::Initialize()
{
    m_rigidbody.SetPosition({ 0.0f, 0.0f });
}

void GameBG::Finalize()
{
}

void GameBG::Update(float dt)
{
    (void)dt;
}

void GameBG::Draw(SpriteRenderer& renderer)
{
    if (m_tm) m_tm->Bind(renderer, "game_bg");
    Vector2 bg = m_rigidbody.GetPosition();
    renderer.Submit(bg.x, bg.y, m_w, m_h, 1, 1, 1, 1, -1, 0, 0.0f);
}