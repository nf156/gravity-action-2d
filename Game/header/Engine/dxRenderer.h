#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

class DXRenderer
{
public:
    bool Initialize(HWND hwnd, int width, int height);
    void BeginFrame(float r, float g, float b, float a);
    void DrawTriangle();
    void EndFrame();
    void Finalize();

private:
    bool CreateTriangleResources();

private:
    Microsoft::WRL::ComPtr<ID3D11Device>           m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;

    Microsoft::WRL::ComPtr<ID3D11Buffer>           m_vb;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>     m_vs;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>      m_ps;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>      m_inputLayout;
};