#include "CashItemEquipmentWindow.h"

#include "../Data/DataManager.h"
#include "imgui/imgui.h"

void CashItemEquipmentWindow::Draw()
{
    struct CashItemEquipmentData& cash_item_equipment_data = DataManager::GetInstance()->GetCashItemEquipmentData();
    ImGui::Text(u8"현재 프리셋: %s", cash_item_equipment_data.preset_no.c_str());

    ImGui::Separator();

    struct BeautyEquipmentData& beauty_equipment_data = DataManager::GetInstance()->GetBeautyEquipmentData();
    ImGui::Text(u8"헤어: %s", beauty_equipment_data.character_hair.name.c_str());
    ImGui::Text(u8"성형: %s", beauty_equipment_data.character_face.name.c_str());
    ImGui::Text(u8"피부: %s", beauty_equipment_data.character_skin_name.c_str());

    if (!beauty_equipment_data.additional_character_face.name.empty())
    {
        ImGui::Separator();
        ImGui::Text(u8"헤어: %s", beauty_equipment_data.additional_character_hair.name.c_str());
        ImGui::Text(u8"성형: %s", beauty_equipment_data.additional_character_face.name.c_str());
        ImGui::Text(u8"피부: %s", beauty_equipment_data.additional_character_skin_name.c_str());
    }

    if (ImGui::BeginTabBar(u8"##캐시 장비"))
    {
        for (int i = 0; i < 3; i++)
        {
            if (ImGui::BeginTabItem((u8"프리셋 " + std::to_string(i + 1)).c_str()))
            {
                ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

                for (auto& cash_item_equipment : cash_item_equipment_data.presets[i])
                {
                    ImGui::Image(cash_item_equipment.cash_item_texture.texture, ImVec2(32, 32));
                    ImGui::SameLine();

                    ImGui::BeginGroup();
                    ImGui::Text(u8"%s", cash_item_equipment.cash_item_name.c_str());

                    ImGui::EndGroup();

                    ImGui::Separator();
                }

                ImGui::EndChild();
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }
}
