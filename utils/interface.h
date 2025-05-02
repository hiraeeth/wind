#pragma once
#include <d3d11.h>
#include <windows.h>

class Interface
{
public:
    Interface();
    ~Interface();

    void Initialize(HWND hwnd, ID3D11Device *device, ID3D11DeviceContext *context);
    void Render();
    void Shutdown();

private:
    bool m_initialized = false;
};