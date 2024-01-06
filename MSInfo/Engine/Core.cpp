#include "Core.h"

#include "Graphics/Graphics.h"
#include "Time/Time.h"
#include "API/APIManager.h"
#include "Data/DataManager.h"
#include "API/DownloadManager.h"
#include "Scene.h"

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

    resolution_ = {960, 960};
    window_area_ = {0, 0, resolution_.x, resolution_.y};
    AdjustWindowRect(&window_area_, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd_ = CreateWindowEx(
        0,
        class_name_,
        L"대적자 정보 탐색기 - v1.0",
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
    io.Fonts->AddFontFromFileTTF(".\\Fonts\\NanumBarunGothicBold.ttf", 16.f, nullptr, io.Fonts->GetGlyphRangesKorean());
    io.Fonts->AddFontFromFileTTF(".\\Fonts\\NanumBarunGothicLight.ttf", 16.f, nullptr, io.Fonts->GetGlyphRangesKorean());
    io.Fonts->AddFontFromFileTTF(".\\Fonts\\NanumBarunGothicUltraLight.ttf", 16.f, nullptr, io.Fonts->GetGlyphRangesKorean());

    io.FontDefault = io.Fonts->Fonts[0];

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

    if (message == WM_GETMINMAXINFO)
    {
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = window_area_.right - window_area_.left;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = window_area_.bottom - window_area_.top;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMaxTrackSize.x = window_area_.right - window_area_.left;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMaxTrackSize.y = window_area_.bottom - window_area_.top;

        return 0;
    }

    if (message == WM_DESTROY)
    {
        is_running_ = false;
        WaitForSingleObject(logic_handle_, INFINITE);

        ImGui_ImplWin32_Shutdown();
        ImGui_ImplDX11_Shutdown();
        ImGui::DestroyContext();

        Graphics::GetInstance()->Release();
        Time::GetInstance()->Release();
        APIManager::GetInstance()->Release();
        DataManager::GetInstance()->Release();
        DownloadManager::GetInstance()->Release();
        Scene::GetInstance()->Release();
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
    Scene::GetInstance()->Tick(delta_time);
}

void Core::Render()
{
    Scene::GetInstance()->Render();
}
