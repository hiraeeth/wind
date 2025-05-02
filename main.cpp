#include <windows.h>
#include "utils/window.h"
#include "utils/renderer.h"
#include "utils/interface.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    SetProcessDPIAware();

    RECT rc;
    GetWindowRect(GetDesktopWindow(), &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    Window window(L"wind", width, height);
    if (!window.Create(hInstance, nCmdShow))
        return 1;

    SetWindowPos(window.GetHWND(), HWND_NOTOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
    ShowWindow(window.GetHWND(), SW_SHOW);
    UpdateWindow(window.GetHWND());

    Renderer renderer;
    if (!renderer.Initialize(window.GetHWND(), width, height))
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

        HDC hdcScreen = GetDC(nullptr);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = renderer.GetWidth();
        bmi.bmiHeader.biHeight = -renderer.GetHeight();
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void *bits = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);
        char bmiDbg[512];

        if (!hBitmap)
            MessageBoxA(nullptr, bmiDbg, "CreateDIBSection failed!", MB_OK);

        if (hBitmap && bits)
        {
            const uint8_t *src = static_cast<const uint8_t *>(renderer.GetPixelBuffer());
            uint8_t *dst = static_cast<uint8_t *>(bits);

            int width = renderer.GetWidth();
            int height = renderer.GetHeight();

            int src_pitch = renderer.GetMappedRowPitch();
            int dst_pitch = width * 4;

            for (int y = 0; y < height; ++y)
                memcpy(dst + y * dst_pitch, src + y * src_pitch, dst_pitch);

            HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
            SIZE size = {renderer.GetWidth(), renderer.GetHeight()};
            POINT ptSrc = {0, 0};
            POINT ptDst = {0, 0};
            BLENDFUNCTION blend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

            char dbg[512];
            sprintf(dbg,
                    "UpdateLayeredWindow params:\n"
                    "hWnd: 0x%p\n"
                    "hdcScreen: 0x%p\n"
                    "hdcMem: 0x%p\n"
                    "hBitmap: 0x%p\n"
                    "width: %d\n"
                    "height: %d\n",
                    window.GetHWND(), hdcScreen, hdcMem, hBitmap, renderer.GetWidth(), renderer.GetHeight());

            BOOL result = UpdateLayeredWindow(window.GetHWND(), hdcScreen, &ptDst, &size, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
            if (!result)
            {
                DWORD err = GetLastError();
                char buf[1024];
                sprintf(buf, "%sUpdateLayeredWindow failed: %lu", dbg, err);
                MessageBoxA(nullptr, buf, "Error", MB_OK);
            }

            SelectObject(hdcMem, oldBitmap);
            DeleteObject(hBitmap);
        }

        DeleteDC(hdcMem);
        ReleaseDC(nullptr, hdcScreen);

        renderer.GetDeviceContext()->Unmap(renderer.GetStagingTexture(), 0);
    }

    ui.Shutdown();
    renderer.Shutdown();
    window.Destroy();

    return 0;
}