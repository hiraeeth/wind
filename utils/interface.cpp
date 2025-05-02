#include "interface.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

Interface::Interface() {}
Interface::~Interface() { Shutdown(); }

void Interface::Initialize(HWND hwnd, ID3D11Device *device, ID3D11DeviceContext *context)
{
    if (m_initialized)
        return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);

    m_initialized = true;
}

void Interface::Render()
{
    if (!m_initialized)
        return;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static bool show_demo = true;
    static bool open = true;
    if (open)
    {
        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(200, 80), ImGuiCond_Once);
        ImGui::Begin("Overlay Controls", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

        if (ImGui::Button("Close Overlay"))
        {
            open = false;
            PostQuitMessage(0);
        }

        ImGui::Checkbox("Show ImGui Demo", &show_demo);
        ImGui::End();
    }

    if (show_demo)
        ImGui::ShowDemoWindow(&show_demo);

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Interface::Shutdown()
{
    if (!m_initialized)
        return;

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    m_initialized = false;
}