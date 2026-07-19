#pragma once

class Camera2D
{
public:
    void SetPosition(float x, float y);
    void Move(float dx, float dy);

    void SetZoom(float zoom);
    float GetZoom() const;

    float GetX() const;
    float GetY() const;

private:
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_zoom = 1.0f;
};