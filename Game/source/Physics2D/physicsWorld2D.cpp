#include "Physics2D/PhysicsWorld2D.h"
#include "Physics2D/Collider.h"
#include <algorithm>

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
    const int kIterations = 6;

    // 1) integrate
    for (auto& d : m_dynamic)
    {
        d.body->SetGravity(m_gravity);
        d.body->Integrate(dt);
        d.collider->SetCenter(d.body->GetPosition());
    }

    // 2) solve iterations
    for (int it = 0; it < kIterations; ++it)
    {
        // -------- dynamic vs static --------
        for (auto& d : m_dynamic)
        {
            for (auto* s : m_static)
            {
                if (!s) continue;
                if (!OverlapBoxBox(*d.collider, *s)) continue;

                Manifold2D m = GetManifoldBoxBox(*d.collider, *s);
                if (!m.hit) continue;

                // positional correction (dynamic only)
                Vector2 p = d.body->GetPosition();
                p.x -= m.normal.x * m.penetration;
                p.y -= m.normal.y * m.penetration;
                d.body->SetPosition(p);
                d.collider->SetCenter(p);

                // velocity resolve (dynamic only)
                Vector2 v = d.body->GetVelocity();
                const float vn = v.x * m.normal.x + v.y * m.normal.y;
                if (vn < 0.0f)
                {
                    const float e = d.body->GetRestitution();
                    v.x -= (1.0f + e) * vn * m.normal.x;
                    v.y -= (1.0f + e) * vn * m.normal.y;
                }

                // friction
                Vector2 t{ -m.normal.y, m.normal.x };
                float vt = v.x * t.x + v.y * t.y;
                vt *= (1.0f - d.body->GetFriction());

                const float vn2 = v.x * m.normal.x + v.y * m.normal.y;
                v.x = m.normal.x * vn2 + t.x * vt;
                v.y = m.normal.y * vn2 + t.y * vt;

                // floor stabilize
                if (m.normal.y != 0.0f && v.y > 0.0f) v.y = 0.0f;
                d.body->SetVelocity(v);
            }
        }

        // -------- dynamic vs dynamic (’Ç‰Á) --------
        const size_t n = m_dynamic.size();
        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = i + 1; j < n; ++j)
            {
                auto& a = m_dynamic[i];
                auto& b = m_dynamic[j];
                if (!a.body || !a.collider || !b.body || !b.collider) continue;

                if (!OverlapBoxBox(*a.collider, *b.collider)) continue;
                Manifold2D m = GetManifoldBoxBox(*a.collider, *b.collider);
                if (!m.hit) continue;

                // positional correction (50/50)
                const float halfPen = 0.5f * m.penetration * 0.8f;

                Vector2 pa = a.body->GetPosition();
                pa.x -= m.normal.x * halfPen;
                pa.y -= m.normal.y * halfPen;
                a.body->SetPosition(pa);
                a.collider->SetCenter(pa);

                Vector2 pb = b.body->GetPosition();
                pb.x += m.normal.x * halfPen;
                pb.y += m.normal.y * halfPen;
                b.body->SetPosition(pb);
                b.collider->SetCenter(pb);

                // relative velocity along normal
                Vector2 va = a.body->GetVelocity();
                Vector2 vb = b.body->GetVelocity();
                Vector2 rv{ vb.x - va.x, vb.y - va.y };

                const float vn = rv.x * m.normal.x + rv.y * m.normal.y;
                if (vn >= 0.0f) continue; // separating

                // restitution
                const float e = (a.body->GetRestitution() + b.body->GetRestitution()) * 0.5f;

                // equal mass impulse
                const float jImpulse = -(1.0f + e) * vn * 0.5f;
                Vector2 imp{ m.normal.x * jImpulse, m.normal.y * jImpulse };

                va.x -= imp.x;
                va.y -= imp.y;
                vb.x += imp.x;
                vb.y += imp.y;

                // simple friction on tangent
                Vector2 t{ -m.normal.y, m.normal.x };
                float vta = va.x * t.x + va.y * t.y;
                float vtb = vb.x * t.x + vb.y * t.y;

                const float mu = (a.body->GetFriction() + b.body->GetFriction()) * 0.5f;
                vta *= (1.0f - mu);
                vtb *= (1.0f - mu);

                float vna = va.x * m.normal.x + va.y * m.normal.y;
                float vnb = vb.x * m.normal.x + vb.y * m.normal.y;

                va.x = m.normal.x * vna + t.x * vta;
                va.y = m.normal.y * vna + t.y * vta;
                vb.x = m.normal.x * vnb + t.x * vtb;
                vb.y = m.normal.y * vnb + t.y * vtb;

                a.body->SetVelocity(va);
                b.body->SetVelocity(vb);
            }
        }
    }
}