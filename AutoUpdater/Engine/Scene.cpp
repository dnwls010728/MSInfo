#include "Scene.h"

#include "API/DownloadManager.h"
#include "imgui/imgui.h"
#include "shellapi.h"

Scene::Scene()
{
}

void Scene::StartUpdate()
{
    thread_handle_ = CreateThread(nullptr, 0, DownloadThread, nullptr, 0, nullptr);
}

void Scene::Tick(float delta_time)
{
    if (thread_handle_ != nullptr)
    {
        DWORD result = WaitForSingleObject(thread_handle_, 0);
        if (result == WAIT_OBJECT_0)
        {
            CloseHandle(thread_handle_);
            thread_handle_ = nullptr;

            ShellExecute(Core::GetInstance()->GetWindowHandle(), L"open", L".\\MSInfo.exe", nullptr, nullptr, SW_SHOW);

            exit(0);
        }
    }
}

void Scene::Render()
{
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    
    if (ImGui::Begin(u8"자동 업데이트", nullptr, flags))
    {
        ImGui::Text(u8"자동 업데이트 중입니다.");
        
        ImVec2 window_size = ImGui::GetWindowSize();
        ImGui::ProgressBar(progress_, ImVec2(window_size.x, 0.f));
    }

    ImGui::End();
}

DWORD Scene::DownloadThread(LPVOID lpParam)
{
    std::string download_url = "";
    DownloadManager::GetInstance()->DownloadFile(download_url, ".\\MSInfo.exe");

    return 0;
}
