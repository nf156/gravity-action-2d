#pragma once
#include <memory>
#include "Scene/scene.h"

enum class SceneType
{
    Title,
    Game,
    Result
};

class SceneManager
{
public:
    void ChangeScene(std::unique_ptr<Scene> next)
    {
        if (m_current) m_current->Finalize();
        m_current = std::move(next);
        if (m_current) m_current->Initialize();
    }

    void Update(float dt)
    {
        if (m_current) m_current->Update(dt);
    }

    void Draw(SpriteRenderer& renderer)
    {
        if (m_current) m_current->Draw(renderer);
    }

private:
    std::unique_ptr<Scene> m_current;
};