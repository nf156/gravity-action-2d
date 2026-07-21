#include "Physics2D/CollisionManager.h"
#include "Physics2D/BoxCollider2D.h"
#include "Physics2D/CircleCollider2D.h"
#include <algorithm>

void CollisionManager::Clear()
{
    m_colliders.clear();
}

void CollisionManager::AddCollider(Collider2D* c)
{
    if (!c) return;
    if (!c->IsEnabled()) return;
    m_colliders.push_back(c);
}

void CollisionManager::CheckAll(const CollisionCallback& onCollision)
{
    const size_t n = m_colliders.size();
    for (size_t i = 0; i < n; ++i)
    {
        Collider2D* a = m_colliders[i];
        if (!a || !a->IsEnabled()) continue;

        for (size_t j = i + 1; j < n; ++j)
        {
            Collider2D* b = m_colliders[j];
            if (!b || !b->IsEnabled()) continue;

            if (Overlap(*a, *b))
            {
                if (onCollision) onCollision(a, b);
            }
        }
    }
}

bool CollisionManager::Overlap(const Collider2D& a, const Collider2D& b)
{
    const auto ta = a.GetType();
    const auto tb = b.GetType();

    if (ta == Collider2D::Type::Box && tb == Collider2D::Type::Box)
        return OverlapBoxBox(
            static_cast<const BoxCollider2D&>(a),
            static_cast<const BoxCollider2D&>(b));

    if (ta == Collider2D::Type::Circle && tb == Collider2D::Type::Circle)
        return OverlapCircleCircle(
            static_cast<const CircleCollider2D&>(a),
            static_cast<const CircleCollider2D&>(b));

    if (ta == Collider2D::Type::Box && tb == Collider2D::Type::Circle)
        return OverlapBoxCircle(
            static_cast<const BoxCollider2D&>(a),
            static_cast<const CircleCollider2D&>(b));

    // Circle-Box
    return OverlapBoxCircle(
        static_cast<const BoxCollider2D&>(b),
        static_cast<const CircleCollider2D&>(a));
}

bool CollisionManager::OverlapBoxBox(const BoxCollider2D& a, const BoxCollider2D& b)
{
    if (a.Right() < b.Left())   return false;
    if (a.Left() > b.Right())  return false;
    if (a.Bottom() < b.Top())    return false;
    if (a.Top() > b.Bottom()) return false;
    return true;
}

bool CollisionManager::OverlapCircleCircle(const CircleCollider2D& a, const CircleCollider2D& b)
{
    const Vector2 ca = a.GetCenter();
    const Vector2 cb = b.GetCenter();

    const float dx = ca.x - cb.x;
    const float dy = ca.y - cb.y;
    const float r = a.GetRadius() + b.GetRadius();

    return (dx * dx + dy * dy) <= (r * r);
}

bool CollisionManager::OverlapBoxCircle(const BoxCollider2D& box, const CircleCollider2D& circle)
{
    const Vector2 bc = box.GetCenter();
    const Vector2 he = box.GetHalfExtents();
    const Vector2 cc = circle.GetCenter();

    const float minX = bc.x - he.x;
    const float maxX = bc.x + he.x;
    const float minY = bc.y - he.y;
    const float maxY = bc.y + he.y;

    const float closestX = std::max(minX, std::min(cc.x, maxX));
    const float closestY = std::max(minY, std::min(cc.y, maxY));

    const float dx = cc.x - closestX;
    const float dy = cc.y - closestY;

    return (dx * dx + dy * dy) <= (circle.GetRadius() * circle.GetRadius());
}

CollisionManifold CollisionManager::GetManifoldBoxBox(const BoxCollider2D& a, const BoxCollider2D& b)
{
    CollisionManifold m{};

    const float aLeft = a.Left();
    const float aRight = a.Right();
    const float aTop = a.Top();
    const float aBottom = a.Bottom();

    const float bLeft = b.Left();
    const float bRight = b.Right();
    const float bTop = b.Top();
    const float bBottom = b.Bottom();

    const float overlapX = (aRight < bRight ? aRight : bRight) - (aLeft > bLeft ? aLeft : bLeft);
    const float overlapY = (aBottom < bBottom ? aBottom : bBottom) - (aTop > bTop ? aTop : bTop);

    if (overlapX <= 0.0f || overlapY <= 0.0f) return m;

    m.hit = true;

    const Vector2 ac = a.GetCenter();
    const Vector2 bc = b.GetCenter();

    if (overlapX < overlapY)
    {
        m.penetration = overlapX;
        m.normal = (ac.x < bc.x) ? Vector2{ 1.0f, 0.0f } : Vector2{ -1.0f, 0.0f };
    }
    else
    {
        m.penetration = overlapY;
        m.normal = (ac.y < bc.y) ? Vector2{ 0.0f, 1.0f } : Vector2{ 0.0f, -1.0f };
    }

    return m;
}
