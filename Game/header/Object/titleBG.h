#pragma once
#include "Object/gameObject.h"
#include "Physics2D/rigidbody2D.h"

class TextureManager;
class SpriteRenderer;

class TitleBG : public GameObject
{
public:
    explicit TitleBG(TextureManager* tm);

    void Initialize() override;
    void Finalize() override;
    void Update(float dt) override;
    void Draw(SpriteRenderer& renderer) override;

private:
    TextureManager* m_tm = nullptr;
    Rigidbody2D m_rigidbody; // ’Ç‰Á: Rigidbody2D ‚ğƒƒ“ƒo‚Æ‚µ‚Ä‚Â
    SpriteRenderer* m_renderer = nullptr; // ’Ç‰Á: SpriteRenderer ‚ğƒƒ“ƒo‚Æ‚µ‚Ä‚Â

    float m_w = 1280.0f;
    float m_h = 720.0f;

};