#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <cstdint>

class SpriteRenderer
{
public:
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

    // 旧互換: 直接ロードして現在テクスチャに設定
    bool LoadTexture(const wchar_t* path);

    // 新規: SRV作成だけ行う（TextureManager用）
    bool CreateTextureSRV(const wchar_t* path, ID3D11ShaderResourceView** outSrv);

    // 新規: 現在テクスチャを設定（TextureManager::Bind用）
    void SetTexture(ID3D11ShaderResourceView* srv);

    void Begin();

    void Submit(
        float x, float y, float w, float h,
        float r, float g, float b, float a,
        int layer, int order, float depth);

    void Flush();
    void End();
    void Finalize();

private:
    struct Vertex
    {
        float px, py, pz;
        float cr, cg, cb, ca;
        float u, v;
    };

    struct CB
    {
        float screenSize[2];
        float pad[2];
    };

    struct DrawCommand
    {
        float x, y, w, h;
        float r, g, b, a;
        int layer;
        int order;
        float depth;
        uint64_t seq;
        ID3D11ShaderResourceView* srv = nullptr; // コマンドごとのテクスチャ
    };

    bool CreateShaders();
    bool CreateBuffers();
    void DrawInternal(const DrawCommand& cmd);

private:
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_ps;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer>       m_vb;
    Microsoft::WRL::ComPtr<ID3D11Buffer>       m_cb;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv; // 現在テクスチャ
    Microsoft::WRL::ComPtr<ID3D11SamplerState>       m_sampler;

    float m_screenW = 1280.0f;
    float m_screenH = 720.0f;

    std::vector<DrawCommand> m_commands;
    uint64_t m_submitSeq = 0;
};