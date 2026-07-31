#pragma once
#include "Object/gameObject.h"
#include "Physics2D/rigidbody2D.h"


class TextureManager;
class SpriteRenderer;

class Enemy : public GameObject
{
public:
    explicit Enemy(TextureManager* tm);

    void Initialize() override;
    void Update(float dt) override;
    void Draw(SpriteRenderer& renderer) override;
    void Finalize() override;


private:
    TextureManager* m_tm = nullptr;
    Rigidbody2D m_rigidbody;

    float m_w = 64.0f;
    float m_h = 64.0f;
    float m_speed = 120.0f;
    int   m_dir = 1; // 1:âE, -1:ç∂
    bool  m_grounded = false;

};