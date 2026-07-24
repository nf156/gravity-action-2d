#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Object/gameObject.h"
#include "Engine/SpriteRenderer.h"

class ObjectManager
{
public:
    void Add(const std::shared_ptr<GameObject>& obj)
    {
        if (!obj) return;
        obj->Initialize();
        m_objects.push_back(obj);
    }

    // 追加: Manager::AddGameObject<T>() 風
    template<typename T, typename... Args>
    std::shared_ptr<T> AddGameObject(Args&&... args)
    {
        static_assert(std::is_base_of<GameObject, T>::value, "T must derive GameObject");

        auto obj = std::make_shared<T>(std::forward<Args>(args)...);
        obj->Initialize();
        m_objects.push_back(obj);
        return obj;
    }

    template<typename T>
    std::shared_ptr<T> GetGameObject()
    {
        for (auto& obj : m_objects)
        {
            auto casted = std::dynamic_pointer_cast<T>(obj);
            if (casted && casted->IsActive()) return casted;
        }
        return nullptr;
    }

    template<typename T>
    std::vector<std::shared_ptr<T>> GetGameObjects()
    {
        std::vector<std::shared_ptr<T>> out;
        for (auto& obj : m_objects)
        {
            auto casted = std::dynamic_pointer_cast<T>(obj);
            if (casted && casted->IsActive()) out.push_back(casted);
        }
        return out;
    }

    void UpdateAll(float dt)
    {
        for (auto& obj : m_objects)
        {
            if (!obj || !obj->IsActive()) continue;
            obj->Update(dt);
        }
    }

    // 変更: rendererを受け取る
    void DrawAll(SpriteRenderer& renderer)
    {
        for (auto& obj : m_objects)
        {
            if (!obj || !obj->IsActive()) continue;
            obj->Draw(renderer); // Draw引数あり版
			renderer.Flush(); // 追加: 描画コマンドをフラッシュ
        }
    }

    void RemoveInactive()
    {
        for (auto& obj : m_objects)
        {
            if (obj && !obj->IsActive()) obj->Finalize();
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