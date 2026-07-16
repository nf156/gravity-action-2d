#pragma once

#include <cmath>      // sqrtf相当（std::sqrt）を使うため
#include <algorithm>  // std::clamp を使うため

// 2Dベクトル（位置・速度・方向などに使う）
struct Vector2
{
    float x; // X成分
    float y; // Y成分

    //==============================
    // コンストラクタ
    //==============================

    // デフォルトは (0, 0)
    constexpr Vector2() : x(0.0f), y(0.0f) {}

    // 任意の値で初期化
    constexpr Vector2(float x_, float y_) : x(x_), y(y_) {}

    //==============================
    // よく使う定数ベクトル
    //==============================

    static constexpr Vector2 Zero() { return Vector2(0.0f, 0.0f); }  // (0,0)
    static constexpr Vector2 One() { return Vector2(1.0f, 1.0f); }  // (1,1)
    static constexpr Vector2 Up() { return Vector2(0.0f, 1.0f); }  // 上
    static constexpr Vector2 Down() { return Vector2(0.0f, -1.0f); } // 下
    static constexpr Vector2 Left() { return Vector2(-1.0f, 0.0f); } // 左
    static constexpr Vector2 Right() { return Vector2(1.0f, 0.0f); }  // 右

    //==============================
    // 単項演算子（+v, -v）
    //==============================

    constexpr Vector2 operator+() const { return *this; }          // そのまま返す
    constexpr Vector2 operator-() const { return Vector2(-x, -y); } // 符号反転

    //==============================
    // ベクトル同士の演算
    //==============================

    constexpr Vector2 operator+(const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
    constexpr Vector2 operator-(const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }

    // 代入付き演算
    Vector2& operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
    Vector2& operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }

    //==============================
    // スカラー（float）との演算
    //==============================

    constexpr Vector2 operator*(float s) const { return Vector2(x * s, y * s); } // 拡大縮小
    constexpr Vector2 operator/(float s) const { return Vector2(x / s, y / s); } // 等分

    Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
    Vector2& operator/=(float s) { x /= s; y /= s; return *this; }

    //==============================
    // 比較
    //==============================
    // ※ floatの完全一致比較なので、将来は誤差比較関数を追加してもよい
    constexpr bool operator==(const Vector2& rhs) const { return x == rhs.x && y == rhs.y; }
    constexpr bool operator!=(const Vector2& rhs) const { return !(*this == rhs); }

    //==============================
    // 長さ
    //==============================

    // ベクトルの長さ（ノルム）
    float Length() const { return std::sqrt(x * x + y * y); }

    // 長さの2乗（sqrtしないので高速）
    constexpr float LengthSq() const { return x * x + y * y; }

    //==============================
    // 正規化（長さ1にする）
    //==============================

    // 正規化した新しいベクトルを返す（元は変更しない）
    Vector2 Normalized() const
    {
        float len = Length();

        // 0ベクトル付近は割り算できないので(0,0)を返す
        if (len <= 0.000001f) return Vector2::Zero();

        return Vector2(x / len, y / len);
    }

    // 自分自身を正規化する
    void Normalize()
    {
        float len = Length();
        if (len <= 0.000001f)
        {
            x = 0.0f;
            y = 0.0f;
            return;
        }

        x /= len;
        y /= len;
    }

    //==============================
    // ヘルパー（静的関数）
    //==============================

    // 内積（方向の近さ判定などに使う）
    static constexpr float Dot(const Vector2& a, const Vector2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    // 線形補間（t=0でa, t=1でb）
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t)
    {
        // tを0～1に制限（暴走防止）
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        return a + (b - a) * t;
    }

    // 2点間距離
    static float Distance(const Vector2& a, const Vector2& b)
    {
        return (b - a).Length();
    }
};

// 左側スカラー対応（例: 2.0f * v）
inline constexpr Vector2 operator*(float s, const Vector2& v)
{
    return Vector2(v.x * s, v.y * s);
}