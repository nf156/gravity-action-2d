#include "System/debugLog.h"
#include <windows.h>
#include <iostream>

namespace
{
    void Output(const std::string& prefix, const std::string& message)
    {
        const std::string text = prefix + message + "\n";

        // Visual Studioの出力ウィンドウへ
        OutputDebugStringA(text.c_str());

        // コンソールにも出す（必要なければ消してOK）
        std::cout << text;
    }
}

void DebugLog::Log(const std::string& message)
{
    Output("[Log] ", message);
}

void DebugLog::LogWarning(const std::string& message)
{
    Output("[Warning] ", message);
}

void DebugLog::LogError(const std::string& message)
{
    Output("[Error] ", message);
}