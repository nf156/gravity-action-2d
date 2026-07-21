#pragma once
#include "Math/Vector2.h"

struct CollisionManifold
{
    bool hit = false;
    Vector2 normal{ 0.0f, 0.0f };   // A -> B •ûŒü
    float penetration = 0.0f;     // ‚ß‚è‚İ—Ê
};