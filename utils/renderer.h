#pragma once
#include <d3d11.h>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Initialize(HWND hwnd);
    void BeginFrame();
    void EndFrame();
    void Shutdown();
    void Resize(int width, int height);

    ID3D11Device *GetDevice() const { return m_device; }
    ID3D11DeviceContext *GetDeviceContext() const { return m_context; }

private:
    HWND m_hwnd = nullptr;
    IDXGISwapChain *m_swapChain = nullptr;
    ID3D11Device *m_device = nullptr;
    ID3D11DeviceContext *m_context = nullptr;
    ID3D11RenderTargetView *m_renderTargetView = nullptr;
};