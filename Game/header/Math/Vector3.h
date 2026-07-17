#pragma once
#include <cmath>
#include <algorithm>
#include "mathf.h"

// 3Dベクトル（将来のカメラ・3D拡張・行列計算の基礎）
struct Vector3
{
    float x;
    float y;
    float z;

    //==============================
    // コンストラクタ
    //==============================
    constexpr Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    constexpr Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    //==============================
    // よく使う定数
    //==============================
    static constexpr Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
    static constexpr Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
    static constexpr Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
    static constexpr Vector3 Down() { return Vector3(0.0f, -1.0f, 0.0f); }
    static constexpr Vector3 Left() { return Vector3(-1.0f, 0.0f, 0.0f); }
    static constexpr Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
    static constexpr Vector3 Forward() { return Vector3(0.0f, 0.0f, 1.0f); }
    static constexpr Vector3 Back() { return Vector3(0.0f, 0.0f, -1.0f); }

    //==============================
    // 単項演算子
    //==============================
    constexpr Vector3 operator+() const { return *this; }
    constexpr Vector3 operator-() const { return Vector3(-x, -y, -z); }

    //==============================
    // ベクトル同士
    //==============================
    constexpr Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
    constexpr Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }

    Vector3& operator+=(const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
    Vector3& operator-=(const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }

    //==============================
    // スカラー 
    //==============================
    constexpr Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
    constexpr Vector3 operator/(float s) const { return Vector3(x / s, y / s, z / s); }

    Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
    Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }

    //==============================
    // 比較（完全一致）
    //==============================
    constexpr bool operator==(const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    constexpr bool operator!=(const Vector3& rhs) const { return !(*this == rhs); }

    //==============================
    // 長さ
    //==============================
    float Length() const { return std::sqrt(x * x + y * y + z * z); }
    constexpr float LengthSq() const { return x * x + y * y + z * z; }

    //==============================
    // 正規化
    //==============================
    Vector3 Normalized() const
    {
        float len = Length();
        if (len <= 0.000001f) return Vector3::Zero();
        return Vector3(x / len, y / len, z / len);
    }

    void Normalize()
    {
        float len = Length();
        if (len <= 0.000001f)
        {
            x = y = z = 0.0f;
            return;
        }
        x /= len;
        y /= len;
        z /= len;
    }

    //==============================
    // ヘルパー
    //==============================
    static constexpr float Dot(const Vector3& a, const Vector3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    // 外積（3Dで重要）
    static constexpr Vector3 Cross(const Vector3& a, const Vector3& b)
    {
        return Vector3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

	// 線形補間（0.0～1.0の範囲で補間）
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
    {
		Mathf::Clamp01(t);
        return a + (b - a) * t;
    }

	// 距離（2点間の距離）
    static float Distance(const Vector3& a, const Vector3& b)
    {
        return (b - a).Length();
    }
};

// 左側スカラー対応
inline constexpr Vector3 operator*(float s, const Vector3& v)
{
    return Vector3(v.x * s, v.y * s, v.z * s);
}