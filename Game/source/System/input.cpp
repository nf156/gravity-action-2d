#include "System/input.h"

bool Input::currKeys[256] = {};
bool Input::prevKeys[256] = {};

void Input::Initialize()
{
    for (int i = 0; i < 256; ++i)
    {
        currKeys[i] = false;
        prevKeys[i] = false;
    }
}

void Input::Update()
{
    for (int i = 0; i < 256; ++i)
    {
        prevKeys[i] = currKeys[i];
        currKeys[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }
}

bool Input::GetKey(int vk)
{
    if (!IsValidKeyCode(vk)) return false;
    return currKeys[vk];
}

bool Input::GetKeyDown(int vk)
{
    if (!IsValidKeyCode(vk)) return false;
    return (currKeys[vk] && !prevKeys[vk]);
}

bool Input::GetKeyUp(int vk)
{
    if (!IsValidKeyCode(vk)) return false;
    return (!currKeys[vk] && prevKeys[vk]);
}

bool Input::IsValidKeyCode(int vk)
{
    return (vk >= 0 && vk < 256);
}