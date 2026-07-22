#include "Physics2D/PhysicsWorld2D.h"
#include "Physics2D/CollisionManager.h"
#include "Physics2D/CollisionManifold.h"

void PhysicsWorld2D::AddStaticBox(BoxCollider2D* box)
{
    if (!box) return;
    m_staticBoxes.push_back(box);
}

void PhysicsWorld2D::AddDynamic(BodyColliderPair p)
{
    if (!p.body || !p.collider) return;
    m_dynamics.push_back(p);
}

void PhysicsWorld2D::ClearStatics()
{
    m_staticBoxes.clear();
}

void PhysicsWorld2D::ClearDynamics()
{
    m_dynamics.clear();
}

void PhysicsWorld2D::Step(float dt)
{
    for (auto& it : m_dynamics)
    {
        if (!it.body || !it.collider) continue;

        // 1) integrate
        it.body->SetGravity(m_gravity);
        it.body->Update(dt);
        it.collider->SetCenter(it.body->GetPosition());

        // 2) solve vs all static boxes
        for (BoxCollider2D* s : m_staticBoxes)
        {
            if (!s) continue;

            CollisionManifold m = CollisionManager::GetManifoldBoxBox(*it.collider, *s);
            if (!m.hit) continue;

            // push back dynamic
            Vector2 p = it.body->GetPosition();
            p.x -= m.normal.x * m.penetration;
            p.y -= m.normal.y * m.penetration;
            it.body->SetPosition(p);
            it.collider->SetCenter(p);

            // remove inward normal velocity
            Vector2 v = it.body->GetVelocity();
            const float vn = v.x * m.normal.x + v.y * m.normal.y;

            if (vn < 0.0f)
            {
                const float e = it.body->GetRestitution();
                v.x -= (1.0f + e) * vn * m.normal.x;
                v.y -= (1.0f + e) * vn * m.normal.y;
            }

            // --- friction ---
            Vector2 t{ -m.normal.y, m.normal.x };         // 接線
            float vt = v.x * t.x + v.y * t.y;             // 接線方向速度
            float mu = it.body->GetFriction();            // 0..1

            // シンプル減衰モデル
            vt *= (1.0f - mu);

            // v = vn成分 + vt成分 に再構成
            float vn2 = v.x * m.normal.x + v.y * m.normal.y;
            v.x = m.normal.x * vn2 + t.x * vt;
            v.y = m.normal.y * vn2 + t.y * vt;

            it.body->SetVelocity(v);
        }
    }
}