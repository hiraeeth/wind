#pragma once
#include <d3d11.h>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Initialize(HWND hwnd, int width, int height);
    void BeginFrame();
    void EndFrame();
    void Shutdown();
    void Resize(int width, int height);

    const void* GetPixelBuffer() const { return m_pixelBuffer; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetMappedRowPitch() const { return m_rowPitch; }

    ID3D11Device *GetDevice() const { return m_device; }
    ID3D11DeviceContext *GetDeviceContext() const { return m_context; }

    ID3D11Texture2D* GetStagingTexture() const { return m_stagingTexture; }

private:
    HWND m_hwnd = nullptr;
    ID3D11Device *m_device = nullptr;
    ID3D11DeviceContext *m_context = nullptr;
    ID3D11Texture2D *m_renderTarget = nullptr;
    ID3D11RenderTargetView *m_renderTargetView = nullptr;
    ID3D11Texture2D *m_stagingTexture = nullptr;
    void* m_pixelBuffer = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_rowPitch = 0;
};