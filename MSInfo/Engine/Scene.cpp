#include "Scene.h"

#include <string>

#include "Data/DataManager.h"
#include "API/APIManager.h"
#include "API/DownloadManager.h"
#include "Graphics/Graphics.h"

#include "imgui/imgui.h"

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

#define CHARACTER_IMAGE_PATH ".\\Temp\\Character\\character_image.png"
#define LINK_SKILL_ICON_PATH ".\\Temp\\Icon\\LinkSkill\\"
#define SKILL_ICON_PATH ".\\Temp\\Icon\\Skill\\"

Scene::Scene()
{
    date_ = DataManager::GetInstance()->GetDataDate();
}

void Scene::Tick(float delta_time)
{
}

void Scene::Render()
{
    std::shared_ptr<DataManager> DataManager = DataManager::GetInstance();

    static bool show_link_skill = false;
    static bool show_skill = false;

#pragma region 기본
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu(u8"파일"))
        {
            if (ImGui::MenuItem(u8"종료"))
            {
                exit(0);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(u8"창"))
        {
            ImGui::MenuItem(u8"링크 스킬", nullptr, &show_link_skill);
            ImGui::MenuItem(u8"스킬", nullptr, &show_skill);
            
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
#pragma endregion

#pragma region 컨트롤
    if (ImGui::Begin(u8"컨트롤"))
    {
        ImGui::Text(u8"데이터 기준일: %s", date_.c_str());

        static char input_character_name[256] = u8"";
        ImGui::InputText(u8"캐릭터 이름", input_character_name, sizeof(input_character_name));

        ImGui::SameLine();

        if (ImGui::Button(u8"캐릭터 조회"))
        {
            SearchCharacter(input_character_name);
        }
        
        if (ImGui::BeginPopupModal(u8"캐릭터 조회 오류", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
        {
            ImGui::Text(u8"캐릭터를 찾을 수 없습니다.");
            ImGui::Separator();
        
            if (ImGui::Button(u8"확인", ImVec2(60,0))) { ImGui::CloseCurrentPopup(); }    
        
            ImGui::EndPopup();
        }
    }

    ImGui::End();
#pragma endregion

#pragma region 캐릭터 정보
    if (ImGui::Begin(u8"캐릭터 정보"))
    {
        if (character_image)
        {
            ImGui::Image(character_image, ImVec2(character_image_width, character_image_height));
        }

        ImGui::SameLine();

        if (!DataManager->GetCharacterData().character_name.empty())
        {
            ImGui::Text(u8"%s (%s)\nLv.%s | %s | %s",
                        DataManager->GetCharacterData().character_name.c_str(),
                        DataManager->GetCharacterData().world_name.c_str(),
                        DataManager->GetCharacterData().character_level.c_str(),
                        DataManager->GetCharacterData().character_class.c_str(),
                        DataManager->GetCharacterData().character_guild_name.c_str());
        }

        ImGui::Separator();

        if (!DataManager->GetStatData().min_stat_attack.empty())
        {
            ImGui::Text(u8"전투력");
            ImGui::SameLine();

            std::string combat_power = DataManager->GetStatData().combat_power;
            std::string format_unit = DataManager->SafeFormatUnit(combat_power);

            float window_width = ImGui::GetWindowSize().x;
            float text_width = ImGui::CalcTextSize(format_unit.c_str()).x;
            float text_pos_x = (window_width - text_width) * .5f;

            ImGui::SetCursorPosX(text_pos_x);
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
            ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), u8"%s", format_unit.c_str());
            ImGui::PopFont();

            ImGui::Separator();

            if (ImGui::BeginTable("##캐릭터 정보_1", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"스텟 공격력");

                ImGui::TableSetColumnIndex(1);

                std::string min_stat_attack = DataManager->GetStatData().min_stat_attack;
                std::string max_stat_attack = DataManager->GetStatData().max_stat_attack;
                std::string format_min_stat_attack = DataManager->SafeFormatComma(min_stat_attack);
                std::string format_max_stat_attack = DataManager->SafeFormatComma(max_stat_attack);
                ImGui::Text(u8"%s ~ %s", format_min_stat_attack.c_str(), format_max_stat_attack.c_str());

                ImGui::EndTable();
            }

            if (ImGui::BeginTable("##캐릭터 정보_2", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"HP");

                ImGui::TableSetColumnIndex(1);

                std::string hp = DataManager->GetStatData().hp;
                std::string format_comma = DataManager->SafeFormatComma(hp);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::Text(u8"MP");

                ImGui::TableSetColumnIndex(3);

                std::string mp = DataManager->GetStatData().mp;
                format_comma = DataManager->SafeFormatComma(mp);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"STR");

                ImGui::TableSetColumnIndex(1);

                std::string stat_str = DataManager->GetStatData().stat_str;
                format_comma = DataManager->SafeFormatComma(stat_str);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::Text(u8"DEX");

                ImGui::TableSetColumnIndex(3);

                std::string stat_dex = DataManager->GetStatData().stat_dex;
                format_comma = DataManager->SafeFormatComma(stat_dex);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"INT");

                ImGui::TableSetColumnIndex(1);

                std::string stat_int = DataManager->GetStatData().stat_int;
                format_comma = DataManager->SafeFormatComma(stat_int);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::Text(u8"LUK");

                ImGui::TableSetColumnIndex(3);

                std::string stat_luk = DataManager->GetStatData().stat_luk;
                format_comma = DataManager->SafeFormatComma(stat_luk);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::EndTable();
            }

            ImGui::Separator();

            ImGui::BeginChild("Stat Scroll", ImVec2(0, 200), false, ImGuiWindowFlags_HorizontalScrollbar);
            if (ImGui::BeginTable("##캐릭터 정보_3", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"데미지");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"최종 데미지");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().final_damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"무기 숙련도");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().weapon_mastery.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"보스 몬스터 데미지");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().boss_damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"일반 몬스터 데미지");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().normal_monster_damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"방어율 무시");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().ignore_defense.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"공격력");

                ImGui::TableSetColumnIndex(1);
                std::string attack_power = DataManager->GetStatData().attack_power;
                std::string format_comma = DataManager->SafeFormatComma(attack_power);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"마력");

                ImGui::TableSetColumnIndex(1);
                std::string magic_power = DataManager->GetStatData().magic_power;
                format_comma = DataManager->SafeFormatComma(magic_power);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"방어력");

                ImGui::TableSetColumnIndex(1);
                std::string defense = DataManager->GetStatData().defense;
                format_comma = DataManager->SafeFormatComma(defense);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"크리티컬 확률");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().critical_rate.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"크리티컬 데미지");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().critical_damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"버프 지속 시간");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().buff_duration.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"재사용 대기시간 감소");

                ImGui::TableSetColumnIndex(1);
                std::string cooldown_reduction = DataManager->GetStatData().cooldown_reduction;
                std::string cooldown_reduction_percent = DataManager->GetStatData().cooldown_reduction_percent;
                ImGui::Text(u8"%s초 / %s%%", cooldown_reduction.c_str(), cooldown_reduction_percent.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"재사용 대기시간 미적용");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().cooldown_reduction_not_apply.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"속성 내성 무시");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().status_resistance_ignore.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"상태이상 내성");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s", DataManager->GetStatData().status_resistance.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"상태이상 추가 데미지");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().additional_damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"소환수 지속시간 증가");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().summon_duration.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"메소 획득량");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().meso_drop_rate.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"아이템 드롭률");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().item_drop_rate.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"추가 경험치 획득");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().additional_exp.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"공격속도");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s단계", DataManager->GetStatData().attack_speed.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"이동속도");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().speed.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"점프력");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().jump.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"스탠스");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatData().stance.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"스타포스");

                ImGui::TableSetColumnIndex(1);
                std::string star_force = DataManager->GetStatData().star_force;
                format_comma = DataManager->SafeFormatComma(star_force);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"아케인포스");

                ImGui::TableSetColumnIndex(1);
                std::string arcane_force = DataManager->GetStatData().arcane_force;
                format_comma = DataManager->SafeFormatComma(arcane_force);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"어센틱포스");

                ImGui::TableSetColumnIndex(1);
                std::string authentic_force = DataManager->GetStatData().authentic_force;
                format_comma = DataManager->SafeFormatComma(authentic_force);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 STR");

                ImGui::TableSetColumnIndex(1);
                std::string ap_str = DataManager->GetStatData().ap_str;
                format_comma = DataManager->SafeFormatComma(ap_str);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 DEX");

                ImGui::TableSetColumnIndex(1);
                std::string ap_dex = DataManager->GetStatData().ap_dex;
                format_comma = DataManager->SafeFormatComma(ap_dex);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 INT");

                ImGui::TableSetColumnIndex(1);
                std::string ap_int = DataManager->GetStatData().ap_int;
                format_comma = DataManager->SafeFormatComma(ap_int);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 LUK");

                ImGui::TableSetColumnIndex(1);
                std::string ap_luk = DataManager->GetStatData().ap_luk;
                format_comma = DataManager->SafeFormatComma(ap_luk);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 HP");

                ImGui::TableSetColumnIndex(1);
                std::string ap_hp = DataManager->GetStatData().ap_hp;
                format_comma = DataManager->SafeFormatComma(ap_hp);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 MP");

                ImGui::TableSetColumnIndex(1);
                std::string ap_mp = DataManager->GetStatData().ap_mp;
                format_comma = DataManager->SafeFormatComma(ap_mp);
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::EndTable();
            }

            ImGui::EndChild();
        }

        ImVec2 window_size = ImGui::GetWindowSize();
        ImGui::BeginChild("Other Scroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (ImGui::CollapsingHeader(u8"세트 장비"))
        {
            for (auto& set_effect : DataManager->GetSetEffectData().set_effects)
            {
                std::string set_name = set_effect.set_name;
                std::string total_set_count = set_effect.total_set_count;
                ImGui::Text(u8"%s: %s세트", set_name.c_str(), total_set_count.c_str());
            }
        }

        if (ImGui::CollapsingHeader(u8"어빌리티"))
        {
            if (!DataManager->GetAbilityData().ability_grade.empty())
            {
                for (auto& ability : DataManager->GetAbilityData().abilities)
                {
                    std::string ability_value = ability.ability_value;
                    ImVec4 ability_value_color = GetColorByGrade(ability.ability_grade);
                    ImGui::TextColored(ability_value_color, u8"%s", ability_value.c_str());
                }
            }
        }

        if (ImGui::CollapsingHeader(u8"하이퍼 스텟"))
        {
            struct HyperStatData hyper_stat_data = DataManager->GetHyperStatData();
            ImGui::Text(u8"현재 프리셋: 프리셋 %s", hyper_stat_data.use_preset_no.c_str());

            if (ImGui::BeginTabBar("하이퍼 스텟 프리셋"))
            {
                if (ImGui::BeginTabItem(u8"프리셋 1"))
                {
                    for (auto& hyper_stat : hyper_stat_data.preset_1)
                    {
                        std::string stat_type = hyper_stat.stat_type;
                        std::string stat_point = hyper_stat.stat_point;
                        std::string stat_level = hyper_stat.stat_level;
                        std::string stat_increase = hyper_stat.stat_increase;

                        if (stat_point.empty()) continue;

                        ImGui::Text(u8"Lv.%s: %s", stat_level.c_str(), stat_increase.c_str());
                    }

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(u8"프리셋 2"))
                {
                    for (auto& hyper_stat : hyper_stat_data.preset_2)
                    {
                        std::string stat_type = hyper_stat.stat_type;
                        std::string stat_point = hyper_stat.stat_point;
                        std::string stat_level = hyper_stat.stat_level;
                        std::string stat_increase = hyper_stat.stat_increase;

                        if (stat_point.empty()) continue;

                        ImGui::Text(u8"Lv.%s: %s", stat_level.c_str(), stat_increase.c_str());
                    }

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(u8"프리셋 3"))
                {
                    for (auto& hyper_stat : hyper_stat_data.preset_3)
                    {
                        std::string stat_type = hyper_stat.stat_type;
                        std::string stat_point = hyper_stat.stat_point;
                        std::string stat_level = hyper_stat.stat_level;
                        std::string stat_increase = hyper_stat.stat_increase;

                        if (stat_point.empty()) continue;

                        ImGui::Text(u8"Lv.%s: %s", stat_level.c_str(), stat_increase.c_str());
                    }

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }

        ImGui::EndChild();
    }

    ImGui::End();
#pragma endregion
    
    if (show_link_skill) ShowLinkSkill(&show_link_skill);
    if (show_skill) ShowSkill(&show_skill);
}

// 추후 성능 확인 후 멀티 스레드로 개선
void Scene::SearchCharacter(const std::string& character_name)
{
    std::shared_ptr<DataManager> DataManager = DataManager::GetInstance();

#pragma region 캐릭터 식별자
    rapidjson::Document id_document = APIManager::GetInstance()->RequestID(character_name);
    DataManager->SetOcid(SafeGetString(id_document, "ocid"));
#pragma endregion
    
    if (DataManager->GetOcid().empty())
    {
        ImGui::OpenPopup(u8"캐릭터 조회 오류");
        return;
    }

#pragma region 캐릭터 정보
    rapidjson::Document character_document = APIManager::GetInstance()->RequestCharacter(DataManager->GetOcid(), date_);

    std::string character_image_url = SafeGetString(character_document, "character_image");
    DownloadManager::GetInstance()->DownloadFile(character_image_url, CHARACTER_IMAGE_PATH);

    bool ret = Graphics::GetInstance()->LoadTexture(CHARACTER_IMAGE_PATH, &character_image, &character_image_width,
                                                    &character_image_height);
    IM_ASSERT(ret);

    struct CharacterData character_data = {
        SafeGetString(character_document, "character_name"),
        SafeGetString(character_document, "world_name"),
        SafeGetString(character_document, "character_class"),
        SafeGetString(character_document, "character_level"),
        SafeGetString(character_document, "character_guild_name")
    };

    DataManager->SetCharacterData(character_data);
#pragma endregion

#pragma region 스텟
    rapidjson::Document stat_document = APIManager::GetInstance()->RequestStat(DataManager->GetOcid(), date_);

    struct StatData stat_data = {
        SafeGetString(stat_document["final_stat"][0], "stat_value"),
        SafeGetString(stat_document["final_stat"][1], "stat_value"),
        SafeGetString(stat_document["final_stat"][2], "stat_value"),
        SafeGetString(stat_document["final_stat"][3], "stat_value"),
        SafeGetString(stat_document["final_stat"][4], "stat_value"),
        SafeGetString(stat_document["final_stat"][5], "stat_value"),
        SafeGetString(stat_document["final_stat"][6], "stat_value"),
        SafeGetString(stat_document["final_stat"][7], "stat_value"),
        SafeGetString(stat_document["final_stat"][8], "stat_value"),
        SafeGetString(stat_document["final_stat"][9], "stat_value"),
        SafeGetString(stat_document["final_stat"][10], "stat_value"),
        SafeGetString(stat_document["final_stat"][11], "stat_value"),
        SafeGetString(stat_document["final_stat"][12], "stat_value"),
        SafeGetString(stat_document["final_stat"][13], "stat_value"),
        SafeGetString(stat_document["final_stat"][14], "stat_value"),
        SafeGetString(stat_document["final_stat"][15], "stat_value"),
        SafeGetString(stat_document["final_stat"][16], "stat_value"),
        SafeGetString(stat_document["final_stat"][17], "stat_value"),
        SafeGetString(stat_document["final_stat"][18], "stat_value"),
        SafeGetString(stat_document["final_stat"][19], "stat_value"),
        SafeGetString(stat_document["final_stat"][20], "stat_value"),
        SafeGetString(stat_document["final_stat"][21], "stat_value"),
        SafeGetString(stat_document["final_stat"][22], "stat_value"),
        SafeGetString(stat_document["final_stat"][23], "stat_value"),
        SafeGetString(stat_document["final_stat"][24], "stat_value"),
        SafeGetString(stat_document["final_stat"][25], "stat_value"),
        SafeGetString(stat_document["final_stat"][26], "stat_value"),
        SafeGetString(stat_document["final_stat"][27], "stat_value"),
        SafeGetString(stat_document["final_stat"][28], "stat_value"),
        SafeGetString(stat_document["final_stat"][29], "stat_value"),
        SafeGetString(stat_document["final_stat"][30], "stat_value"),
        SafeGetString(stat_document["final_stat"][31], "stat_value"),
        SafeGetString(stat_document["final_stat"][32], "stat_value"),
        SafeGetString(stat_document["final_stat"][33], "stat_value"),
        SafeGetString(stat_document["final_stat"][34], "stat_value"),
        SafeGetString(stat_document["final_stat"][35], "stat_value"),
        SafeGetString(stat_document["final_stat"][36], "stat_value"),
        SafeGetString(stat_document["final_stat"][37], "stat_value"),
        SafeGetString(stat_document["final_stat"][38], "stat_value"),
        SafeGetString(stat_document["final_stat"][39], "stat_value"),
        SafeGetString(stat_document["final_stat"][40], "stat_value"),
        SafeGetString(stat_document["final_stat"][41], "stat_value"),
        SafeGetString(stat_document["final_stat"][42], "stat_value"),
        SafeGetString(stat_document["final_stat"][43], "stat_value")
    };

    DataManager->SetStatData(stat_data);
#pragma endregion

#pragma region 어빌리티
    rapidjson::Document ability_document = APIManager::GetInstance()->RequestAbility(DataManager->GetOcid(), date_);
    rapidjson::Value& ability_info = ability_document["ability_info"].GetArray();

    struct AbilityData ability_data;
    ability_data.ability_grade = SafeGetString(ability_info[0], "ability_grade");

    for (int i = 0; i < ability_info.Size(); i++)
    {
        struct AbilityData::Ability ability;
        ability.ability_grade = SafeGetString(ability_info[i], "ability_grade");
        ability.ability_value = SafeGetString(ability_info[i], "ability_value");
        ability_data.abilities.push_back(ability);
    }

    DataManager->SetAbilityData(ability_data);

#pragma endregion

#pragma region 하이퍼 스텟
    rapidjson::Document hyper_stat_document = APIManager::GetInstance()->
        RequestHyperStat(DataManager->GetOcid(), date_);
    rapidjson::Value& hyper_stat_preset_1 = hyper_stat_document["hyper_stat_preset_1"].GetArray();
    rapidjson::Value& hyper_stat_preset_2 = hyper_stat_document["hyper_stat_preset_2"].GetArray();
    rapidjson::Value& hyper_stat_preset_3 = hyper_stat_document["hyper_stat_preset_3"].GetArray();

    struct HyperStatData hyper_stat_data;
    hyper_stat_data.use_preset_no = hyper_stat_document["use_preset_no"].GetString();

    for (int i = 0; i < hyper_stat_preset_1.Size(); i++)
    {
        struct HyperStatData::HyperStat hyper_stat;
        hyper_stat.stat_type = SafeGetString(hyper_stat_preset_1[i], "stat_type");
        hyper_stat.stat_point = SafeGetString(hyper_stat_preset_1[i], "stat_point");
        hyper_stat.stat_level = SafeGetString(hyper_stat_preset_1[i], "stat_level");
        hyper_stat.stat_increase = SafeGetString(hyper_stat_preset_1[i], "stat_increase");

        hyper_stat_data.preset_1.push_back(hyper_stat);
    }

    for (int i = 0; i < hyper_stat_preset_2.Size(); i++)
    {
        struct HyperStatData::HyperStat hyper_stat;
        hyper_stat.stat_type = SafeGetString(hyper_stat_preset_2[i], "stat_type");
        hyper_stat.stat_point = SafeGetString(hyper_stat_preset_2[i], "stat_point");
        hyper_stat.stat_level = SafeGetString(hyper_stat_preset_2[i], "stat_level");
        hyper_stat.stat_increase = SafeGetString(hyper_stat_preset_2[i], "stat_increase");

        hyper_stat_data.preset_2.push_back(hyper_stat);
    }

    for (int i = 0; i < hyper_stat_preset_3.Size(); i++)
    {
        struct HyperStatData::HyperStat hyper_stat;
        hyper_stat.stat_type = SafeGetString(hyper_stat_preset_3[i], "stat_type");
        hyper_stat.stat_point = SafeGetString(hyper_stat_preset_3[i], "stat_point");
        hyper_stat.stat_level = SafeGetString(hyper_stat_preset_3[i], "stat_level");
        hyper_stat.stat_increase = SafeGetString(hyper_stat_preset_3[i], "stat_increase");

        hyper_stat_data.preset_3.push_back(hyper_stat);
    }

    DataManager->SetHyperStatData(hyper_stat_data);

#pragma endregion

#pragma region 세트 장비
    rapidjson::Document set_effect_document = APIManager::GetInstance()->
        RequestSetEffect(DataManager->GetOcid(), date_);
    rapidjson::Value& set_effect_info = set_effect_document["set_effect"].GetArray();

    struct SetEffectData set_effect_data;
    for (int i = 0; i < set_effect_info.Size(); i++)
    {
        struct SetEffectData::SetEffect set_effect;
        set_effect.set_name = SafeGetString(set_effect_info[i], "set_name");
        set_effect.total_set_count = SafeGetString(set_effect_info[i], "total_set_count");
        set_effect_data.set_effects.push_back(set_effect);
    }

    DataManager->SetSetEffectData(set_effect_data);
#pragma endregion

#pragma region 링크 스킬
    rapidjson::Document link_skill_document = APIManager::GetInstance()->
        RequestLinkSkill(DataManager->GetOcid(), date_);
    rapidjson::Value& link_skill_info = link_skill_document["character_link_skill"].GetArray();

    DataManager->GetLinkSkillData().clear();

    for (int i = 0; i < link_skill_info.Size(); i++)
    {
        struct SkillData link_skill_data;
        link_skill_data.skill_name = SafeGetString(link_skill_info[i], "skill_name");
        link_skill_data.skill_description = SafeGetString(link_skill_info[i], "skill_description");
        link_skill_data.skill_level = SafeGetString(link_skill_info[i], "skill_level");
        link_skill_data.skill_effect = SafeGetString(link_skill_info[i], "skill_effect");

        std::string icon_url = SafeGetString(link_skill_info[i], "skill_icon");
        DownloadManager::GetInstance()->DownloadFile(icon_url, LINK_SKILL_ICON_PATH + std::to_string(i) + ".png");

        bool ret = Graphics::GetInstance()->LoadTexture(LINK_SKILL_ICON_PATH + std::to_string(i) + ".png",
                                                        &link_skill_data.icon, &link_skill_data.icon_width,
                                                        &link_skill_data.icon_height);

        IM_ASSERT(ret);
        
        DataManager->GetLinkSkillData().push_back(link_skill_data);
    }
#pragma endregion

#pragma region 스킬
    for (int i = 0; i < 2; i++)
    {
        DataManager::GetInstance()->skill_data[i].clear();
        
        std::string skill_grade = std::to_string(i + 5);
        
        rapidjson::Document skill_document = APIManager::GetInstance()->
            RequestSkill(DataManager->GetOcid(), date_, skill_grade);

        rapidjson::Value& skill_info = skill_document["character_skill"].GetArray();
        
        for (int j = 0; j < skill_info.Size(); j++)
        {
            struct SkillData skill_data;
            skill_data.skill_name = SafeGetString(skill_info[j], "skill_name");
            skill_data.skill_description = SafeGetString(skill_info[j], "skill_description");
            skill_data.skill_level = SafeGetString(skill_info[j], "skill_level");
            skill_data.skill_effect = SafeGetString(skill_info[j], "skill_effect");

            std::string icon_url = SafeGetString(skill_info[j], "skill_icon");
            DownloadManager::GetInstance()->DownloadFile(icon_url, SKILL_ICON_PATH + skill_grade + "\\" + std::to_string(j) + ".png");

            bool ret = Graphics::GetInstance()->LoadTexture(SKILL_ICON_PATH + skill_grade + "\\" + std::to_string(j) + ".png",
                                                            &skill_data.icon, &skill_data.icon_width,
                                                            &skill_data.icon_height);

            IM_ASSERT(ret);

            DataManager::GetInstance()->skill_data[i].push_back(skill_data);
        }
    }
#pragma endregion
}

void Scene::ShowLinkSkill(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(u8"링크 스킬", p_open))
    {
        ImGui::End();
        return;
    }

    for (auto& link_skill : DataManager::GetInstance()->GetLinkSkillData())
    {
        ImGui::Image(link_skill.icon, ImVec2(link_skill.icon_width, link_skill.icon_height));
        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::Text(u8"%s Lv.%s", link_skill.skill_name.c_str(), link_skill.skill_level.c_str());
        ImGui::TextWrapped(u8"%s", link_skill.skill_effect.c_str());
        ImGui::EndGroup();

        ImGui::Separator();
    }
    
    ImGui::End();
}

void Scene::ShowSkill(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(u8"스킬", p_open))
    {
        ImGui::End();
        return;
    }

    std::shared_ptr<DataManager> DataManager = DataManager::GetInstance();

    if (ImGui::BeginTabBar(u8"전직 차수"))
    {
        for (int i = 0; i < 2; i++)
        {
            std::string skill_grade = std::to_string(i + 5);

            if (ImGui::BeginTabItem((skill_grade + u8"차").c_str()))
            {
                ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

                for (auto& skill : DataManager->skill_data[i])
                {
                    ImGui::Image(skill.icon, ImVec2(skill.icon_width, skill.icon_height));
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
    ImGui::End();
}

std::string Scene::SafeGetString(const rapidjson::Value& value, const std::string& key)
{
    const rapidjson::Value::ConstMemberIterator iter = value.FindMember(key.c_str());
    
    if (iter != value.MemberEnd() && !iter->value.IsNull())
    {
        if (iter->value.IsString()) return iter->value.GetString();
        if (iter->value.IsInt()) return std::to_string(iter->value.GetInt());
        if (iter->value.IsDouble()) return std::to_string(iter->value.GetDouble());
        if (iter->value.IsBool()) return std::to_string(iter->value.GetBool());
    }
    
    return "";
}

ImVec4 Scene::GetColorByGrade(const std::string& grade)
{
    if (grade.compare(u8"레전드리") == 0)
    {
        return ImVec4(140.f / 255.f, 174.f / 255.f, 14.f / 255.f, 1.f);
    }

    if (grade.compare(u8"유니크") == 0)
    {
        return ImVec4(232.f / 255.f, 156.f / 255.f, 9.f / 255.f, 1.f);
    }

    if (grade.compare(u8"에픽") == 0)
    {
        return ImVec4(127.f / 255.f, 102.f / 255.f, 211.f / 255.f, 1.f);
    }

    if (grade.compare(u8"레어") == 0)
    {
        return ImVec4(54.f / 255.f, 184.f / 255.f, 208.f / 255.f, 1.f);
    }

    return ImVec4(1.f, 1.f, 1.f, 1.f);
}
