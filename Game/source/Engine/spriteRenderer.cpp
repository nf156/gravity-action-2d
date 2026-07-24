#include "Engine/SpriteRenderer.h"
#include <d3dcompiler.h>
#include <wincodec.h>
#include <vector>
#include <algorithm>
#include <cstring>
#pragma comment(lib, "windowscodecs.lib")

static bool LoadWICToSRV(
    ID3D11Device* device,
    const wchar_t* path,
    ID3D11ShaderResourceView** outSrv)
{
    if (!device || !path || !outSrv) return false;
    *outSrv = nullptr;

    Microsoft::WRL::ComPtr<IWICImagingFactory> factory;
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(factory.GetAddressOf()));
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
    hr = factory->CreateDecoderFromFilename(
        path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad,
        decoder.GetAddressOf());
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
    td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA init = {};
    init.pSysMem = pixels.data();
    init.SysMemPitch = w * 4;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
    hr = device->CreateTexture2D(&td, &init, tex.GetAddressOf());
    if (FAILED(hr)) return false;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
    srvd.Format = td.Format;
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MipLevels = 1;

    hr = device->CreateShaderResourceView(tex.Get(), &srvd, outSrv);
    return SUCCEEDED(hr);
}

bool SpriteRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_device = device;
    m_context = context;
    if (!m_device || !m_context) return false;

    if (!CreateShaders()) return false;
    if (!CreateBuffers()) return false;

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
struct VS_IN { float3 pos:POSITION; float4 col:COLOR; float2 uv:TEXCOORD0; };
struct VS_OUT{ float4 pos:SV_POSITION; float4 col:COLOR; float2 uv:TEXCOORD0; };
VS_OUT VSMain(VS_IN v){
    VS_OUT o;
    float2 ndc;
    ndc.x = (v.pos.x / screenSize.x) * 2.0f - 1.0f;
    ndc.y = 1.0f - (v.pos.y / screenSize.y) * 2.0f;
    o.pos = float4(ndc,0,1);
    o.col = v.col;
    o.uv = v.uv;
    return o;
})";

    const char* psSrc = R"(
Texture2D tex0 : register(t0);
SamplerState smp0 : register(s0);
struct VS_OUT{ float4 pos:SV_POSITION; float4 col:COLOR; float2 uv:TEXCOORD0; };
float4 PSMain(VS_OUT i) : SV_TARGET {
    return tex0.Sample(smp0, i.uv) * i.col;
})";

    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, psBlob, errBlob;
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
    flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = D3DCompile(vsSrc, strlen(vsSrc), nullptr, nullptr, nullptr,
        "VSMain", "vs_5_0", flags, 0,
        vsBlob.GetAddressOf(), errBlob.GetAddressOf());
    if (FAILED(hr)) return false;

    errBlob.Reset();
    hr = D3DCompile(psSrc, strlen(psSrc), nullptr, nullptr, nullptr,
        "PSMain", "ps_5_0", flags, 0,
        psBlob.GetAddressOf(), errBlob.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_vs.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, m_ps.GetAddressOf());
    if (FAILED(hr)) return false;

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                 D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = m_device->CreateInputLayout(layout, _countof(layout),
        vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
        m_inputLayout.GetAddressOf());
    return SUCCEEDED(hr);
}

bool SpriteRenderer::CreateBuffers()
{
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DYNAMIC;
    vbDesc.ByteWidth = sizeof(Vertex) * 6;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    if (FAILED(m_device->CreateBuffer(&vbDesc, nullptr, m_vb.GetAddressOf()))) return false;

    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.ByteWidth = sizeof(CB);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    if (FAILED(m_device->CreateBuffer(&cbDesc, nullptr, m_cb.GetAddressOf()))) return false;

    return true;
}

bool SpriteRenderer::CreateTextureSRV(const wchar_t* path, ID3D11ShaderResourceView** outSrv)
{
    return LoadWICToSRV(m_device, path, outSrv);
}

bool SpriteRenderer::LoadTexture(const wchar_t* path)
{
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    if (!LoadWICToSRV(m_device, path, srv.GetAddressOf())) return false;
    m_srv = srv;
    return true;
}

void SpriteRenderer::SetTexture(ID3D11ShaderResourceView* srv)
{
    m_srv = srv;
}

void SpriteRenderer::Begin()
{
    m_commands.clear();
    m_submitSeq = 0;

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
    m_context->PSSetSamplers(0, 1, m_sampler.GetAddressOf());
}

void SpriteRenderer::Submit(
    float x, float y, float w, float h,
    float r, float g, float b, float a,
    int layer, int order, float depth)
{
    DrawCommand cmd{};
    cmd.x = x; cmd.y = y; cmd.w = w; cmd.h = h;
    cmd.r = r; cmd.g = g; cmd.b = b; cmd.a = a;
    cmd.layer = layer;
    cmd.order = order;
    cmd.depth = depth;
    cmd.seq = m_submitSeq++;

    cmd.srv = m_srv.Get();
    if (cmd.srv) cmd.srv->AddRef();

    m_commands.push_back(cmd);
}

void SpriteRenderer::Flush()
{
    std::stable_sort(m_commands.begin(), m_commands.end(),
        [](const DrawCommand& a, const DrawCommand& b)
        {
            if (a.layer != b.layer) return a.layer < b.layer;
            if (a.depth != b.depth) return a.depth > b.depth;
            if (a.order != b.order) return a.order < b.order;
            return a.seq < b.seq;
        });

    for (const auto& cmd : m_commands) DrawInternal(cmd);

    for (auto& cmd : m_commands)
    {
        if (cmd.srv) { cmd.srv->Release(); cmd.srv = nullptr; }
    }
    m_commands.clear();
}

void SpriteRenderer::DrawInternal(const DrawCommand& c)
{
    ID3D11ShaderResourceView* srv[1] = { c.srv };
    m_context->PSSetShaderResources(0, 1, srv);

    Vertex v[6] =
    {
        { c.x,       c.y,       0, c.r,c.g,c.b,c.a, 0,0 },
        { c.x + c.w,   c.y,       0, c.r,c.g,c.b,c.a, 1,0 },
        { c.x,       c.y + c.h,   0, c.r,c.g,c.b,c.a, 0,1 },

        { c.x + c.w,   c.y,       0, c.r,c.g,c.b,c.a, 1,0 },
        { c.x + c.w,   c.y + c.h,   0, c.r,c.g,c.b,c.a, 1,1 },
        { c.x,       c.y + c.h,   0, c.r,c.g,c.b,c.a, 0,1 },
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