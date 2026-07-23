#include "Physics2D/PhysicsWorld2D.h"
#include "Physics2D/Collider.h"
#include <algorithm>
#include <cmath>

void PhysicsWorld2D::AddStaticBox(BoxCollider2D* box)
{
    if (box) m_static.push_back(box);
}

void PhysicsWorld2D::AddDynamic(BodyColliderPair p)
{
    if (p.body && p.collider) m_dynamic.push_back(p);
}

// 追加: 動的ペアの解除
void PhysicsWorld2D::RemoveDynamic(Rigidbody2D* body, BoxCollider2D* collider)
{
    if (!body || !collider) return;

    m_dynamic.erase(
        std::remove_if(m_dynamic.begin(), m_dynamic.end(),
            [&](const BodyColliderPair& p)
            {
                return p.body == body && p.collider == collider;
            }),
        m_dynamic.end());
}

void PhysicsWorld2D::ClearStatics() { m_static.clear(); }
void PhysicsWorld2D::ClearDynamics() { m_dynamic.clear(); }

void PhysicsWorld2D::Step(float dt)
{
    const int kIterations = 6;

    // --- Sleep tuning ---
    const float kSleepVel2 = 4.0f;       // |v| < 2.0 px/s 相当（v^2）
    const float kSleepTime = 0.35f;      // この秒数静かなら sleep
    const float kWakeVel2 = 25.0f;      // |v| > 5.0 px/s で wake

    // 1) integrate
    for (auto& d : m_dynamic)
    {
        Vector2 v = d.body->GetVelocity();
        float v2 = v.x * v.x + v.y * v.y;

        // 強く動いているならwake
        if (d.body->IsSleeping() && v2 > kWakeVel2)
            d.body->SetSleeping(false);

        if (!d.body->IsSleeping())
        {
            d.body->SetGravity(m_gravity);
            d.body->Integrate(dt);
            d.collider->SetCenter(d.body->GetPosition());
        }
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

                // 接触したらwake（sleep中でも位置補正は必要）
                if (d.body->IsSleeping()) d.body->SetSleeping(false);

                // positional correction
                Vector2 p = d.body->GetPosition();
                p.x -= m.normal.x * m.penetration;
                p.y -= m.normal.y * m.penetration;
                d.body->SetPosition(p);
                d.collider->SetCenter(p);

                // velocity resolve
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

                if (m.normal.y != 0.0f && v.y > 0.0f) v.y = 0.0f;
                d.body->SetVelocity(v);
            }
        }

        // -------- dynamic vs dynamic --------
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

                // 接触でwake
                if (a.body->IsSleeping()) a.body->SetSleeping(false);
                if (b.body->IsSleeping()) b.body->SetSleeping(false);

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

                Vector2 va = a.body->GetVelocity();
                Vector2 vb = b.body->GetVelocity();
                Vector2 rv{ vb.x - va.x, vb.y - va.y };

                const float vn = rv.x * m.normal.x + rv.y * m.normal.y;
                if (vn >= 0.0f) continue;

                const float e = (a.body->GetRestitution() + b.body->GetRestitution()) * 0.5f;
                const float jImpulse = -(1.0f + e) * vn * 0.5f;
                Vector2 imp{ m.normal.x * jImpulse, m.normal.y * jImpulse };

                va.x -= imp.x; va.y -= imp.y;
                vb.x += imp.x; vb.y += imp.y;

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

    // 3) sleep判定（最後にまとめて）
    for (auto& d : m_dynamic)
    {
        Vector2 v = d.body->GetVelocity();
        float v2 = v.x * v.x + v.y * v.y;

        if (v2 < kSleepVel2)
        {
            float t = d.body->GetSleepTimer() + dt;
            d.body->SetSleepTimer(t);

            if (t >= kSleepTime)
            {
                d.body->SetSleeping(true);
                d.body->SetVelocity({ 0.0f, 0.0f }); // 完全停止
            }
        }
        else
        {
            d.body->SetSleepTimer(0.0f);
            d.body->SetSleeping(false);
        }
    }
}