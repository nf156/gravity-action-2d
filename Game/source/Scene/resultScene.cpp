#include "Scene/resultScene.h"
#include "Scene/sceneManager.h"
#include "Scene/titleScene.h"
#include "System/textureManager.h"
#include "System/input.h"
#include "Engine/SpriteRenderer.h"
#include <memory>
#include <windows.h>

void ResultScene::Initialize()
{
    if (m_tm) m_tm->Load("result_bg", "texture/result.png");
}

void ResultScene::Update(float)
{
	if (Input::GetKeyDown(VK_RETURN))
    {
        m_sm->ChangeScene(std::make_unique<TitleScene>(m_sm, m_tm));
    }
}

void ResultScene::Draw(SpriteRenderer& renderer)
{
    if (m_tm) m_tm->Bind(renderer, "result_bg");
    renderer.Submit(0, 0, 1280, 720, 1.0f, 1.0f, 1.0f, 1.0f, 0, 0, 0.0f);
}

void ResultScene::Finalize()
{
}
