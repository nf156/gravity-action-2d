#pragma once
#include <string>

class FileSystem
{
public:
    // ファイルが存在するか
    static bool Exists(const std::string& path);

    // テキスト全体を読み込む（失敗時 false）
    static bool ReadAllText(const std::string& path, std::string& outText);
};