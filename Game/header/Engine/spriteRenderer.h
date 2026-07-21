#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

class SpriteRenderer
{
public:
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    bool LoadTexture(const wchar_t* path);
    void Begin();

    // ’Ç‰Á: ’¼ÚDraw‚Ì‘ã‚í‚è‚ÉƒLƒ…[‚ÖÏ‚Ş
    // Submit ‚Ìˆø”‚É depth ’Ç‰Á
    void Submit(
        float x, float y, float w, float h,
        float r, float g, float b, float a,
        int layer, int order, float depth);

    // ’Ç‰Á: sort‚µ‚ÄÀ•`‰æ
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
        float depth;   // ’Ç‰Á: ¬‚³‚¢‚Ù‚Çè‘O / ‘å‚«‚¢‚Ù‚Ç‰œ
        uint64_t seq;
    };

    void DrawInternal(const DrawCommand& cmd);
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
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>       m_sampler;

    float m_screenW = 1280.0f;
    float m_screenH = 720.0f;

    std::vector<DrawCommand> m_commands; // ’Ç‰Á
    uint64_t m_submitSeq = 0;             // ’Ç‰Á
};