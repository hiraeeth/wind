#include <windows.h>
#include "utils/window.h"
#include "utils/renderer.h"
#include "utils/interface.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    SetProcessDPIAware();
    int w = 1, h = 1;

    Window window(L"wind", w, h);
    if (!window.Create(hInstance, nCmdShow))
        return 1;

    SetWindowPos(window.GetHWND(), HWND_NOTOPMOST, -10000, -10000, 1, 1, SWP_SHOWWINDOW);
    Renderer renderer;
    if (!renderer.Initialize(window.GetHWND(), w, h))
        return 1;
        
    window.SetRenderer(&renderer);

    Interface ui;
    ui.Initialize(window.GetHWND(), renderer.GetDevice(), renderer.GetDeviceContext());
    ShowWindow(window.GetHWND(), SW_HIDE);

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        renderer.BeginFrame();
        ui.Render();
        renderer.EndFrame();
    }

    ui.Shutdown();
    renderer.Shutdown();
    window.Destroy();

    return 0;
}