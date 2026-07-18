#pragma once

class GameObject; // 前方宣言（循環参照回避）

class Component
{
public:
    Component() : m_owner(nullptr), m_isActive(true) {}
    virtual ~Component() = default;

    // ライフサイクル
    virtual void Initialize() {}
    virtual void Update(float /*dt*/) {}
    virtual void Draw() {}
    virtual void Finalize() {}

    // Owner
    void SetOwner(GameObject* owner) { m_owner = owner; }
    GameObject* GetOwner() { return m_owner; }
    const GameObject* GetOwner() const { return m_owner; }

    // Active制御
    bool IsActive() const { return m_isActive; }
    void SetActive(bool active) { m_isActive = active; }

protected:
    GameObject* m_owner;
    bool m_isActive;
};