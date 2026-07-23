#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <chrono>
#include <memory>
#include <vector>
#pragma comment(lib, "d3d11.lib")

#include "Engine/SpriteRenderer.h"
#include "Object/objectManager.h"
#include "Object/testObject.h"
#include "System/assetPath.h"
#include "Physics2D/PhysicsWorld2D.h"
#include "Physics2D/BoxCollider2D.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY: PostQuitMessage(0); return 0;
    default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"ObjPhysTestWindow";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Object + Physics Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        nullptr, nullptr, hInstance, nullptr);
    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);

    RECT rc{}; GetClientRect(hwnd, &rc);
    UINT w = rc.right - rc.left, h = rc.bottom - rc.top;

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = w;
    scd.BufferDesc.Height = h;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    D3D_FEATURE_LEVEL fl{};
    if (FAILED(D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &scd, swapChain.GetAddressOf(),
        device.GetAddressOf(), &fl, context.GetAddressOf())))
        return 0;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    if (FAILED(swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())))) return 0;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
    if (FAILED(device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtv.GetAddressOf()))) return 0;

    D3D11_VIEWPORT vp{};
    vp.Width = static_cast<float>(w);
    vp.Height = static_cast<float>(h);
    vp.MinDepth = 0.0f; vp.MaxDepth = 1.0f;
    context->RSSetViewports(1, &vp);

    SpriteRenderer renderer;
    if (!renderer.Initialize(device.Get(), context.Get())) return 0;

    AssetPath::SetBasePath("asset/");
    const std::string tex = AssetPath::Resolve("texture/white.png");
    std::wstring wtex(tex.begin(), tex.end());
    if (!renderer.LoadTexture(wtex.c_str())) return 0;

    // Physics world + static geometry
    PhysicsWorld2D world;
    world.SetGravity(980.0f);

    BoxCollider2D groundBox;
    groundBox.SetCenter({ 640.0f, 680.0f });
    groundBox.SetHalfExtents({ 640.0f, 20.0f });
    world.AddStaticBox(&groundBox);

    BoxCollider2D wallLeft;
    wallLeft.SetCenter({ 20.0f, 360.0f });
    wallLeft.SetHalfExtents({ 20.0f, 360.0f });
    world.AddStaticBox(&wallLeft);

    BoxCollider2D wallRight;
    wallRight.SetCenter({ 1260.0f, 360.0f });
    wallRight.SetHalfExtents({ 20.0f, 360.0f });
    world.AddStaticBox(&wallRight);

    ObjectManager objectManager;
    std::vector<std::shared_ptr<TestObject>> keepAlive; // éQè∆ï€éùÅiîCà”Åj

    auto Spawn = [&](int count)
        {
            const int cols = 20;
            const float sx = 120.0f;
            const float sy = 60.0f;
            const float gx = 28.0f;
            const float gy = 28.0f;

            size_t base = keepAlive.size();
            keepAlive.reserve(base + count);

            for (int i = 0; i < count; ++i)
            {
                int cx = i % cols;
                int cy = i / cols;
                float x = sx + cx * gx;
                float y = sy + cy * gy;

                auto obj = std::make_shared<TestObject>(&world);
                obj->SetSpawn(x, y, 0.0f, 0.0f);
                objectManager.Add(obj);
                keepAlive.push_back(obj);
            }
        };

    Spawn(50); // èâä˙

    using clock = std::chrono::steady_clock;
    auto prev = clock::now();
    const float fixedDt = 1.0f / 60.0f;
    float accumulator = 0.0f;

    MSG msg{};
    bool running = true;

    while (running)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) { running = false; break; }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (!running) break;

        // TÇ≈í«â¡
        if (GetAsyncKeyState('T') & 1) Spawn(100);

        auto now = clock::now();
        float dt = std::chrono::duration<float>(now - prev).count();
        prev = now;
        if (dt > 0.25f) dt = 0.25f;
        accumulator += dt;

        while (accumulator >= fixedDt)
        {
            world.Step(fixedDt);
            objectManager.UpdateAll(fixedDt);
            objectManager.RemoveInactive();
            accumulator -= fixedDt;
        }

        float clear[4] = { 0.08f, 0.09f, 0.12f, 1.0f };
        context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
        context->ClearRenderTargetView(rtv.Get(), clear);

        renderer.Begin();

        // static boxesï`âÊ
        auto DrawStatic = [&](const BoxCollider2D& b, float r, float g, float bl)
            {
                Vector2 c = b.GetCenter();
                Vector2 he = b.GetHalfExtents();
                renderer.Submit(c.x - he.x, c.y - he.y, he.x * 2.0f, he.y * 2.0f, r, g, bl, 1.0f, 0, 0, 0.0f);
            };
        DrawStatic(groundBox, 0.2f, 0.8f, 0.3f);
        DrawStatic(wallLeft, 0.2f, 0.7f, 0.9f);
        DrawStatic(wallRight, 0.2f, 0.7f, 0.9f);

        objectManager.DrawAll(renderer);

        renderer.Flush();
        renderer.End();
        swapChain->Present(1, 0);

        wchar_t title[128];
        swprintf_s(title, L"Object+Physics | count=%zu (T:+100)", objectManager.GetCount());
        SetWindowTextW(hwnd, title);
    }

    objectManager.Clear();
    renderer.Finalize();
    return 0;
}