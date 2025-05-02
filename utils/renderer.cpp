#include "renderer.h"
#include <dxgi.h>

Renderer::Renderer() {}
Renderer::~Renderer()
{
    Shutdown();
}

bool Renderer::Initialize(HWND hwnd, int width, int height)
{
    m_hwnd = hwnd;
    m_width = width;
    m_height = height;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[1] = {D3D_FEATURE_LEVEL_11_0};
    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 1,
                                 D3D11_SDK_VERSION, &m_device, &featureLevel, &m_context)))
        return false;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = m_width;
    desc.Height = m_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    if (FAILED(m_device->CreateTexture2D(&desc, nullptr, &m_renderTarget)))
        return false;

    if (FAILED(m_device->CreateRenderTargetView(m_renderTarget, nullptr, &m_renderTargetView)))
        return false;

    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    if (FAILED(m_device->CreateTexture2D(&desc, nullptr, &m_stagingTexture)))
        return false;

    m_pixelBuffer = nullptr;
    return true;
}

void Renderer::BeginFrame()
{
    const float clearColor[4] = {0, 0, 0, 0};
    m_context->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
    m_context->ClearRenderTargetView(m_renderTargetView, clearColor);
}

void Renderer::EndFrame()
{
    m_context->CopyResource(m_stagingTexture, m_renderTarget);
    D3D11_MAPPED_SUBRESOURCE mapped = {};
    if (SUCCEEDED(m_context->Map(m_stagingTexture, 0, D3D11_MAP_READ, 0, &mapped)))
    {
        m_pixelBuffer = mapped.pData;
        m_rowPitch = mapped.RowPitch;
    }
}

void Renderer::Resize(int width, int height)
{
    if (!m_device || !m_context)
        return;

    if (m_renderTargetView)
    {
        m_renderTargetView->Release();
        m_renderTargetView = nullptr;
    }

    if (m_renderTarget)
    {
        m_renderTarget->Release();
        m_renderTarget = nullptr;
    }

    if (m_stagingTexture)
    {
        m_stagingTexture->Release();
        m_stagingTexture = nullptr;
    }

    m_width = width;
    m_height = height;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = m_width;
    desc.Height = m_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    m_device->CreateTexture2D(&desc, nullptr, &m_renderTarget);
    m_device->CreateRenderTargetView(m_renderTarget, nullptr, &m_renderTargetView);

    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    m_device->CreateTexture2D(&desc, nullptr, &m_stagingTexture);
}

void Renderer::Shutdown()
{
    if (m_renderTargetView)
    {
        m_renderTargetView->Release();
        m_renderTargetView = nullptr;
    }

    if (m_renderTarget)
    {
        m_renderTarget->Release();
        m_renderTarget = nullptr;
    }

    if (m_stagingTexture)
    {
        m_stagingTexture->Release();
        m_stagingTexture = nullptr;
    }

    if (m_context)
    {
        m_context->Release();
        m_context = nullptr;
    }

    if (m_device)
    {
        m_device->Release();
        m_device = nullptr;
    }

    m_pixelBuffer = nullptr;
}