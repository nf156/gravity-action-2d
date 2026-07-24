#include "Scene/titleScene.h"
#include "Scene/sceneManager.h"
#include "Scene/gameScene.h"
#include "System/textureManager.h"
#include "System/input.h"
#include "Engine/SpriteRenderer.h"
#include <memory>
#include <windows.h>

void TitleScene::Initialize()
{
    if (m_tm) m_tm->Load("title_bg", "texture/title.png");
}

void TitleScene::Update(float)
{
    if (Input::GetKeyDown(VK_RETURN))
    {
        m_sm->ChangeScene(std::make_unique<GameScene>(m_sm, m_tm));
    }
}

void TitleScene::Draw(SpriteRenderer& renderer)
{
    if (m_tm) m_tm->Bind(renderer, "title_bg");
    renderer.Submit(0, 0, 1280, 720, 1.0f, 1.0f, 1.0f, 1.0f, 0, 0, 0.0f);
}