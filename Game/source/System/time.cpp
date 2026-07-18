#include "System/time.h"

Time::clock::time_point Time::startTime{};
Time::clock::time_point Time::prevTime{};

float Time::deltaTime = 0.0f;
float Time::unscaledDeltaTime = 0.0f;
float Time::timeScale = 1.0f;

int Time::fps = 0;
int Time::fpsFrameCount = 0;
float Time::fpsElapsed = 0.0f;

void Time::Initialize()
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

void Time::Update()
{
    auto now = clock::now();
    std::chrono::duration<float> frame = now - prevTime;
    prevTime = now;

    unscaledDeltaTime = frame.count();

    if (unscaledDeltaTime > 0.25f)
        unscaledDeltaTime = 0.25f;

    deltaTime = unscaledDeltaTime * timeScale;

    fpsFrameCount++;
    fpsElapsed += unscaledDeltaTime;
    if (fpsElapsed >= 1.0f)
    {
        fps = fpsFrameCount;
        fpsFrameCount = 0;
        fpsElapsed = 0.0f;
    }
}

float Time::DeltaTime() { return deltaTime; }
float Time::UnscaledDeltaTime() { return unscaledDeltaTime; }

float Time::TimeSinceStartup()
{
    std::chrono::duration<float> elapsed = clock::now() - startTime;
    return elapsed.count();
}

void Time::SetTimeScale(float scale)
{
    if (scale < 0.0f) scale = 0.0f;
    timeScale = scale;
}

float Time::TimeScale() { return timeScale; }
int Time::Fps() { return fps; }