#include "Object/testObject.h"
#include "System/debugLog.h"
#include <string>

void TestObject::Initialize()
{
    DebugLog::Log("[TestObject] Initialize");
    m_transform.position.x = 0.0f;
    m_transform.position.y = 0.0f;
}

void TestObject::Update(float dt)
{
    // 毎秒 50 ずつ右へ移動
    m_transform.position.x += 50.0f * dt;

    DebugLog::Log(
        "[TestObject] Update x=" + std::to_string(m_transform.position.x));

    // 200超えたら非アクティブ化（RemoveInactive確認用）
    if (m_transform.position.x > 200.0f)
    {
        SetActive(false);
    }
}

void TestObject::Draw()
{
    // 今は描画処理の代わりにログだけ
    DebugLog::Log("[TestObject] Draw");
}

void TestObject::Finalize()
{
    DebugLog::Log("[TestObject] Finalize");
}