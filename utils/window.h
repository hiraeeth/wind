#pragma once
#include <windows.h>
#include <string>
class Renderer;

class Window
{
public:
    Window(const std::wstring &title, int width, int height);
    ~Window();

    bool Create(HINSTANCE hInstance, int nCmdShow);
    void Destroy();
    HWND GetHWND() const { return m_hwnd; }
    void SetRenderer(Renderer* renderer) { m_renderer = renderer; }
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    std::wstring m_title;
    int m_width, m_height;
    HWND m_hwnd = nullptr;
    HINSTANCE m_hInstance = nullptr;
    Renderer* m_renderer = nullptr;
};