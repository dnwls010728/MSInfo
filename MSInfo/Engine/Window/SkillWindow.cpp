#include "SkillWindow.h"

#include "../Data/DataManager.h"
#include "imgui/imgui.h"

void SkillWindow::Draw()
{
    std::shared_ptr<DataManager> DataManager = DataManager::GetInstance();

    if (ImGui::BeginTabBar(u8"전직 차수"))
    {
        for (int i = 0; i < 2; i++)
        {
            std::string skill_grade = std::to_string(i + 5);

            if (ImGui::BeginTabItem((skill_grade + u8"차").c_str()))
            {
                ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

                for (auto& skill : DataManager->skill_data[i])
                {
                    ImGui::Image(skill.skill_texture.texture, ImVec2(skill.skill_texture.width, skill.skill_texture.height));
                    ImGui::SameLine();

                    ImGui::BeginGroup();
                    ImGui::Text(u8"%s Lv.%s", skill.skill_name.c_str(), skill.skill_level.c_str());
                    ImGui::TextWrapped(u8"%s", skill.skill_effect.c_str());
                    ImGui::EndGroup();

                    ImGui::Separator();
                }

                ImGui::EndChild();
                ImGui::EndTabItem();
            }
        }
    }

    ImGui::EndTabBar();
}
