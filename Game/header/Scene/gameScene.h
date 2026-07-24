#pragma once
#include "Scene/scene.h"

class SceneManager;
class TextureManager;
class SpriteRenderer;

class GameScene : public Scene
{
public:
    GameScene(SceneManager* sm, TextureManager* tm) : m_sm(sm), m_tm(tm) {}

    void Initialize() override;
    void Update(float dt) override;
    void Draw(SpriteRenderer& renderer) override;

private:
    SceneManager* m_sm = nullptr;
    TextureManager* m_tm = nullptr;
};