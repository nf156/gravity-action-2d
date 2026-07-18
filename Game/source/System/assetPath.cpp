#include "System/assetPath.h"

std::string AssetPath::s_basePath = "assets/";

void AssetPath::SetBasePath(const std::string& basePath)
{
    s_basePath = EnsureTrailingSlash(NormalizeSlashes(basePath));
}

const std::string& AssetPath::GetBasePath()
{
    return s_basePath;
}

std::string AssetPath::Resolve(const std::string& relativePath)
{
    if (relativePath.empty())
        return s_basePath;

    std::string rel = NormalizeSlashes(relativePath);

    // ‚·‚Å‚Éâ‘ÎƒpƒX‚Á‚Û‚¢ê‡‚Í‚»‚Ì‚Ü‚Ü•Ô‚·
    // —á: "C:/...", "D:/..."
    if (rel.size() >= 3 &&
        ((rel[0] >= 'A' && rel[0] <= 'Z') || (rel[0] >= 'a' && rel[0] <= 'z')) &&
        rel[1] == ':' && rel[2] == '/')
    {
        return rel;
    }

    // æ“ª‚Ì "./" ‚ðœ‹Ž
    if (rel.size() >= 2 && rel[0] == '.' && rel[1] == '/')
    {
        rel = rel.substr(2);
    }

    return s_basePath + rel;
}

std::string AssetPath::NormalizeSlashes(const std::string& path)
{
    std::string out = path;
    for (char& c : out)
    {
        if (c == '\\')
            c = '/';
    }
    return out;
}

std::string AssetPath::EnsureTrailingSlash(const std::string& path)
{
    if (path.empty())
        return "";

    if (path.back() == '/')
        return path;

    return path + "/";
}