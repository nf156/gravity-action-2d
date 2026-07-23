#include "Physics2D/PhysicsWorld2D.h"
#include "Physics2D/Collider.h"

void PhysicsWorld2D::AddStaticBox(BoxCollider2D* box)
{
    if (box) m_static.push_back(box);
}
void PhysicsWorld2D::AddDynamic(BodyColliderPair p)
{
    if (p.body && p.collider) m_dynamic.push_back(p);
}
void PhysicsWorld2D::ClearStatics() { m_static.clear(); }
void PhysicsWorld2D::ClearDynamics() { m_dynamic.clear(); }

void PhysicsWorld2D::Step(float dt)
{
    const int kIterations = 4;

    for (auto& d : m_dynamic)
    {
        d.body->SetGravity(m_gravity);
        d.body->Integrate(dt);
        d.collider->SetCenter(d.body->GetPosition());
    }

    for (int it = 0; it < kIterations; ++it)
    {
        for (auto& d : m_dynamic)
        {
            for (auto* s : m_static)
            {
                if (!s) continue;
                if (!OverlapBoxBox(*d.collider, *s)) continue;

                Manifold2D m = GetManifoldBoxBox(*d.collider, *s);
                if (!m.hit) continue;

                Vector2 p = d.body->GetPosition();
                p.x -= m.normal.x * m.penetration;
                p.y -= m.normal.y * m.penetration;
                d.body->SetPosition(p);
                d.collider->SetCenter(p);

                Vector2 v = d.body->GetVelocity();
                const float vn = v.x * m.normal.x + v.y * m.normal.y;
                if (vn < 0.0f)
                {
                    float e = d.body->GetRestitution();
                    v.x -= (1.0f + e) * vn * m.normal.x;
                    v.y -= (1.0f + e) * vn * m.normal.y;
                }

                Vector2 t{ -m.normal.y, m.normal.x };
                float vt = v.x * t.x + v.y * t.y;
                vt *= (1.0f - d.body->GetFriction());

                float vn2 = v.x * m.normal.x + v.y * m.normal.y;
                v.x = m.normal.x * vn2 + t.x * vt;
                v.y = m.normal.y * vn2 + t.y * vt;

                if (m.normal.y != 0.0f && v.y > 0.0f) v.y = 0.0f; // °ˆÀ’è
                d.body->SetVelocity(v);
            }
        }
    }
}