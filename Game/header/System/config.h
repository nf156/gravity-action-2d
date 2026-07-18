#pragma once
#include <string>
#include <unordered_map>

class Config
{
public:
    // 読み込み成功で true
    static bool Load(const std::string& path);

    // 値取得（キーが無い/変換失敗時は defaultValue）
    static std::string GetString(const std::string& key, const std::string& defaultValue = "");
    static int         GetInt(const std::string& key, int defaultValue = 0);
    static float       GetFloat(const std::string& key, float defaultValue = 0.0f);
    static bool        GetBool(const std::string& key, bool defaultValue = false);

    static bool HasKey(const std::string& key);
    static void Clear();

private:
    static std::string Trim(const std::string& s);
    static std::string ToLower(std::string s);

private:
    static std::unordered_map<std::string, std::string> s_values;
};