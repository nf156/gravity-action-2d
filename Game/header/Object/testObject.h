#pragma once
#include "Object/gameObject.h"
#include "Physics2D/Rigidbody2D.h"
#include "Physics2D/BoxCollider2D.h"
class PhysicsWorld2D;
class SpriteRenderer;

class TestObject : public GameObject
{
public:
    explicit TestObject(PhysicsWorld2D* world);

    void Initialize() override;
    void Update(float dt) override;
    void Draw(SpriteRenderer& renderer) override;
    void Finalize() override;

    // ¶¬’¼Œã‚É‰Šú’l‚ğ“ü‚ê‚é—p
    void SetSpawn(float x, float y, float vx = 0.0f, float vy = 0.0f);

private:
    PhysicsWorld2D* m_world = nullptr;
    bool m_registered = false;

    Rigidbody2D m_body;
    BoxCollider2D m_box;

    float m_w = 24.0f;
    float m_h = 24.0f;
};