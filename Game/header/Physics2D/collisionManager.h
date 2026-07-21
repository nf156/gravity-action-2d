#pragma once
#include <vector>
#include <functional>
#include "Physics2D/collider2D.h"
#include "Physics2D/CollisionManifold.h"

class BoxCollider2D;
class CircleCollider2D;

class CollisionManager
{
public:
    using CollisionCallback = std::function<void(Collider2D*, Collider2D*)>;

    void Clear();
    void AddCollider(Collider2D* c);
    void CheckAll(const CollisionCallback& onCollision);

    static bool Overlap(const Collider2D& a, const Collider2D& b);
    static bool OverlapBoxBox(const BoxCollider2D& a, const BoxCollider2D& b);
    static bool OverlapCircleCircle(const CircleCollider2D& a, const CircleCollider2D& b);
    static bool OverlapBoxCircle(const BoxCollider2D& box, const CircleCollider2D& circle);
    static CollisionManifold GetManifoldBoxBox(const BoxCollider2D& a, const BoxCollider2D& b);

private:
    std::vector<Collider2D*> m_colliders;
};