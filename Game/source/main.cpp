#include <windows.h>
#include <chrono>

// 追加
#include "Physics2D/CollisionManager.h"
#include "Physics2D/BoxCollider2D.h"
#include "Physics2D/CircleCollider2D.h"
#include "Physics2D/rigidbody2D.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
        0, CLASS_NAME, L"Gravity Action 2D", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        nullptr, nullptr, hInstance, nullptr
    );
    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);

    // --- ここから物理テスト用 ---
    BoxCollider2D playerBox;
    playerBox.SetCenter({ 180.0f, 180.0f });
    playerBox.SetHalfExtents({ 32.0f, 32.0f });

    CircleCollider2D enemyCircle;
    enemyCircle.SetCenter({ 220.0f, 200.0f });
    enemyCircle.SetRadius(24.0f);

    CollisionManager collision;
    bool isHit = false;
    // --- ここまで物理テスト用 ---

    using clock = std::chrono::steady_clock;
    auto prev = clock::now();

    bool running = true;
    MSG msg = {};
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
        std::chrono::duration<float> elapsed = now - prev;
        prev = now;
        float dt = elapsed.count();
        (void)dt; // 今は未使用でもOK

        // ===== Update =====
        collision.Clear();
        collision.AddCollider(&playerBox);
        collision.AddCollider(&enemyCircle);

        isHit = false;
        collision.CheckAll([&](Collider2D*, Collider2D*) {
            isHit = true;
            });

        // collision.CheckAll(...) の直後
        SetWindowTextW(hwnd, isHit ? L"Gravity Action 2D - HIT" : L"Gravity Action 2D - NO HIT");

        Rigidbody2D playerBody;
        playerBody.SetPosition({ 180.0f, 120.0f });
        playerBody.SetUseGravity(true);

        // 毎フレーム
        playerBody.Update(dt);
        playerBox.SetCenter(playerBody.GetPosition()); // コライダ中心へ反映

        enemyCircle.SetCenter({ 180.0f, 400.0f }); // 最初は離す
        enemyCircle.SetRadius(24.0f);

        // TODO: isHitで色変更やログ出力など
        // ===== Render =====
        // （今は未実装）
    }

    return 0;
}