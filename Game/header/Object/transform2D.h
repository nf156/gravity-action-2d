#pragma once
#include "Math/vector2.h"  // あなたの実パスに合わせて変更

struct Transform2D
{
    Vector2 position;   // 座標
    float   rotation;   // 角度
    Vector2 scale;      // 拡大率

    Transform2D()
        : position(0.0f, 0.0f)
        , rotation(0.0f)
        , scale(1.0f, 1.0f)
    {
    }
};