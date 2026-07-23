#pragma once
#include <vector>
#include "Physics2D/Rigidbody2D.h"
#include "Physics2D/BoxCollider2D.h"

struct BodyColliderPair
{
    Rigidbody2D* body = nullptr;
    BoxCollider2D* collider = nullptr;
};

class PhysicsWorld2D
{
public:
    void SetGravity(float g) { m_gravity = g; }
    void AddStaticBox(BoxCollider2D* box);
    void AddDynamic(BodyColliderPair p);

    // ’Ç‰Á
    void RemoveDynamic(Rigidbody2D* body, BoxCollider2D* collider);

    void ClearStatics();
    void ClearDynamics();
    void Step(float dt);

private:
    float m_gravity = 980.0f;
    std::vector<BoxCollider2D*> m_static;
    std::vector<BodyColliderPair> m_dynamic;
};