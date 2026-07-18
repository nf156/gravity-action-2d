#include "Engine/dxRenderer.h"

bool DXRenderer::Initialize(HWND hwnd, int width, int height)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = static_cast<UINT>(width);
    sd.BufferDesc.Height = static_cast<UINT>(height);
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createFlags = 0;
#if defined(_DEBUG)
    createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createFlags,
        levels, _countof(levels), D3D11_SDK_VERSION,
        &sd, m_swapChain.GetAddressOf(), m_device.GetAddressOf(),
        &featureLevel, m_context.GetAddressOf());

    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    if (FAILED(hr)) return false;

    hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_rtv.GetAddressOf());
    if (FAILED(hr)) return false;

    m_context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp = {};
    vp.Width = static_cast<float>(width);
    vp.Height = static_cast<float>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    m_context->RSSetViewports(1, &vp);

    return true;
}

void DXRenderer::BeginFrame(float r, float g, float b, float a)
{
    const float clearColor[4] = { r, g, b, a };
    m_context->ClearRenderTargetView(m_rtv.Get(), clearColor);
}

void DXRenderer::EndFrame()
{
    m_swapChain->Present(1, 0);
}

void DXRenderer::Finalize()
{
    m_rtv.Reset();
    m_swapChain.Reset();
    m_context.Reset();
    m_device.Reset();
}