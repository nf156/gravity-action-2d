#include <windows.h>
#include <chrono>

// 追加
#include "Physics2D/CollisionManager.h"
#include "Physics2D/BoxCollider2D.h"
#include "Physics2D/CircleCollider2D.h"
#include "Physics2D/rigidbody2D.h"

#include "Object/testObject.h"

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

    Rigidbody2D playerBody;

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

        collision.CheckAll([&](Collider2D* ca, Collider2D* cb)
            {
                isHit = true;

                if (!ca || !cb) return;
                if (ca->GetType() != Collider2D::Type::Box || cb->GetType() != Collider2D::Type::Box) return;

                auto* a = static_cast<BoxCollider2D*>(ca);
                auto* b = static_cast<BoxCollider2D*>(cb);

                // playerBox / groundBox の順にそろえる（必要に応じて）
                BoxCollider2D* playerCol = a;
                BoxCollider2D* otherCol = b;

                // 例: もし a が地面なら入れ替え（あなたの管理に合わせて条件を書く）
                // if (a == &groundBox) { playerCol = b; otherCol = a; }

                CollisionManifold m = CollisionManager::GetManifoldBoxBox(*playerCol, *otherCol);
                if (!m.hit) return;

                // player を m.normal 逆方向へ押し戻す（A->B法線なのでAを戻すなら -normal）
                Vector2 p = playerBody.GetPosition();
                p.x -= m.normal.x * m.penetration;
                p.y -= m.normal.y * m.penetration;
                playerBody.SetPosition(p);
                playerCol->SetCenter(p);

                // 法線方向速度を除去（めり込み再発防止）
                Vector2 v = playerBody.GetVelocity();
                const float vn = v.x * m.normal.x + v.y * m.normal.y;
                if (vn > 0.0f) return; // すでに離れる向きなら何もしない

                v.x -= vn * m.normal.x;
                v.y -= vn * m.normal.y;
                playerBody.SetVelocity(v);
            });

        

        // collision.CheckAll(...) の直後
        SetWindowTextW(hwnd, isHit ? L"Gravity Action 2D - HIT" : L"Gravity Action 2D - NO HIT");

        playerBody.SetPosition({ 180.0f, 120.0f });
        playerBody.SetUseGravity(true);

        // 毎フレーム
        playerBody.Update(dt);
        playerBox.SetCenter(playerBody.GetPosition()); // コライダ中心へ反映
		playerBox.SetHalfExtents({ 32.0f, 32.0f });

        enemyCircle.SetCenter({ 180.0f, 400.0f }); // 最初は離す
        enemyCircle.SetRadius(24.0f);

        // TODO: isHitで色変更やログ出力など
        // ===== Render =====
        // （今は未実装）
    }

    return 0;
}