#include "Engine/SpriteRenderer.h"
#include <d3dcompiler.h>
#include <wincodec.h>
#include <vector>
#pragma comment(lib, "windowscodecs.lib")

bool SpriteRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_device = device;
    m_context = context;
    if (!m_device || !m_context) return false;

    if (!CreateShaders()) return false;
    if (!CreateBuffers()) return false;

    // ƒTƒ“ƒvƒ‰ì¬
    D3D11_SAMPLER_DESC samp = {};
    samp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samp.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samp.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samp.MaxLOD = D3D11_FLOAT32_MAX;
    if (FAILED(m_device->CreateSamplerState(&samp, m_sampler.GetAddressOf()))) return false;

    return true;
}

bool SpriteRenderer::CreateShaders()
{
    const char* vsSrc = R"(
cbuffer CB0 : register(b0)
{
    float2 screenSize;
    float2 pad;
};

struct VS_IN
{
    float3 pos : POSITION;
    float4 col : COLOR;
    float2 uv  : TEXCOORD0;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 uv  : TEXCOORD0;
};

VS_OUT VSMain(VS_IN v)
{
    VS_OUT o;
    float2 p = v.pos.xy;
    float2 ndc;
    ndc.x = (p.x / screenSize.x) * 2.0f - 1.0f;
    ndc.y = 1.0f - (p.y / screenSize.y) * 2.0f;
    o.pos = float4(ndc, 0.0f, 1.0f);
    o.col = v.col;
    o.uv = v.uv;
    return o;
}
)";

    const char* psSrc = R"(
Texture2D tex0 : register(t0);
SamplerState smp0 : register(s0);

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 uv  : TEXCOORD0;
};

float4 PSMain(VS_OUT i) : SV_TARGET
{
    float4 t = tex0.Sample(smp0, i.uv);
    return t * i.col;
}
)";

    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, psBlob, errBlob;
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
    flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = D3DCompile(vsSrc, strlen(vsSrc), nullptr, nullptr, nullptr,
        "VSMain", "vs_5_0", flags, 0, vsBlob.GetAddressOf(), errBlob.GetAddressOf());
    if (FAILED(hr)) return false;

    errBlob.Reset();
    hr = D3DCompile(psSrc, strlen(psSrc), nullptr, nullptr, nullptr,
        "PSMain", "ps_5_0", flags, 0, psBlob.GetAddressOf(), errBlob.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_vs.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, m_ps.GetAddressOf());
    if (FAILED(hr)) return false;

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, sizeof(float) * 7,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = m_device->CreateInputLayout(layout, _countof(layout),
        vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
        m_inputLayout.GetAddressOf());
    if (FAILED(hr)) return false;

    return true;
}

bool SpriteRenderer::CreateBuffers()
{
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DYNAMIC;
    vbDesc.ByteWidth = sizeof(Vertex) * 6; // 1quad
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = m_device->CreateBuffer(&vbDesc, nullptr, m_vb.GetAddressOf());
    if (FAILED(hr)) return false;

    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.ByteWidth = sizeof(CB);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = m_device->CreateBuffer(&cbDesc, nullptr, m_cb.GetAddressOf());
    if (FAILED(hr)) return false;

    return true;
}

bool SpriteRenderer::LoadTexture(const wchar_t* path)
{
    Microsoft::WRL::ComPtr<IWICImagingFactory> factory;
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(factory.GetAddressOf()));
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
    hr = factory->CreateDecoderFromFilename(path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf());
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, frame.GetAddressOf());
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
    hr = factory->CreateFormatConverter(converter.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = converter->Initialize(
        frame.Get(),
        GUID_WICPixelFormat32bppBGRA,
        WICBitmapDitherTypeNone,
        nullptr, 0.0, WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) return false;

    UINT w = 0, h = 0;
    converter->GetSize(&w, &h);

    std::vector<unsigned char> pixels(w * h * 4);
    hr = converter->CopyPixels(nullptr, w * 4, (UINT)pixels.size(), pixels.data());
    if (FAILED(hr)) return false;

    D3D11_TEXTURE2D_DESC td = {};
    td.Width = w;
    td.Height = h;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // BGRA
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA init = {};
    init.pSysMem = pixels.data();
    init.SysMemPitch = w * 4;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
    hr = m_device->CreateTexture2D(&td, &init, tex.GetAddressOf());
    if (FAILED(hr)) return false;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
    srvd.Format = td.Format;
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MipLevels = 1;

    hr = m_device->CreateShaderResourceView(tex.Get(), &srvd, m_srv.GetAddressOf());
    if (FAILED(hr)) return false;

    return true;
}

void SpriteRenderer::Begin()
{
    D3D11_MAPPED_SUBRESOURCE mapped = {};
    if (SUCCEEDED(m_context->Map(m_cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
    {
        CB* cb = reinterpret_cast<CB*>(mapped.pData);
        cb->screenSize[0] = m_screenW;
        cb->screenSize[1] = m_screenH;
        cb->pad[0] = cb->pad[1] = 0.0f;
        m_context->Unmap(m_cb.Get(), 0);
    }

    m_context->VSSetConstantBuffers(0, 1, m_cb.GetAddressOf());
    m_context->VSSetShader(m_vs.Get(), nullptr, 0);
    m_context->PSSetShader(m_ps.Get(), nullptr, 0);
    m_context->IASetInputLayout(m_inputLayout.Get());
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_context->PSSetShaderResources(0, 1, m_srv.GetAddressOf());
    m_context->PSSetSamplers(0, 1, m_sampler.GetAddressOf());
}

void SpriteRenderer::Draw(float x, float y, float w, float h, float r, float g, float b, float a)
{
    Vertex v[6] =
    {
        { x,     y,     0, r,g,b,a, 0,0 },
        { x + w, y,     0, r,g,b,a, 1,0 },
        { x,     y + h, 0, r,g,b,a, 0,1 },

        { x + w, y,     0, r,g,b,a, 1,0 },
        { x + w, y + h, 0, r,g,b,a, 1,1 },
        { x,     y + h, 0, r,g,b,a, 0,1 },
    };

    D3D11_MAPPED_SUBRESOURCE mapped = {};
    if (FAILED(m_context->Map(m_vb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) return;
    memcpy(mapped.pData, v, sizeof(v));
    m_context->Unmap(m_vb.Get(), 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_context->IASetVertexBuffers(0, 1, m_vb.GetAddressOf(), &stride, &offset);

    m_context->Draw(6, 0);
}

void SpriteRenderer::End()
{
    ID3D11ShaderResourceView* nullSrv[1] = { nullptr };
    m_context->PSSetShaderResources(0, 1, nullSrv);
}

void SpriteRenderer::Finalize()
{
    m_sampler.Reset();
    m_srv.Reset();
    m_cb.Reset();
    m_vb.Reset();
    m_inputLayout.Reset();
    m_ps.Reset();
    m_vs.Reset();
}