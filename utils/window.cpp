#include "window.h"
#include "renderer.h"
#include "imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window(const std::wstring &title, int width, int height)
    : m_title(title), m_width(width), m_height(height) {}

Window::~Window()
{
    Destroy();
}

bool Window::Create(HINSTANCE hInstance, int nCmdShow)
{
    m_hInstance = hInstance;
    WNDCLASSEXW wc = {sizeof(WNDCLASSEXW), CS_CLASSDC, WndProc, 0L, 0L, hInstance, nullptr, nullptr, nullptr, nullptr, L"WindClass", nullptr};
    RegisterClassExW(&wc);

    RECT wr = {0, 0, m_width, m_height};
    AdjustWindowRect(&wr, WS_POPUP, FALSE);

    m_hwnd = CreateWindowExW(
        WS_EX_LAYERED,
        L"WindClass",
        m_title.c_str(),
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, hInstance, nullptr);

    if (!m_hwnd)
        return false;

    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
    ShowWindow(m_hwnd, nCmdShow);
    UpdateWindow(m_hwnd);
    return true;
}

void Window::Destroy()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }

    UnregisterClassW(L"WindClass", m_hInstance);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window *window = reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    if (msg == WM_SIZE && window && window->m_renderer)
    {
        RECT rect;
        GetClientRect(hWnd, &rect);

        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        if (width > 0 && height > 0)
            window->m_renderer->Resize(width, height);
    }

    if (msg == WM_CLOSE)
    {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}