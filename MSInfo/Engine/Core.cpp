#include "Core.h"

#include "Graphics/Graphics.h"
#include "Time/Time.h"
#include "API/APIManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

Core::Core() : class_name_(L"MSINFO")
{
}

ATOM Core::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = StaticWndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wcex.lpszClassName = class_name_;

    return RegisterClassEx(&wcex);
}

BOOL Core::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    const int screen_width = GetSystemMetrics(SM_CXSCREEN);
    const int screen_height = GetSystemMetrics(SM_CYSCREEN);

    resolution_ = { 960, 960 };
    window_area_ = { 0, 0, resolution_.x, resolution_.y };
    AdjustWindowRect(&window_area_, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd_ = CreateWindowEx(
        0,
        class_name_,
        L"메이플스토리 정보",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
        (screen_width - (window_area_.right - window_area_.left)) / 2,
        (screen_height - (window_area_.bottom - window_area_.top)) / 2,
        window_area_.right - window_area_.left,
        window_area_.bottom - window_area_.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd_) return FALSE;
    ShowWindow(hWnd_, nCmdShow);
    
    return TRUE;
}

bool Core::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    MyRegisterClass(hInstance);
    
    if (!InitInstance(hInstance, nCmdShow)) return false;
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    
    if (!Graphics::GetInstance()->Init()) return false;
    Time::GetInstance()->Init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO();
    static_cast<void>(io);
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontFromFileTTF(".\\Fonts\\NanumBarunGothic.ttf", 16.f, nullptr, io.Fonts->GetGlyphRangesKorean());

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hWnd_);
    ImGui_ImplDX11_Init(Graphics::GetInstance()->GetD3DDevice(), Graphics::GetInstance()->GetD3DDeviceContext());

    logic_handle_ = CreateThread(nullptr, 0, LogicThread, nullptr, 0, nullptr);
    
    return true;
}

LRESULT Core::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return GetInstance()->WndProc(hWnd, message, wParam, lParam);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) return 0;
    
    if (message == WM_DESTROY)
    {
        is_running_ = false;
        WaitForSingleObject(logic_handle_, INFINITE);

        ImGui_ImplWin32_Shutdown();
        ImGui_ImplDX11_Shutdown();
        ImGui::DestroyContext();
        
        Graphics::GetInstance()->Release();
        Time::GetInstance()->Release();
        GetInstance()->Release();

        CoUninitialize();
        PostQuitMessage(0);
        return 0;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

DWORD Core::LogicThread(LPVOID lpParam)
{
    GetInstance()->is_running_ = true;
    while (GetInstance()->is_running_)
    {
        GetInstance()->MainLogic();
    }

    return 0;
}

void Core::MainLogic()
{
    Time::GetInstance()->Tick();
    Tick(Time::GetInstance()->GetDeltaTime());
    
    Graphics::GetInstance()->BeginRenderD3D();
    Graphics::GetInstance()->BeginRenderD2D();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    Render();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    
    Graphics::GetInstance()->EndRenderD2D();
    Graphics::GetInstance()->EndRenderD3D();
}

void Core::Tick(float delta_time)
{
}

void Core::Render()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu(u8"메뉴"))
        {
            if (ImGui::MenuItem(u8"종료"))
            {
                PostMessage(hWnd_, WM_DESTROY, 0, 0);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        if (ImGui::Begin(u8"캐릭터 정보"))
        {
            ImGui::End();
        }
    }
}
