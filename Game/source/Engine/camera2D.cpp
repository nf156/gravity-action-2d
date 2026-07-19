#include "Engine/Camera2D.h"

void Camera2D::SetPosition(float x, float y)
{
    m_x = x;
    m_y = y;
}

void Camera2D::Move(float dx, float dy)
{
    m_x += dx;
    m_y += dy;
}

void Camera2D::SetZoom(float zoom)
{
    // ç≈è¨ÉKÅ[Éh
    if (zoom < 0.01f) zoom = 0.01f;
    m_zoom = zoom;
}

float Camera2D::GetZoom() const { return m_zoom; }
float Camera2D::GetX() const { return m_x; }
float Camera2D::GetY() const { return m_y; }