#include "Scene/gameScene.h"
#include "Scene/sceneManager.h"
#include "Scene/resultScene.h"
#include "System/textureManager.h"
#include "System/input.h"
#include "Engine/SpriteRenderer.h"
#include "Object/player.h"
#include "Object/gameBG.h"
#include <memory>
#include <windows.h>

void GameScene::Initialize()
{
    // 注意: ここではrenderer参照がないのでテクスチャLoadはしない
    // （起動時などrendererがあるタイミングで事前Loadするか、Scene API拡張が必要）
    m_objectManager.AddGameObject<GameBG>(m_tm);
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
    // 初回だけロード（簡易運用）
    static bool loaded = false;
    if (!loaded && m_tm)
    {
        m_tm->Load(renderer, "game_bg", "texture/game.png");
        m_tm->Load(renderer, "player", "texture/white.png");
        loaded = true;
    }

    m_objectManager.DrawAll(renderer);
    renderer.Flush(); // フレームで1回
}

void GameScene::Finalize()
{
    m_objectManager.Clear();
}