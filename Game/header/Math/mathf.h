#pragma once
#include <cmath>      // fabs, sqrt
#include <algorithm>  // std::min, std::max

// UnityのMathfライクな最小ユーティリティ集
namespace Mathf
{
    // 浮動小数の比較に使う微小値
    // 「ほぼ0」を判定するときに使う
    constexpr float Epsilon = 1e-6f;

    // 円周率（必要になったら使えるように先に定義）
    constexpr float PI = 3.14159265358979323846f;

    //==============================
    // 基本関数
    //==============================

    // 絶対値
    inline float Abs(float v)
    {
        return std::fabs(v);
    }

    // 最小値
    inline float Min(float a, float b)
    {
        return (a < b) ? a : b;
    }

    // 最大値
    inline float Max(float a, float b)
    {
        return (a > b) ? a : b;
    }

    // 平方根
    inline float Sqrt(float v)
    {
        return std::sqrt(v);
    }

    //==============================
    // クランプ系
    //==============================

    // v を [minValue, maxValue] の範囲に収める
    inline float Clamp(float v, float minValue, float maxValue)
    {
        if (v < minValue) return minValue;
        if (v > maxValue) return maxValue;
        return v;
    }

    // 0～1に制限するクランプ
    inline float Clamp01(float v)
    {
        return Clamp(v, 0.0f, 1.0f);
    }

    //==============================
    // 補間
    //==============================

    // 線形補間（t=0でa, t=1でb）
    // tは安全のため0～1に丸める
    inline float Lerp(float a, float b, float t)
    {
        t = Clamp01(t);
        return a + (b - a) * t;
    }

    // 逆Lerp：value が [a,b] のどの位置かを 0～1 で返す
    inline float InverseLerp(float a, float b, float value)
    {
        float denom = (b - a);

        // 0割り回避（aとbがほぼ同じ値）
        if (Abs(denom) <= Epsilon)
            return 0.0f;

        return Clamp01((value - a) / denom);
    }

    //==============================
    // 比較
    //==============================

    // ほぼ等しいか（浮動小数の誤差対策）
    inline bool Approximately(float a, float b, float eps = Epsilon)
    {
        return Abs(a - b) <= eps;
    }

    //==============================
    // 角度変換（必要になったとき用）
    //==============================

    // 度 -> ラジアン
    inline float Deg2Rad(float degree)
    {
        return degree * (PI / 180.0f);
    }

    // ラジアン -> 度
    inline float Rad2Deg(float radian)
    {
        return radian * (180.0f / PI);
    }
}