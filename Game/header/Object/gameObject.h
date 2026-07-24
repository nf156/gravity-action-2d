#pragma once
#include "Math/Vector2.h"
class SpriteRenderer;

class GameObject
{
public:
    virtual ~GameObject() = default;
    virtual void Initialize() {}
    virtual void Update(float dt) {}
    virtual void Draw(SpriteRenderer& renderer) {} // ïœçX
    virtual void Finalize() {}

    bool IsActive() const { return m_active; }
    void SetActive(bool v) { m_active = v; }

    int GetLayer() const { return m_layer; }
    void SetLayer(int layer) { m_layer = layer; }

protected:
    struct Transform
    {
        Vector2 position{ 0, 0 };
    } m_transform;

private:
    bool m_active = true;
    int  m_layer = 0;
};