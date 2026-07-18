#pragma once
#include <d3d11.h>
#include <wrl/client.h>

class SpriteRenderer
{
public:
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    bool LoadTexture(const wchar_t* path); // ’Ç‰Á
    void Begin();
    void Draw(float x, float y, float w, float h, float r, float g, float b, float a);
    void End();
    void Finalize();

private:
    struct Vertex
    {
        float px, py, pz;
        float cr, cg, cb, ca;
        float u, v; // ’Ç‰Á
    };

    struct CB
    {
        float screenSize[2]; // width, height
        float pad[2];
    };

    bool CreateShaders();
    bool CreateBuffers();

private:
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_ps;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer>       m_vb;
    Microsoft::WRL::ComPtr<ID3D11Buffer>       m_cb;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv; // ’Ç‰Á
    Microsoft::WRL::ComPtr<ID3D11SamplerState>       m_sampler; // ’Ç‰Á

    float m_screenW = 1280.0f;
    float m_screenH = 720.0f;
};