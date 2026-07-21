#pragma once
#include <cstdint>
#include "math/vector2.h"   // Šù‘¶‚ÌŒ^‚ðŽg‚¤

class Collider2D
{
public:
    enum class Type : uint8_t
    {
        Box,
        Circle
    };

    explicit Collider2D(Type t) : m_type(t) {}
    virtual ~Collider2D() = default;

    Type GetType() const { return m_type; }

    bool IsEnabled() const { return m_enabled; }
    void SetEnabled(bool v) { m_enabled = v; }

    bool IsTrigger() const { return m_isTrigger; }
    void SetTrigger(bool v) { m_isTrigger = v; }

    virtual Vector2 GetCenter() const = 0;

private:
    Type m_type;
    bool m_enabled = true;
    bool m_isTrigger = false;
};