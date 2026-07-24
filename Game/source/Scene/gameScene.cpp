#include "Scene/gameScene.h"
#include "Scene/sceneManager.h"
#include "Scene/resultScene.h"
#include "System/textureManager.h"
#include "System/input.h"
#include "Engine/SpriteRenderer.h"
#include "Object/player.h"
#include <memory>
#include <windows.h>

void GameScene::Initialize()
{
    if (m_tm) m_tm->Load("game_bg", "texture/game.png");

    // ‚±‚ê‚ª‚â‚è‚½‚¢Œ`
    m_objectManager.AddGameObject<Player>(m_tm);
}

void GameScene::Update(float dt)
{
    m_objectManager.UpdateAll(dt);
    m_objectManager.RemoveInactive();

    if (Input::GetKeyDown('R'))
    {
        m_sm->ChangeScene(std::make_unique<ResultScene>(m_sm, m_tm));
    }
}

void GameScene::Draw(SpriteRenderer& renderer)
{
   

    m_objectManager.DrawAll(renderer);
}

void GameScene::Finalize()
{
    m_objectManager.Clear();
}