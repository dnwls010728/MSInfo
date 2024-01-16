#include "InfoWindow.h"

#include "../Core.h"
#include "imgui/imgui.h"
#include "../Extension/ImGuiEx.h"

void InfoWindow::Draw()
{
    std::string api = u8"대적자 정보 탐색기 - v" + Core::GetInstance()->GetVersion();
    std::string email = u8"버그 제보 및 피드백: dnwls010728@gmail.com";
    std::string github = u8"GitHub: https://github.com/UnitySio";
    std::string content = u8"함께 발전시켜 나가실 분을 모집하고 있습니다.";

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImGui::AlignCenter(api);
    ImGui::Text(u8"%s", api.c_str());
    ImGui::PopFont();

    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::AlignCenter(email);
    ImGui::Text(u8"%s", email.c_str());

    ImGui::AlignCenter(github);
    ImGui::Text(u8"%s", github.c_str());

    ImGui::NewLine();

    ImGui::AlignCenter(content);
    ImGui::Text(u8"%s", content.c_str());
}
