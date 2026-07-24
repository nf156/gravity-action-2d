#include "System/textureManager.h"
#include "Engine/SpriteRenderer.h"
#include "System/assetPath.h"

bool TextureManager::Load(SpriteRenderer& renderer, const std::string& key, const std::string& relativePath)
{
    if (key.empty() || relativePath.empty()) return false;
    if (m_srvByKey.find(key) != m_srvByKey.end()) return true;

    const std::string full = AssetPath::Resolve(relativePath);
    const std::wstring w(full.begin(), full.end());

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    if (!renderer.CreateTextureSRV(w.c_str(), srv.GetAddressOf())) return false;

    m_srvByKey[key] = srv;
    return true;
}

bool TextureManager::Bind(SpriteRenderer& renderer, const std::string& key) const
{
    auto it = m_srvByKey.find(key);
    if (it == m_srvByKey.end()) return false;
    renderer.SetTexture(it->second.Get());
    return true;
}