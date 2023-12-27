#include "Core.h"

#include <string>

#include "Graphics/Graphics.h"
#include "Time/Time.h"
#include "API/APIManager.h"
#include "API/DataManager.h"
#include "API/DownloadManager.h"

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

    ImGui::StyleColorsLight();
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
        APIManager::GetInstance()->Release();
        DataManager::GetInstance()->Release();
        DownloadManager::GetInstance()->Release();
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

        if (ImGui::BeginMenu(u8"보기"))
        {
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (ImGui::Begin(u8"컨트롤"))
    {
        ImGui::Text(u8"데이터 기준일: 2023-12-26");

        static char input_character_name[256] = u8"";
        ImGui::InputText(u8"캐릭터 이름", input_character_name, 256);

        if (ImGui::Button(u8"캐릭터 조회"))
        {
            // 캐릭터 식별자 조회
            std::string character_name = input_character_name;
            rapidjson::Document id_doc = APIManager::GetInstance()->RequestID(character_name);
            DataManager::GetInstance()->ocid = id_doc["ocid"].GetString();

            rapidjson::Document character_doc = APIManager::GetInstance()->RequestCharacter(
                DataManager::GetInstance()->ocid, "2023-12-26");
            DataManager::GetInstance()->character_info = std::move(character_doc);

            // 캐릭터 이미지 다운로드
            std::string character_image_url = DataManager::GetInstance()->character_info["character_image"].
                GetString();

            DownloadManager::GetInstance()->DownloadFile(character_image_url,
                                                         ".\\Temp\\Character\\character_image.png");

            int image_width = 0;
            int image_height = 0;
            bool ret = Graphics::GetInstance()->LoadTexture(".\\Temp\\Character\\character_image.png",
                                                            &texture_view_, &image_width, &image_height);
            IM_ASSERT(ret);

            // 캐릭터 장비 조회
            rapidjson::Document item_equip_doc = APIManager::GetInstance()->RequestItemEquip(
                DataManager::GetInstance()->ocid, "2023-12-26");
            DataManager::GetInstance()->item_equip = std::move(item_equip_doc);

            // 장비 이미지 다운로드
            std::string a = DataManager::GetInstance()->item_equip["character_class"].GetString();
            int item_count = DataManager::GetInstance()->item_equip["item_equipment"].Size();

            items_.clear();

            for (int i = 0; i < item_count; ++i)
            {
                auto& item = DataManager::GetInstance()->item_equip["item_equipment"][i];
                std::string item_image_url = item["item_shape_icon"].GetString();
                std::string item_name = item["item_name"].GetString();

                DownloadManager::GetInstance()->DownloadFile(item_image_url,
                                                             ".\\Temp\\Character\\ItemEquip\\" + std::to_string(i) + ".png");

                std::unique_ptr<Item> item_ptr = std::make_unique<Item>();
                item_ptr->item_name = item_name;

                int temp_width = 0;
                int temp_height = 0;
                bool ret = Graphics::GetInstance()->LoadTexture(".\\Temp\\Character\\ItemEquip\\" + std::to_string(i) + ".png",
                                                                &item_ptr->texture_view, &temp_width, &temp_height);

                items_.push_back(*item_ptr);
            }
        }
    }

    ImGui::End();

    if (ImGui::Begin(u8"캐릭터 정보"))
    {
        if (!DataManager::GetInstance()->ocid.empty())
        {
            std::string name = DataManager::GetInstance()->character_info["character_name"].GetString();
            std::string world = DataManager::GetInstance()->character_info["world_name"].GetString();
            std::string _class = DataManager::GetInstance()->character_info["character_class"].GetString();
            std::string level = std::to_string(
                DataManager::GetInstance()->character_info["character_level"].GetInt());
            std::string guild = DataManager::GetInstance()->character_info["character_guild_name"].GetString();

            ImGui::Text(u8"%s %s", name.c_str(), world.c_str());
            ImGui::Text(u8"%s %s %s", _class.c_str(), level.c_str(), guild.c_str());
        }

        if (texture_view_)
        {
            ImGui::Image(texture_view_, ImVec2(128, 128));
        }
    }

    ImGui::End();

    if (ImGui::Begin(u8"장비"))
    {
        float height = ImGui::GetWindowHeight();
        
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("Equip", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("ALL"))
            {
                float tab_height = ImGui::GetFrameHeight();
                
                ImGui::BeginChild("ALL Scroll", ImVec2(0, height - tab_height), false, ImGuiWindowFlags_HorizontalScrollbar);
                
                for (auto& item : items_)
                {
                    ImGui::Columns(2);
                    
                    ImGui::SetColumnWidth(0, 48);
                    ImGui::Image(item.texture_view, ImVec2(32, 32));

                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Image(item.texture_view, ImVec2(128, 128));
                        ImGui::SameLine();
                        ImGui::Text(item.item_name.c_str());
                        ImGui::EndTooltip();
                    }
                    
                    ImGui::NextColumn();
                    ImGui::Text(item.item_name.c_str());
                    
                    ImGui::Columns(1);
                }

                ImGui::EndChild();
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem(u8"무보엠"))
            {
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem(u8"방어구"))
            {
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem(u8"장신구"))
            {
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
    }

    ImGui::End();
}
