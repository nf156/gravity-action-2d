#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Object/gameObject.h"

class ObjectManager
{
public:
    void Add(const std::shared_ptr<GameObject>& obj)
    {
        if (!obj) return;
        obj->Initialize();
        m_objects.push_back(obj);
    }

    void UpdateAll(float dt)
    {
        for (auto& obj : m_objects)
        {
            if (!obj) continue;
            if (!obj->IsActive()) continue;
            obj->Update(dt);
        }
    }

    void DrawAll()
    {
        for (auto& obj : m_objects)
        {
            if (!obj) continue;
            if (!obj->IsActive()) continue;
            obj->Draw();
        }
    }

    void RemoveInactive()
    {
        for (auto& obj : m_objects)
        {
            if (obj && !obj->IsActive())
            {
                obj->Finalize();
            }
        }

        m_objects.erase(
            std::remove_if(m_objects.begin(), m_objects.end(),
                [](const std::shared_ptr<GameObject>& obj)
                {
                    return (!obj || !obj->IsActive());
                }),
            m_objects.end());
    }

    void Clear()
    {
        for (auto& obj : m_objects)
        {
            if (obj) obj->Finalize();
        }
        m_objects.clear();
    }

    size_t GetCount() const { return m_objects.size(); }

private:
    std::vector<std::shared_ptr<GameObject>> m_objects;
};