#include "Scene/titleScene.h"
#include "Scene/sceneManager.h"
#include "Scene/gameScene.h"
#include "System/textureManager.h"
#include "System/input.h"
#include "Engine/SpriteRenderer.h"
#include "Object/player.h"
#include "Object/titleBG.h"
#include <memory>
#include <windows.h>
void TitleScene::Initialize()
{
    m_objectManager.AddGameObject<TitleBG>(m_tm);

}

void TitleScene::Update(float dt)
{
    m_objectManager.UpdateAll(dt);
    m_objectManager.RemoveInactive();
    if (Input::GetKeyDown(VK_RETURN))
    {
        m_sm->ChangeScene(std::make_unique<GameScene>(m_sm, m_tm));
    }
}

void TitleScene::Draw(SpriteRenderer& renderer)
{
    // 初回だけロード（簡易運用）
    static bool loaded = false;
    if (!loaded && m_tm)
    {
        m_tm->Load(renderer, "title_bg", "texture/title.png");
        loaded = true;
    }

    m_objectManager.DrawAll(renderer);
    renderer.Flush(); // フレームで1回
}

void TitleScene::Finalize()
{
    m_objectManager.Clear();
}
