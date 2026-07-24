#pragma once
#include "Object/gameObject.h"
#include "Physics2D/rigidbody2D.h"

class TextureManager;
class SpriteRenderer;

class Player : public GameObject
{
public:
    explicit Player(TextureManager* tm);

    void Initialize() override;
    void Update(float dt) override;
    void Draw(SpriteRenderer& renderer) override;
    void Finalize() override;

private:
    TextureManager* m_tm = nullptr;
	Rigidbody2D m_rigidbody; // ’Ç‰Á: Rigidbody2D ‚ğƒƒ“ƒo‚Æ‚µ‚Ä‚Â

    float m_w = 64.0f;
    float m_h = 64.0f;
    float m_moveSpeed = 360.0f;
    float m_jumpPower = -720.0f; // y+‚ª‰ºŒü‚«‘z’è
    bool  m_grounded = false;
};