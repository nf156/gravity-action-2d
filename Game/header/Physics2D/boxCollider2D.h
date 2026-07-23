#pragma once
#include "Math/Vector2.h"

class BoxCollider2D
{
public:
    void SetCenter(const Vector2& c) { m_center = c; }
    Vector2 GetCenter() const { return m_center; }

    void SetHalfExtents(const Vector2& he) { m_half = he; }
    Vector2 GetHalfExtents() const { return m_half; }

private:
    Vector2 m_center{ 0,0 };
    Vector2 m_half{ 0.5f,0.5f };
};