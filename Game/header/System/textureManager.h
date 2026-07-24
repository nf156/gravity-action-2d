#pragma once
#include <string>
#include <unordered_map>

class SpriteRenderer;

// DirectXTK不使用版：
// 実ロードは SpriteRenderer::LoadTexture()、
// ここは key -> ファイルパス管理だけ行う
class TextureManager
{
public:
    bool Load(const std::string& key, const std::string& relativePath); // 例: "title_bg", "texture/title.png"
    bool Bind(SpriteRenderer& renderer, const std::string& key) const;  // 描画前にセット

private:
    std::unordered_map<std::string, std::string> m_pathByKey;
};