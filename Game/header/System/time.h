#pragma once
#include <chrono>

// フレーム時間管理クラス
// 使い方:
// 1) 起動時に Time::Initialize()
// 2) 毎フレーム先頭で Time::Update()
// 3) 各所で Time::DeltaTime(), Time::Fps() を参照

class Time
{
public:
    // 初期化（アプリ開始時に1回）
    static void Initialize()
    {
        auto now = clock::now();
        startTime = now;
        prevTime = now;
        deltaTime = 0.0f;
        unscaledDeltaTime = 0.0f;
        timeScale = 1.0f;

        fps = 0;
        fpsFrameCount = 0;
        fpsElapsed = 0.0f;
    }

    // 毎フレーム更新
    static void Update()
    {
        auto now = clock::now();

        // 前フレームからの経過秒（timeScale適用前）
        std::chrono::duration<float> frame = now - prevTime;
        prevTime = now;

        unscaledDeltaTime = frame.count();

        // 異常値ガード（ブレーク復帰直後などの巨大dt対策）
        if (unscaledDeltaTime > 0.25f)
            unscaledDeltaTime = 0.25f;

        // timeScale適用後のdeltaTime
        deltaTime = unscaledDeltaTime * timeScale;

        // FPS計測（1秒ごとに更新）
        fpsFrameCount++;
        fpsElapsed += unscaledDeltaTime;
        if (fpsElapsed >= 1.0f)
        {
            fps = fpsFrameCount;
            fpsFrameCount = 0;
            fpsElapsed = 0.0f;
        }
    }

    // 1フレームの経過時間（timeScale適用後）
    static float DeltaTime() { return deltaTime; }

    // 1フレームの経過時間（timeScale適用前）
    static float UnscaledDeltaTime() { return unscaledDeltaTime; }

    // 経過時間（起動からの秒）
    static float TimeSinceStartup()
    {
        std::chrono::duration<float> elapsed = clock::now() - startTime;
        return elapsed.count();
    }

    // タイムスケール設定（0で停止、1で等速、2で2倍速）
    static void SetTimeScale(float scale)
    {
        if (scale < 0.0f) scale = 0.0f;
        timeScale = scale;
    }

    // 現在のタイムスケール取得
    static float TimeScale() { return timeScale; }

    // 現在のFPS
    static int Fps() { return fps; }

private:
    using clock = std::chrono::steady_clock;

    // 起動時刻
    inline static clock::time_point startTime{};

    // 前フレーム時刻
    inline static clock::time_point prevTime{};

    // timeScale適用後 / 前 のdeltaTime
    inline static float deltaTime = 0.0f;
    inline static float unscaledDeltaTime = 0.0f;

    // 時間倍率
    inline static float timeScale = 1.0f;

    // FPS計測用
    inline static int fps = 0;
    inline static int fpsFrameCount = 0;
    inline static float fpsElapsed = 0.0f;
};