#include "System/textureManager.h"
#include "Engine/SpriteRenderer.h"
#include "System/assetPath.h"

bool TextureManager::Load(const std::string& key, const std::string& relativePath)
{
    if (key.empty() || relativePath.empty()) return false;
    m_pathByKey[key] = relativePath; // 上書き可
    return true;
}

bool TextureManager::Bind(SpriteRenderer& renderer, const std::string& key) const
{
    auto it = m_pathByKey.find(key);
    if (it == m_pathByKey.end()) return false;

    // 相対 -> 絶対(ベースパス付き)
    const std::string full = AssetPath::Resolve(it->second);
    const std::wstring w(full.begin(), full.end());

    // 既存のローダーを再利用
    return renderer.LoadTexture(w.c_str());
}