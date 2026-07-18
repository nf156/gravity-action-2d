#pragma once
#include <windows.h>

class Input
{
public:
    // 起動時に1回
    static void Initialize();

    // 毎フレーム先頭で呼ぶ
    static void Update();

    // 押している間 true
    static bool GetKey(int vk);

    // 押した瞬間の1フレームだけ true
    static bool GetKeyDown(int vk);

    // 離した瞬間の1フレームだけ true
    static bool GetKeyUp(int vk);

private:
    static bool IsValidKeyCode(int vk);

private:
    static bool currKeys[256];
    static bool prevKeys[256];
};