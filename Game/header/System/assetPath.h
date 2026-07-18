#pragma once
#include <string>

class AssetPath
{
public:
    // 例: "assets/" や "C:/proj/Game/assets/"
    static void SetBasePath(const std::string& basePath);

    // 現在のベースパスを取得
    static const std::string& GetBasePath();

    // 相対パスをベースパスと結合して返す
    // 例: Resolve("textures/player.png")
    static std::string Resolve(const std::string& relativePath);

private:
    static std::string NormalizeSlashes(const std::string& path);
    static std::string EnsureTrailingSlash(const std::string& path);

private:
    static std::string s_basePath;
};