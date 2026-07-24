#include "Scene/gameScene.h"
#include "Scene/sceneManager.h"
#include "Scene/resultScene.h"
#include "System/textureManager.h"
#include "System/input.h"
#include "Engine/SpriteRenderer.h"
#include <memory>
#include <windows.h>

void GameScene::Initialize()
{
    if (m_tm) m_tm->Load("game_bg", "texture/game.png");
}

void GameScene::Update(float)
{
	if (Input::GetKeyDown('R'))
    {
        m_sm->ChangeScene(std::make_unique<ResultScene>(m_sm, m_tm));
    }
}

void GameScene::Draw(SpriteRenderer& renderer)
{
    if (m_tm) m_tm->Bind(renderer, "game_bg");
    renderer.Submit(0, 0, 1280, 720, 1.0f, 1.0f, 1.0f, 1.0f, 0, 0, 0.0f);
}

void GameScene::Finalize()
{
}
