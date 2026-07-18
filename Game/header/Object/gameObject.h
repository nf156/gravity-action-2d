#pragma once
#include "Object/transform2D.h"

class GameObject
{
public:
    GameObject() : m_isActive(true) {}
    virtual ~GameObject() = default;

    // ライフサイクル
    virtual void Initialize() {}
    virtual void Update(float /*dt*/) {}
    virtual void Draw() {}
    virtual void Finalize() {}

    // Active制御
    bool IsActive() const { return m_isActive; }
    void SetActive(bool active) { m_isActive = active; }

    // Transformアクセス
    Transform2D& GetTransform() { return m_transform; }
    const Transform2D& GetTransform() const { return m_transform; }

protected:
    Transform2D m_transform;
    bool m_isActive;
};