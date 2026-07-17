#pragma once
#include <chrono>

class Time
{
public:
    static void Initialize();
    static void Update();

    static float DeltaTime();
    static float UnscaledDeltaTime();
    static float TimeSinceStartup();

    static void SetTimeScale(float scale);
    static float TimeScale();

    static int Fps();

private:
    using clock = std::chrono::steady_clock;

    static clock::time_point startTime;
    static clock::time_point prevTime;

    static float deltaTime;
    static float unscaledDeltaTime;
    static float timeScale;

    static int fps;
    static int fpsFrameCount;
    static float fpsElapsed;
};