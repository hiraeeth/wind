#include "utils/window.h"
#include "utils/renderer.h"
#include "utils/interface.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    Window window(L"wind", 1280, 720);
    if (!window.Create(hInstance, nCmdShow))
        return 1;

    Renderer renderer;
    if (!renderer.Initialize(window.GetHWND()))
        return 1;
    window.SetRenderer(&renderer);

    Interface ui;
    ui.Initialize(window.GetHWND(), renderer.GetDevice(), renderer.GetDeviceContext());

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