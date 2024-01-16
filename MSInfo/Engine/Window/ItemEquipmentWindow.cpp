#include "ItemEquipmentWindow.h"

#include "../ResourceManager.h"
#include "../Data/DataManager.h"
#include "../Scene.h"
#include "imgui/imgui.h"
#include "../Extension/ImGuiEx.h"

void ItemEquipmentWindow::Draw()
{
    std::shared_ptr<ResourceManager> ResourceManager = ResourceManager::GetInstance();
    
    struct ItemEquipmentData& item_equipment_data = DataManager::GetInstance()->GetItemEquipmentData();
    for (auto& item_equipment : item_equipment_data.item_equipment)
    {
        ImGui::Image(item_equipment.item_shape_texture.texture, ImVec2(32, 32));
        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::Text(u8"%s", item_equipment.item_name.c_str());

        if (item_equipment.starforce.compare("0") != 0)
        {
            ImGui::Image(ResourceManager->GetTexture("Star")->texture, ImVec2(16, 16));
            ImGui::SameLine();
            ImGui::Text(u8"%s", item_equipment.starforce.c_str());
        }

        ImGui::EndGroup();

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();

            if (item_equipment.starforce.compare("0") != 0)
            {
                float window_width = ImGui::GetWindowSize().x;
                float text_width = ImGui::CalcTextSize(item_equipment.starforce.c_str()).x;
                float text_pos_x = (window_width - (text_width + 20)) / 2;

                ImGui::SetCursorPosX(text_pos_x);
                ImGui::Image(ResourceManager->GetTexture("Star")->texture, ImVec2(16, 16));
                ImGui::SameLine();
                ImGui::Text(u8"%s", item_equipment.starforce.c_str());
            }

            std::string item_name = item_equipment.item_name + "(+" + item_equipment.scroll_upgrade + ")";
            ImGui::AlignCenter(item_name);
            ImGui::Text(u8"%s", item_name.c_str());

            if (!item_equipment.potential_option_grade.empty())
            {
                std::string item_grade = "(" + item_equipment.potential_option_grade + u8" 아이템)";
                ImGui::AlignCenter(item_grade);
                ImGui::Text(u8"%s", item_grade.c_str());
            }

            ImGui::Separator();
            ImGui::Image(item_equipment.item_shape_texture.texture, ImVec2(32, 32));
            ImGui::Separator();
            ImGui::Text(u8"장비분류: %s", item_equipment.item_equipment_part.c_str());

            DrawOption(u8"STR", item_equipment.item_total_option.stat_str, item_equipment.item_base_option.stat_str,
                       item_equipment.item_add_option.stat_str, item_equipment.item_etc_option.stat_str,
                       item_equipment.item_starforce_option.stat_str);
            DrawOption(u8"DEX", item_equipment.item_total_option.stat_dex, item_equipment.item_base_option.stat_dex,
                       item_equipment.item_add_option.stat_dex, item_equipment.item_etc_option.stat_dex,
                       item_equipment.item_starforce_option.stat_dex);
            DrawOption(u8"INT", item_equipment.item_total_option.stat_int, item_equipment.item_base_option.stat_int,
                       item_equipment.item_add_option.stat_int, item_equipment.item_etc_option.stat_int,
                       item_equipment.item_starforce_option.stat_int);
            DrawOption(u8"LUK", item_equipment.item_total_option.stat_luk, item_equipment.item_base_option.stat_luk,
                       item_equipment.item_add_option.stat_luk, item_equipment.item_etc_option.stat_luk,
                       item_equipment.item_starforce_option.stat_luk);
            DrawOption(u8"최대 HP", item_equipment.item_total_option.max_hp, item_equipment.item_base_option.max_hp,
                       item_equipment.item_add_option.max_hp, item_equipment.item_etc_option.max_hp,
                       item_equipment.item_starforce_option.max_hp);
            DrawOption(u8"최대 MP", item_equipment.item_total_option.max_mp, item_equipment.item_base_option.max_mp,
                       item_equipment.item_add_option.max_mp, item_equipment.item_etc_option.max_mp,
                       item_equipment.item_starforce_option.max_mp);
            DrawOption(u8"최대 HP", item_equipment.item_total_option.max_hp_rate,
                       item_equipment.item_base_option.max_hp_rate, "0", "0", "0", true);
            DrawOption(u8"최대 MP", item_equipment.item_total_option.max_mp_rate,
                       item_equipment.item_base_option.max_mp_rate, "0", "0", "0", true);
            DrawOption(u8"공격력", item_equipment.item_total_option.attack_power,
                       item_equipment.item_base_option.attack_power, item_equipment.item_add_option.attack_power,
                       item_equipment.item_etc_option.attack_power, item_equipment.item_starforce_option.attack_power);
            DrawOption(u8"마력", item_equipment.item_total_option.magic_power,
                       item_equipment.item_base_option.magic_power, item_equipment.item_add_option.magic_power,
                       item_equipment.item_etc_option.magic_power, item_equipment.item_starforce_option.magic_power);
            DrawOption(u8"방어력", item_equipment.item_total_option.armor, item_equipment.item_base_option.armor,
                       item_equipment.item_add_option.armor, item_equipment.item_etc_option.armor,
                       item_equipment.item_starforce_option.armor);
            DrawOption(u8"이동속도", item_equipment.item_total_option.speed, item_equipment.item_base_option.speed,
                       item_equipment.item_add_option.speed, item_equipment.item_etc_option.speed,
                       item_equipment.item_starforce_option.speed);
            DrawOption(u8"점프력", item_equipment.item_total_option.jump, item_equipment.item_base_option.jump,
                       item_equipment.item_add_option.jump, item_equipment.item_etc_option.jump,
                       item_equipment.item_starforce_option.jump);
            DrawOption(u8"보스 몬스터 공격 시 데미지", item_equipment.item_total_option.boss_damage,
                       item_equipment.item_base_option.boss_damage, item_equipment.item_add_option.boss_damage, "0",
                       "0", true);
            DrawOption(u8"몬스터 방어율 무시", item_equipment.item_total_option.ignore_monster_armor,
                       item_equipment.item_base_option.ignore_monster_armor, "0", "0", "0", true);
            DrawOption(u8"올스탯", item_equipment.item_total_option.all_stat, item_equipment.item_base_option.all_stat,
                       item_equipment.item_add_option.all_stat, "0", "0", true);
            DrawOption(u8"데미지", item_equipment.item_total_option.damage, "0", item_equipment.item_add_option.damage,
                       "0", "0", true);

            if (!item_equipment.potential_option_grade.empty())
            {
                ImGui::Separator();

                ImVec4 potential_value_color = Scene::GetInstance()->GetColorByGrade(item_equipment.potential_option_grade);
                ImGui::TextColored(potential_value_color, u8"잠재옵션");

                if (!item_equipment.potential_option_1.empty()) ImGui::Text(
                    u8"%s", item_equipment.potential_option_1.c_str());
                if (!item_equipment.potential_option_2.empty()) ImGui::Text(
                    u8"%s", item_equipment.potential_option_2.c_str());
                if (!item_equipment.potential_option_3.empty()) ImGui::Text(
                    u8"%s", item_equipment.potential_option_3.c_str());
            }

            if (!item_equipment.additional_potential_option_grade.empty())
            {
                ImGui::Separator();

                ImVec4 additional_potential_value_color = Scene::GetInstance()->GetColorByGrade(
                    item_equipment.additional_potential_option_grade);
                ImGui::TextColored(additional_potential_value_color, u8"에디셔널 잠재옵션");

                if (!item_equipment.additional_potential_option_1.empty()) ImGui::Text(
                    u8"%s", item_equipment.additional_potential_option_1.c_str());
                if (!item_equipment.additional_potential_option_2.empty()) ImGui::Text(
                    u8"%s", item_equipment.additional_potential_option_2.c_str());
                if (!item_equipment.additional_potential_option_3.empty()) ImGui::Text(
                    u8"%s", item_equipment.additional_potential_option_3.c_str());
            }

            if (item_equipment.item_name.compare(item_equipment.item_shape_name) != 0)
            {
                ImVec4 add_option_color = ImVec4(204.f / 255.f, 1.f, 0.f, 1.f);

                ImGui::Separator();
                
                ImGui::TextColored(add_option_color, u8"신비의 모루에 의해 [%s]의 외형이 합성됨",
                                   item_equipment.item_shape_name.c_str());
            }

            ImGui::EndTooltip();
        }

        ImGui::Separator();
    }
}

void ItemEquipmentWindow::DrawOption(std::string option, std::string total, std::string base, std::string add,
    std::string etc, std::string starforce, bool is_percent)
{
    if (total.compare("0") == 0) return;

    ImVec4 upgrade_color = ImVec4(102.f / 255.f, 1.f, 1.f, 1.f);
    ImVec4 add_option_color = ImVec4(204.f / 255.f, 1.f, 0.f, 1.f);
    ImVec4 etc_option_color = ImVec4(170.f / 255.f, 170.f / 255.f, 1.f, 1.f);
    ImVec4 starforce_option_color = ImVec4(1.f, 204.f / 255.f, 0.f, 1.f);

    if (add.compare("0") != 0 || etc.compare("0") != 0 || starforce.compare("0") != 0)
    {
        ImGui::TextColored(upgrade_color, u8"%s: +%s%s", option.c_str(), total.c_str(), is_percent ? u8"%" : "");
        ImGui::SameLine();
        ImGui::Text(u8"(");
        ImGui::SameLine();

        if (starforce.compare("0") != 0)
        {
            ImGui::Text(u8"%s", base.c_str());
            ImGui::SameLine();
            ImGui::Text(u8"+");
            ImGui::SameLine();
            ImGui::TextColored(add_option_color, u8"%s", add.c_str());
            ImGui::SameLine();
            ImGui::Text(u8"+");
            ImGui::SameLine();
            ImGui::TextColored(etc_option_color, u8"%s", etc.c_str());
            ImGui::SameLine();
            ImGui::Text(u8"+");
            ImGui::SameLine();
            ImGui::TextColored(starforce_option_color, u8"%s", starforce.c_str());
        }
        else if (etc.compare("0") != 0)
        {
            ImGui::Text(u8"%s", base.c_str());
            ImGui::SameLine();
            ImGui::Text(u8"+");
            ImGui::SameLine();
            ImGui::TextColored(add_option_color, u8"%s", add.c_str());
            ImGui::SameLine();
            ImGui::Text(u8"+");
            ImGui::SameLine();
            ImGui::TextColored(etc_option_color, u8"%s", etc.c_str());
        }
        else
        {
            ImGui::Text(u8"%s", base.c_str());
            ImGui::SameLine();
            ImGui::Text(u8"+");
            ImGui::SameLine();
            ImGui::TextColored(add_option_color, u8"%s", add.c_str());
        }

        ImGui::SameLine();
        ImGui::Text(u8")");

        return;
    }

    ImGui::Text(u8"%s: +%s%s", option.c_str(), total.c_str(), is_percent ? u8"%" : "");
}
