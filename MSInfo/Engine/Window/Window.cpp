#include "Window.h"

#include "../Scene.h"
#include "imgui/imgui.h"

void Window::Open(const std::string& title, bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title.c_str(), p_open) || Scene::GetInstance()->IsSearching())
    {
        ImGui::End();
        return;
    }

    Draw();
    ImGui::End();
}
