#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <chrono>
#include <memory>
#pragma comment(lib, "d3d11.lib")

#include "Engine/SpriteRenderer.h"
#include "System/assetPath.h"
#include "System/textureManager.h"
#include "System/input.h"

#include "Scene/sceneManager.h"
#include "Scene/titleScene.h"

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
    const wchar_t CLASS_NAME[] = L"GameWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Gravity Action 2D",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);

    // =========================
    // D3D11 初期化
    // =========================
    RECT rc{};
    GetClientRect(hwnd, &rc);
    UINT w = rc.right - rc.left;
    UINT h = rc.bottom - rc.top;

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
    {
        return 0;
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    if (FAILED(swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()))))
        return 0;

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
    if (FAILED(device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtv.GetAddressOf())))
        return 0;

    D3D11_VIEWPORT vp{};
    vp.Width = static_cast<float>(w);
    vp.Height = static_cast<float>(h);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    context->RSSetViewports(1, &vp);

	Input::Initialize();

    // =========================
    // Renderer 初期化
    // =========================
    SpriteRenderer renderer;
    if (!renderer.Initialize(device.Get(), context.Get()))
        return 0;

    // アセットの基準パス
    AssetPath::SetBasePath("asset/");

    // =========================
    // TextureManager（DirectXTKなし版）
    // =========================
    TextureManager textureManager;

    // =========================
    // Scene 初期化
    // =========================
    SceneManager sceneManager;
    sceneManager.ChangeScene(std::make_unique<TitleScene>(&sceneManager, &textureManager));

    // =========================
    // Main loop
    // =========================
    using clock = std::chrono::steady_clock;
    auto prev = clock::now();

    MSG msg{};
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

		Input::Update();

        auto now = clock::now();
        float dt = std::chrono::duration<float>(now - prev).count();
        prev = now;
        if (dt > 0.25f) dt = 0.25f;

        sceneManager.Update(dt);
        
        float clear[4] = { 0.08f, 0.09f, 0.12f, 1.0f };
        context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
        context->ClearRenderTargetView(rtv.Get(), clear);

        renderer.Begin();
        sceneManager.Draw(renderer);
        renderer.Flush();
        renderer.End();

        swapChain->Present(1, 0);
    }

    renderer.Finalize();
    return 0;
}