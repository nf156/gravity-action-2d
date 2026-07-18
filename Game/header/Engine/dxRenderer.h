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
    void DrawTriangle(float timeSec);
    void EndFrame();
    void Finalize();

private:
    bool CreateTriangleResources();

private:
    struct CB0
    {
        float offset[2];
		float pad[2]; // Padding to make the size of the structure a multiple of 16 bytes
    };

    Microsoft::WRL::ComPtr<ID3D11Device>           m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;

    Microsoft::WRL::ComPtr<ID3D11Buffer>           m_vb;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>     m_vs;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>      m_ps;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>      m_inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>           m_cb0;
};