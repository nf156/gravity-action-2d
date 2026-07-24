#pragma once
#include <string>
#include <unordered_map>
#include <d3d11.h>
#include <wrl/client.h>

class SpriteRenderer;

class TextureManager
{
public:
    bool Load(SpriteRenderer& renderer, const std::string& key, const std::string& relativePath);
    bool Bind(SpriteRenderer& renderer, const std::string& key) const;

private:
    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_srvByKey;
};