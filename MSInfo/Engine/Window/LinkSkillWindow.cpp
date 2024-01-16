#include "LinkSkillWindow.h"

#include "../Data/DataManager.h"
#include "imgui/imgui.h"

void LinkSkillWindow::Draw()
{
    for (auto& link_skill : DataManager::GetInstance()->GetLinkSkillData())
    {
        ImGui::Image(link_skill.skill_texture.texture, ImVec2(link_skill.skill_texture.width, link_skill.skill_texture.height));
        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::Text(u8"%s Lv.%s", link_skill.skill_name.c_str(), link_skill.skill_level.c_str());
        ImGui::TextWrapped(u8"%s", link_skill.skill_effect.c_str());
        ImGui::EndGroup();

        ImGui::Separator();
    }
}
