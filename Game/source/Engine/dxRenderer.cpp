#include "Engine/dxRenderer.h"
#include <string>

struct VertexPC
{
    float x, y, z;
    float r, g, b, a;
};

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

    return CreateTriangleResources();
}

bool DXRenderer::CreateTriangleResources()
{
    // 1) 頂点バッファ
    VertexPC verts[3] =
    {
        {  0.0f,  0.5f, 0.0f, 1,0,0,1 },
        {  0.5f, -0.5f, 0.0f, 0,1,0,1 },
        { -0.5f, -0.5f, 0.0f, 0,0,1,1 }
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(verts);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA init = {};
    init.pSysMem = verts;

    HRESULT hr = m_device->CreateBuffer(&bd, &init, m_vb.GetAddressOf());
    if (FAILED(hr)) return false;

    // 2) ファイルからシェーダコンパイル
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, psBlob, errBlob;

    UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    hr = D3DCompileFromFile(
        L"shader/basic.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VSMain",
        "vs_5_0",
        compileFlags,
        0,
        vsBlob.GetAddressOf(),
        errBlob.GetAddressOf()
    );
    if (FAILED(hr))
    {
        if (errBlob) OutputDebugStringA((char*)errBlob->GetBufferPointer());
        return false;
    }

    errBlob.Reset();
    hr = D3DCompileFromFile(
        L"shader/basic.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PSMain",
        "ps_5_0",
        compileFlags,
        0,
        psBlob.GetAddressOf(),
        errBlob.GetAddressOf()
    );
    if (FAILED(hr))
    {
        if (errBlob) OutputDebugStringA((char*)errBlob->GetBufferPointer());
        return false;
    }

    // 3) VS/PS作成
    hr = m_device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        m_vs.GetAddressOf()
    );
    if (FAILED(hr)) return false;

    hr = m_device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        m_ps.GetAddressOf()
    );
    if (FAILED(hr)) return false;

    // 4) InputLayout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                 D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = m_device->CreateInputLayout(
        layout,
        _countof(layout),
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        m_inputLayout.GetAddressOf()
    );
    if (FAILED(hr)) return false;

    return true;
}

void DXRenderer::BeginFrame(float r, float g, float b, float a)
{
    const float clearColor[4] = { r, g, b, a };
    m_context->ClearRenderTargetView(m_rtv.Get(), clearColor);
}

void DXRenderer::DrawTriangle()
{
    UINT stride = sizeof(VertexPC);
    UINT offset = 0;
    m_context->IASetVertexBuffers(0, 1, m_vb.GetAddressOf(), &stride, &offset);
    m_context->IASetInputLayout(m_inputLayout.Get());
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_context->VSSetShader(m_vs.Get(), nullptr, 0);
    m_context->PSSetShader(m_ps.Get(), nullptr, 0);

    m_context->Draw(3, 0);
}

void DXRenderer::EndFrame()
{
    m_swapChain->Present(1, 0);
}

void DXRenderer::Finalize()
{
    m_inputLayout.Reset();
    m_ps.Reset();
    m_vs.Reset();
    m_vb.Reset();

    m_rtv.Reset();
    m_swapChain.Reset();
    m_context.Reset();
    m_device.Reset();
}