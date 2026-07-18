#pragma once
#include "Object/gameObject.h"

class TestObject : public GameObject
{
public:
    void Initialize() override;
    void Update(float dt) override;
    void Draw() override;
    void Finalize() override;
};