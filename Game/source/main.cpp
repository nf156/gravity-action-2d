#include <windows.h>
#include "System/time.h"
#include "Object/objectManager.h"
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

    if (hwnd == nullptr) return 0;
    ShowWindow(hwnd, nCmdShow);

    Time::Initialize();

    // 追加: オブジェクト基盤
    ObjectManager objectManager;
    objectManager.Add(std::make_shared<TestObject>());

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

        Time::Update();
        float dt = Time::DeltaTime(); // あなたのTime実装名に合わせて必要なら修正

        objectManager.UpdateAll(dt);
        objectManager.DrawAll();
        objectManager.RemoveInactive();

        // Render();
    }

    objectManager.Clear();
    return 0;
}