#include "Scene/resultScene.h"
#include "Scene/sceneManager.h"
#include "Scene/titleScene.h"
#include "System/textureManager.h"
#include "System/input.h"
#include "Engine/SpriteRenderer.h"
#include "Object/player.h"
#include "Object/ResultBG.h"
#include <memory>
#include <windows.h>

void ResultScene::Initialize()
{
    m_objectManager.AddGameObject<ResultBG>(m_tm);
}

void ResultScene::Update(float dt)
{
    m_objectManager.UpdateAll(dt);
    m_objectManager.RemoveInactive();
	if (Input::GetKeyDown(VK_RETURN))
    {
        m_sm->ChangeScene(std::make_unique<TitleScene>(m_sm, m_tm));
    }
}

void ResultScene::Draw(SpriteRenderer& renderer)
{
    // 初回だけロード（簡易運用）
    static bool loaded = false;
    if (!loaded && m_tm)
    {
        m_tm->Load(renderer, "result_bg", "texture/result.png");
        loaded = true;
    }

    m_objectManager.DrawAll(renderer);
    renderer.Flush(); // フレームで1回
}

void ResultScene::Finalize()
{
    m_objectManager.Clear();
}
