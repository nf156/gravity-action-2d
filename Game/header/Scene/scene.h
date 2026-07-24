#pragma once
class SpriteRenderer;

class Scene
{
public:
    virtual ~Scene() = default;
    virtual void Initialize() {}
    virtual void Update(float dt) = 0;
    virtual void Draw(SpriteRenderer& renderer) = 0;
    virtual void Finalize() {}
};