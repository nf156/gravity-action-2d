#pragma once
#include "Scene/scene.h"
#include "Object/objectManager.h"

class SceneManager;
class TextureManager;
class SpriteRenderer;

class ResultScene : public Scene
{
public:
    ResultScene(SceneManager* sm, TextureManager* tm) : m_sm(sm), m_tm(tm) {}

    void Initialize() override;
    void Update(float dt) override;
    void Draw(SpriteRenderer& renderer) override;
	void Finalize() override;

private:
    SceneManager* m_sm = nullptr;
    TextureManager* m_tm = nullptr;

    ObjectManager m_objectManager; // Å© í«â¡
};