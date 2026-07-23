//#pragma once
//#include <vector>
//#include <functional>
//#include "Physics2D/collider.h"
//#include "Physics2D/CollisionManifold.h"
//
//class BoxCollider2D;
//class CircleCollider2D;
//
//class CollisionManager
//{
//public:
//    using CollisionCallback = std::function<void(Collider*, Collider*)>;
//
//    void Clear();
//    void AddCollider(Collider* c);
//    void CheckAll(const CollisionCallback& onCollision);
//
//    static bool Overlap(const Collider& a, const Collider& b);
//    static bool OverlapBoxBox(const BoxCollider2D& a, const BoxCollider2D& b);
//    static bool OverlapCircleCircle(const CircleCollider2D& a, const CircleCollider2D& b);
//    static bool OverlapBoxCircle(const BoxCollider2D& box, const CircleCollider2D& circle);
//    static CollisionManifold GetManifoldBoxBox(const BoxCollider2D& a, const BoxCollider2D& b);
//
//private:
//    std::vector<Collider*> m_colliders;
//};