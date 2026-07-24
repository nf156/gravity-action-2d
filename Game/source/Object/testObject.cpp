#include "Object/testObject.h"
#include "Engine/SpriteRenderer.h"
#include "Physics2D/PhysicsWorld2D.h"

TestObject::TestObject(PhysicsWorld2D* world)
    : m_world(world)
{
}

void TestObject::SetSpawn(float x, float y, float vx, float vy)
{
    m_body.SetPosition({ x, y });
    m_body.SetVelocity({ vx, vy });
    m_body.SetUseGravity(true);
    m_body.SetRestitution(0.0f);
    m_body.SetFriction(0.2f);

    m_box.SetCenter({ x, y });
    m_box.SetHalfExtents({ m_w, m_h });
}

void TestObject::Initialize()
{
    if (!m_world || m_registered) return;

    // ñ¢ê›íËéûÇÃï€åØ
    if (m_box.GetHalfExtents().x <= 0.0f) {
        SetSpawn(100.0f, 100.0f, 0.0f, 0.0f);
    }

    m_world->AddDynamic({ &m_body, &m_box });
    m_registered = true;
}

void TestObject::Update(float)
{
    // ï®óùåãâ ÇTransformÇ…îΩâf
    m_transform.position = m_body.GetPosition();

    // âÊñ äOâ∫Ç»ÇÁè¡Ç∑ÅiîCà”Åj
    if (m_transform.position.y > 2000.0f) {
        SetActive(false);
    }
}

void TestObject::Draw(SpriteRenderer& renderer)
{
    const Vector2 c = m_box.GetCenter();
    const Vector2 he = m_box.GetHalfExtents();
    renderer.Submit(
        c.x - he.x, c.y - he.y,
        he.x * 2.0f, he.y * 2.0f,
        1.0f, 0.9f, 0.3f,
        1, 0, 0.0f
    );
}

void TestObject::Finalize()
{
    if (m_registered && m_world)
    {
        m_world->RemoveDynamic(&m_body, &m_box);
    }
    m_registered = false;
}