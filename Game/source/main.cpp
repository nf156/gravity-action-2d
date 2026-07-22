#include <windows.h>
#include <chrono>

// 物理
#include "Physics2D/PhysicsWorld2D.h"
#include "Physics2D/Rigidbody2D.h"
#include "Physics2D/BoxCollider2D.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"GravityGameWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Gravity Action 2D",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        nullptr, nullptr, hInstance, nullptr
    );
    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    // =========================
    // Physics setup
    // =========================
    PhysicsWorld2D world;
    world.SetGravity(980.0f);

    // 静的コライダ（床・壁など）
    BoxCollider2D groundBox;
    groundBox.SetCenter({ 640.0f, 680.0f });
    groundBox.SetHalfExtents({ 640.0f, 20.0f });

    BoxCollider2D wallLeft;
    wallLeft.SetCenter({ 20.0f, 360.0f });
    wallLeft.SetHalfExtents({ 20.0f, 360.0f });

    BoxCollider2D wallRight;
    wallRight.SetCenter({ 1260.0f, 360.0f });
    wallRight.SetHalfExtents({ 20.0f, 360.0f });

    world.AddStaticBox(&groundBox);
    world.AddStaticBox(&wallLeft);
    world.AddStaticBox(&wallRight);

    // 動的ボディ（プレイヤー想定）
    Rigidbody2D playerBody;
    playerBody.SetPosition({ 220.0f, 120.0f });
    playerBody.SetVelocity({ 120.0f, 0.0f }); // 横移動テスト
    playerBody.SetUseGravity(true);

    BoxCollider2D playerBox;
    playerBox.SetCenter(playerBody.GetPosition());
    playerBox.SetHalfExtents({ 24.0f, 24.0f });

    world.AddDynamic({ &playerBody, &playerBox });

    // =========================
    // Fixed timestep
    // =========================
    const float fixedDt = 1.0f / 60.0f;
    float accumulator = 0.0f;

    using clock = std::chrono::steady_clock;
    auto prev = clock::now();

    MSG msg = {};
    bool running = true;

    while (running)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (!running) break;

        auto now = clock::now();
        float frameDt = std::chrono::duration<float>(now - prev).count();
        prev = now;

        // 暴走防止
        if (frameDt > 0.25f) frameDt = 0.25f;

        accumulator += frameDt;

        // 物理更新（固定刻み）
        while (accumulator >= fixedDt)
        {
            world.Step(fixedDt);
            accumulator -= fixedDt;
        }

        // デバッグ表示（任意）
        Vector2 p = playerBody.GetPosition();
        wchar_t title[128];
        wsprintf(title, L"Gravity Action 2D  x=%.1f y=%.1f", p.x, p.y);
        SetWindowText(hwnd, title);

        // =========================
        // Render
        // =========================
        // ここに既存描画処理を呼ぶ
        // 例:
        // renderer.Begin();
        // renderer.Draw(... playerBox ...);
        // renderer.Draw(... groundBox ...);
        // renderer.End();
    }

    return 0;
}