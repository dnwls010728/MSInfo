#include "Scene.h"

#include <string>

#include "Data/DataManager.h"
#include "API/APIManager.h"
#include "API/DownloadManager.h"
#include "Graphics/Graphics.h"

#include "imgui/imgui.h"

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

#include "Data/ItemEquipment/ItemEquipmentInfoData.h"
#include "Data/ItemEquipment/ItemTotalOptionData.h"
#include "Data/ItemEquipment/ItemBaseOptionData.h"
#include "Data/ItemEquipment/ItemExceptionalOptionData.h"
#include "Data/ItemEquipment/ItemAddOptionData.h"

#define CHARACTER_IMAGE_PATH ".\\Temp\\Character\\character_image.png"
#define LINK_SKILL_ICON_PATH ".\\Temp\\Icon\\LinkSkill\\"
#define SKILL_ICON_PATH ".\\Temp\\Icon\\Skill\\"
#define ITEM_EQUIPMENT_ICON_PATH ".\\Temp\\Icon\\ItemEquipment\\"
#define RESOURCES ".\\Resources\\"

#define UPGRADE() ImVec4(102.f / 255.f, 1.f, 1.f, 1.f)
#define ADD_OPTION() ImVec4(204.f / 255.f, 1.f, 0.f, 1.f);
#define ETC_OPTION() ImVec4(170.f / 255.f, 170.f / 255.f, 1.f, 1.f);
#define STARFORCE_OPTION() ImVec4(1.f, 204.f / 255.f, 0.f, 1.f);

Scene::Scene()
{
    date_ = DataManager::GetInstance()->GetDataDate();

    bool ret = Graphics::GetInstance()->LoadTexture(RESOURCES "star.png", &star_icon, &star_icon_width, &star_icon_height);
    IM_ASSERT(ret);
}

void Scene::Tick(float delta_time)
{
}

void Scene::Render()
{
    std::shared_ptr<DataManager> DataManager = DataManager::GetInstance();

    static bool show_link_skill = false;
    static bool show_skill = false;
    static bool show_equipment = false;

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
            ImGui::MenuItem(u8"장비", nullptr, &show_equipment);
            
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
    if (show_equipment) ShowEquipment(&show_equipment);
}

void Scene::SetAlignCenter(std::string text)
{
    float window_width = ImGui::GetWindowSize().x;
    float text_width = ImGui::CalcTextSize(text.c_str()).x;
    float text_pos_x = (window_width - text_width) * .5f;

    ImGui::SetCursorPosX(text_pos_x);
}

void Scene::DrawOption(std::string option, std::string total, std::string base, std::string add, std::string etc,
    std::string starforce, bool is_percent)
{
    if (total.compare("0") == 0) return;
    
    ImVec4 upgrade_color = UPGRADE();
    ImVec4 add_option_color = ADD_OPTION();
    ImVec4 etc_option_color = ETC_OPTION();
    ImVec4 starforce_option_color = STARFORCE_OPTION();

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

    DataManager->GetCharacterData().character_name = SafeGetString(character_document, "character_name");
    DataManager->GetCharacterData().world_name = SafeGetString(character_document, "world_name");
    DataManager->GetCharacterData().character_class = SafeGetString(character_document, "character_class");
    DataManager->GetCharacterData().character_level = SafeGetString(character_document, "character_level");
    DataManager->GetCharacterData().character_guild_name = SafeGetString(character_document, "character_guild_name");
#pragma endregion

#pragma region 스텟
    rapidjson::Document stat_document = APIManager::GetInstance()->RequestStat(DataManager->GetOcid(), date_);

    DataManager->GetStatData().min_stat_attack = SafeGetString(stat_document["final_stat"][0], "stat_value");
    DataManager->GetStatData().max_stat_attack = SafeGetString(stat_document["final_stat"][1], "stat_value");
    DataManager->GetStatData().damage = SafeGetString(stat_document["final_stat"][2], "stat_value");
    DataManager->GetStatData().boss_damage = SafeGetString(stat_document["final_stat"][3], "stat_value");
    DataManager->GetStatData().final_damage = SafeGetString(stat_document["final_stat"][4], "stat_value");
    DataManager->GetStatData().ignore_defense = SafeGetString(stat_document["final_stat"][5], "stat_value");
    DataManager->GetStatData().critical_rate = SafeGetString(stat_document["final_stat"][6], "stat_value");
    DataManager->GetStatData().critical_damage = SafeGetString(stat_document["final_stat"][7], "stat_value");
    DataManager->GetStatData().status_resistance = SafeGetString(stat_document["final_stat"][8], "stat_value");
    DataManager->GetStatData().stance = SafeGetString(stat_document["final_stat"][9], "stat_value");
    DataManager->GetStatData().defense = SafeGetString(stat_document["final_stat"][10], "stat_value");
    DataManager->GetStatData().speed = SafeGetString(stat_document["final_stat"][11], "stat_value");
    DataManager->GetStatData().jump = SafeGetString(stat_document["final_stat"][12], "stat_value");
    DataManager->GetStatData().star_force = SafeGetString(stat_document["final_stat"][13], "stat_value");
    DataManager->GetStatData().arcane_force = SafeGetString(stat_document["final_stat"][14], "stat_value");
    DataManager->GetStatData().authentic_force = SafeGetString(stat_document["final_stat"][15], "stat_value");
    DataManager->GetStatData().stat_str = SafeGetString(stat_document["final_stat"][16], "stat_value");
    DataManager->GetStatData().stat_dex = SafeGetString(stat_document["final_stat"][17], "stat_value");
    DataManager->GetStatData().stat_int = SafeGetString(stat_document["final_stat"][18], "stat_value");
    DataManager->GetStatData().stat_luk = SafeGetString(stat_document["final_stat"][19], "stat_value");
    DataManager->GetStatData().hp = SafeGetString(stat_document["final_stat"][20], "stat_value");
    DataManager->GetStatData().mp = SafeGetString(stat_document["final_stat"][21], "stat_value");
    DataManager->GetStatData().ap_str = SafeGetString(stat_document["final_stat"][22], "stat_value");
    DataManager->GetStatData().ap_dex = SafeGetString(stat_document["final_stat"][23], "stat_value");
    DataManager->GetStatData().ap_int = SafeGetString(stat_document["final_stat"][24], "stat_value");
    DataManager->GetStatData().ap_luk = SafeGetString(stat_document["final_stat"][25], "stat_value");
    DataManager->GetStatData().ap_hp = SafeGetString(stat_document["final_stat"][26], "stat_value");
    DataManager->GetStatData().ap_mp = SafeGetString(stat_document["final_stat"][27], "stat_value");
    DataManager->GetStatData().item_drop_rate = SafeGetString(stat_document["final_stat"][28], "stat_value");
    DataManager->GetStatData().meso_drop_rate = SafeGetString(stat_document["final_stat"][29], "stat_value");
    DataManager->GetStatData().buff_duration = SafeGetString(stat_document["final_stat"][30], "stat_value");
    DataManager->GetStatData().attack_speed = SafeGetString(stat_document["final_stat"][31], "stat_value");
    DataManager->GetStatData().normal_monster_damage = SafeGetString(stat_document["final_stat"][32], "stat_value");
    DataManager->GetStatData().cooldown_reduction = SafeGetString(stat_document["final_stat"][33], "stat_value");
    DataManager->GetStatData().cooldown_reduction_percent = SafeGetString(stat_document["final_stat"][34], "stat_value");
    DataManager->GetStatData().cooldown_reduction_not_apply = SafeGetString(stat_document["final_stat"][35], "stat_value");
    DataManager->GetStatData().status_resistance_ignore = SafeGetString(stat_document["final_stat"][36], "stat_value");
    DataManager->GetStatData().additional_damage = SafeGetString(stat_document["final_stat"][37], "stat_value");
    DataManager->GetStatData().weapon_mastery = SafeGetString(stat_document["final_stat"][38], "stat_value");
    DataManager->GetStatData().additional_exp = SafeGetString(stat_document["final_stat"][39], "stat_value");
    DataManager->GetStatData().attack_power = SafeGetString(stat_document["final_stat"][40], "stat_value");
    DataManager->GetStatData().magic_power = SafeGetString(stat_document["final_stat"][41], "stat_value");
    DataManager->GetStatData().combat_power = SafeGetString(stat_document["final_stat"][42], "stat_value");
    DataManager->GetStatData().summon_duration = SafeGetString(stat_document["final_stat"][43], "stat_value");
#pragma endregion

#pragma region 어빌리티
    rapidjson::Document ability_document = APIManager::GetInstance()->RequestAbility(DataManager->GetOcid(), date_);
    rapidjson::Value& ability_info = ability_document["ability_info"].GetArray();

    DataManager->GetAbilityData().ability_grade = SafeGetString(ability_document, "ability_grade");
    DataManager->GetAbilityData().abilities.clear();

    for (int i = 0; i < ability_info.Size(); i++)
    {
        struct AbilityData::Ability ability;
        ability.ability_grade = SafeGetString(ability_info[i], "ability_grade");
        ability.ability_value = SafeGetString(ability_info[i], "ability_value");

        DataManager->GetAbilityData().abilities.push_back(ability);
    }

#pragma endregion

#pragma region 하이퍼 스텟
    rapidjson::Document hyper_stat_document = APIManager::GetInstance()->
        RequestHyperStat(DataManager->GetOcid(), date_);
    rapidjson::Value& hyper_stat_preset_1 = hyper_stat_document["hyper_stat_preset_1"].GetArray();
    rapidjson::Value& hyper_stat_preset_2 = hyper_stat_document["hyper_stat_preset_2"].GetArray();
    rapidjson::Value& hyper_stat_preset_3 = hyper_stat_document["hyper_stat_preset_3"].GetArray();

    DataManager->GetHyperStatData().use_preset_no = SafeGetString(hyper_stat_document, "use_preset_no");
    DataManager->GetHyperStatData().preset_1.clear();
    DataManager->GetHyperStatData().preset_2.clear();
    DataManager->GetHyperStatData().preset_3.clear();

    for (int i = 0; i < hyper_stat_preset_1.Size(); i++)
    {
        struct HyperStatData::HyperStat hyper_stat;
        hyper_stat.stat_type = SafeGetString(hyper_stat_preset_1[i], "stat_type");
        hyper_stat.stat_point = SafeGetString(hyper_stat_preset_1[i], "stat_point");
        hyper_stat.stat_level = SafeGetString(hyper_stat_preset_1[i], "stat_level");
        hyper_stat.stat_increase = SafeGetString(hyper_stat_preset_1[i], "stat_increase");

        DataManager->GetHyperStatData().preset_1.push_back(hyper_stat);
    }

    for (int i = 0; i < hyper_stat_preset_2.Size(); i++)
    {
        struct HyperStatData::HyperStat hyper_stat;
        hyper_stat.stat_type = SafeGetString(hyper_stat_preset_2[i], "stat_type");
        hyper_stat.stat_point = SafeGetString(hyper_stat_preset_2[i], "stat_point");
        hyper_stat.stat_level = SafeGetString(hyper_stat_preset_2[i], "stat_level");
        hyper_stat.stat_increase = SafeGetString(hyper_stat_preset_2[i], "stat_increase");

        DataManager->GetHyperStatData().preset_2.push_back(hyper_stat);
    }

    for (int i = 0; i < hyper_stat_preset_3.Size(); i++)
    {
        struct HyperStatData::HyperStat hyper_stat;
        hyper_stat.stat_type = SafeGetString(hyper_stat_preset_3[i], "stat_type");
        hyper_stat.stat_point = SafeGetString(hyper_stat_preset_3[i], "stat_point");
        hyper_stat.stat_level = SafeGetString(hyper_stat_preset_3[i], "stat_level");
        hyper_stat.stat_increase = SafeGetString(hyper_stat_preset_3[i], "stat_increase");

        DataManager->GetHyperStatData().preset_3.push_back(hyper_stat);
    }

#pragma endregion

#pragma region 세트 장비
    rapidjson::Document set_effect_document = APIManager::GetInstance()->
        RequestSetEffect(DataManager->GetOcid(), date_);
    rapidjson::Value& set_effect_info = set_effect_document["set_effect"].GetArray();

    DataManager->GetSetEffectData().set_effects.clear();

    for (int i = 0; i < set_effect_info.Size(); i++)
    {
        struct SetEffectData::SetEffect set_effect;
        set_effect.set_name = SafeGetString(set_effect_info[i], "set_name");
        set_effect.total_set_count = SafeGetString(set_effect_info[i], "total_set_count");

        DataManager->GetSetEffectData().set_effects.push_back(set_effect);
    }
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

#pragma region 장비
    rapidjson::Document equipment_document = APIManager::GetInstance()->
        RequestItemEquipment(DataManager->GetOcid(), date_);

    rapidjson::Value& item_equipment_info = equipment_document["item_equipment"].GetArray();

    DataManager->GetItemEquipmentData().item_equipment.clear();
    DataManager->GetItemEquipmentData().dragon_equipment.clear();
    DataManager->GetItemEquipmentData().mechanic_equipment.clear();
    
    for (int i = 0; i < item_equipment_info.Size(); i++)
    {
        struct ItemEquipmentInfoData item_equipment_info_data;
        item_equipment_info_data.item_equipment_part = SafeGetString(item_equipment_info[i], "item_equipment_part");
        item_equipment_info_data.item_equipment_slot = SafeGetString(item_equipment_info[i], "item_equipment_slot");
        item_equipment_info_data.item_name = SafeGetString(item_equipment_info[i], "item_name");
        item_equipment_info_data.item_description = SafeGetString(item_equipment_info[i], "item_description");
        item_equipment_info_data.item_shape_name = SafeGetString(item_equipment_info[i], "item_shape_name");

        std::string item_icon_url = SafeGetString(item_equipment_info[i], "item_shape_icon");
        DownloadManager::GetInstance()->DownloadFile(item_icon_url, ITEM_EQUIPMENT_ICON_PATH + std::to_string(i) + ".png");

        bool ret = Graphics::GetInstance()->LoadTexture(ITEM_EQUIPMENT_ICON_PATH + std::to_string(i) + ".png",
                                                        &item_equipment_info_data.icon,
                                                        &item_equipment_info_data.icon_width,
                                                        &item_equipment_info_data.icon_height);

        IM_ASSERT(ret);
        
        item_equipment_info_data.item_gender = SafeGetString(item_equipment_info[i], "item_gender");

        item_equipment_info_data.item_total_option.stat_str = SafeGetString(item_equipment_info[i]["item_total_option"], "str");
        item_equipment_info_data.item_total_option.stat_dex = SafeGetString(item_equipment_info[i]["item_total_option"], "dex");
        item_equipment_info_data.item_total_option.stat_int = SafeGetString(item_equipment_info[i]["item_total_option"], "int");
        item_equipment_info_data.item_total_option.stat_luk = SafeGetString(item_equipment_info[i]["item_total_option"], "luk");
        item_equipment_info_data.item_total_option.max_hp = SafeGetString(item_equipment_info[i]["item_total_option"], "max_hp");
        item_equipment_info_data.item_total_option.max_mp = SafeGetString(item_equipment_info[i]["item_total_option"], "max_mp");
        item_equipment_info_data.item_total_option.attack_power = SafeGetString(item_equipment_info[i]["item_total_option"], "attack_power");
        item_equipment_info_data.item_total_option.magic_power = SafeGetString(item_equipment_info[i]["item_total_option"], "magic_power");
        item_equipment_info_data.item_total_option.armor = SafeGetString(item_equipment_info[i]["item_total_option"], "armor");
        item_equipment_info_data.item_total_option.speed = SafeGetString(item_equipment_info[i]["item_total_option"], "speed");
        item_equipment_info_data.item_total_option.jump = SafeGetString(item_equipment_info[i]["item_total_option"], "jump");
        item_equipment_info_data.item_total_option.boss_damage = SafeGetString(item_equipment_info[i]["item_total_option"], "boss_damage");
        item_equipment_info_data.item_total_option.ignore_monster_armor = SafeGetString(item_equipment_info[i]["item_total_option"], "ignore_monster_armor");
        item_equipment_info_data.item_total_option.all_stat = SafeGetString(item_equipment_info[i]["item_total_option"], "all_stat");
        item_equipment_info_data.item_total_option.damage = SafeGetString(item_equipment_info[i]["item_total_option"], "damage");
        item_equipment_info_data.item_total_option.equipment_level_decrease = SafeGetString(item_equipment_info[i]["item_total_option"], "equipment_level_decrease");
        item_equipment_info_data.item_total_option.max_hp_rate = SafeGetString(item_equipment_info[i]["item_total_option"], "max_hp_rate");
        item_equipment_info_data.item_total_option.max_mp_rate = SafeGetString(item_equipment_info[i]["item_total_option"], "max_mp_rate");

        item_equipment_info_data.item_base_option.stat_str = SafeGetString(item_equipment_info[i]["item_base_option"], "str");
        item_equipment_info_data.item_base_option.stat_dex = SafeGetString(item_equipment_info[i]["item_base_option"], "dex");
        item_equipment_info_data.item_base_option.stat_int = SafeGetString(item_equipment_info[i]["item_base_option"], "int");
        item_equipment_info_data.item_base_option.stat_luk = SafeGetString(item_equipment_info[i]["item_base_option"], "luk");
        item_equipment_info_data.item_base_option.max_hp = SafeGetString(item_equipment_info[i]["item_base_option"], "max_hp");
        item_equipment_info_data.item_base_option.max_mp = SafeGetString(item_equipment_info[i]["item_base_option"], "max_mp");
        item_equipment_info_data.item_base_option.attack_power = SafeGetString(item_equipment_info[i]["item_base_option"], "attack_power");
        item_equipment_info_data.item_base_option.magic_power = SafeGetString(item_equipment_info[i]["item_base_option"], "magic_power");
        item_equipment_info_data.item_base_option.armor = SafeGetString(item_equipment_info[i]["item_base_option"], "armor");
        item_equipment_info_data.item_base_option.speed = SafeGetString(item_equipment_info[i]["item_base_option"], "speed");
        item_equipment_info_data.item_base_option.jump = SafeGetString(item_equipment_info[i]["item_base_option"], "jump");
        item_equipment_info_data.item_base_option.boss_damage = SafeGetString(item_equipment_info[i]["item_base_option"], "boss_damage");
        item_equipment_info_data.item_base_option.ignore_monster_armor = SafeGetString(item_equipment_info[i]["item_base_option"], "ignore_monster_armor");
        item_equipment_info_data.item_base_option.all_stat = SafeGetString(item_equipment_info[i]["item_base_option"], "all_stat");
        item_equipment_info_data.item_base_option.max_hp_rate = SafeGetString(item_equipment_info[i]["item_base_option"], "max_hp_rate");
        item_equipment_info_data.item_base_option.max_mp_rate = SafeGetString(item_equipment_info[i]["item_base_option"], "max_mp_rate");
        item_equipment_info_data.item_base_option.base_equipment_level = SafeGetString(item_equipment_info[i]["item_base_option"], "base_equipment_level");

        item_equipment_info_data.potential_option_grade = SafeGetString(item_equipment_info[i], "potential_option_grade");
        item_equipment_info_data.potential_option_1 = SafeGetString(item_equipment_info[i], "potential_option_1");
        item_equipment_info_data.potential_option_2 = SafeGetString(item_equipment_info[i], "potential_option_2");
        item_equipment_info_data.potential_option_3 = SafeGetString(item_equipment_info[i], "potential_option_3");
        item_equipment_info_data.additional_potential_option_grade = SafeGetString(item_equipment_info[i], "additional_potential_option_grade");
        item_equipment_info_data.additional_potential_option_1 = SafeGetString(item_equipment_info[i], "additional_potential_option_1");
        item_equipment_info_data.additional_potential_option_2 = SafeGetString(item_equipment_info[i], "additional_potential_option_2");
        item_equipment_info_data.additional_potential_option_3 = SafeGetString(item_equipment_info[i], "additional_potential_option_3");
        item_equipment_info_data.equipment_level_increase = SafeGetString(item_equipment_info[i], "equipment_level_increase");

        item_equipment_info_data.item_exceptional_option.stat_str = SafeGetString(item_equipment_info[i]["item_exceptional_option"], "str");
        item_equipment_info_data.item_exceptional_option.stat_dex = SafeGetString(item_equipment_info[i]["item_exceptional_option"], "dex");
        item_equipment_info_data.item_exceptional_option.stat_int = SafeGetString(item_equipment_info[i]["item_exceptional_option"], "int");
        item_equipment_info_data.item_exceptional_option.stat_luk = SafeGetString(item_equipment_info[i]["item_exceptional_option"], "luk");
        item_equipment_info_data.item_exceptional_option.max_hp = SafeGetString(item_equipment_info[i]["item_exceptional_option"], "max_hp");
        item_equipment_info_data.item_exceptional_option.max_mp = SafeGetString(item_equipment_info[i]["item_exceptional_option"], "max_mp");
        item_equipment_info_data.item_exceptional_option.attack_power = SafeGetString(item_equipment_info[i]["item_exceptional_option"], "attack_power");
        item_equipment_info_data.item_exceptional_option.magic_power = SafeGetString(item_equipment_info[i]["item_exceptional_option"], "magic_power");

        item_equipment_info_data.item_add_option.stat_str = SafeGetString(item_equipment_info[i]["item_add_option"], "str");
        item_equipment_info_data.item_add_option.stat_dex = SafeGetString(item_equipment_info[i]["item_add_option"], "dex");
        item_equipment_info_data.item_add_option.stat_int = SafeGetString(item_equipment_info[i]["item_add_option"], "int");
        item_equipment_info_data.item_add_option.stat_luk = SafeGetString(item_equipment_info[i]["item_add_option"], "luk");
        item_equipment_info_data.item_add_option.max_hp = SafeGetString(item_equipment_info[i]["item_add_option"], "max_hp");
        item_equipment_info_data.item_add_option.max_mp = SafeGetString(item_equipment_info[i]["item_add_option"], "max_mp");
        item_equipment_info_data.item_add_option.attack_power = SafeGetString(item_equipment_info[i]["item_add_option"], "attack_power");
        item_equipment_info_data.item_add_option.magic_power = SafeGetString(item_equipment_info[i]["item_add_option"], "magic_power");
        item_equipment_info_data.item_add_option.armor = SafeGetString(item_equipment_info[i]["item_add_option"], "armor");
        item_equipment_info_data.item_add_option.speed = SafeGetString(item_equipment_info[i]["item_add_option"], "speed");
        item_equipment_info_data.item_add_option.jump = SafeGetString(item_equipment_info[i]["item_add_option"], "jump");
        item_equipment_info_data.item_add_option.boss_damage = SafeGetString(item_equipment_info[i]["item_add_option"], "boss_damage");
        item_equipment_info_data.item_add_option.damage = SafeGetString(item_equipment_info[i]["item_add_option"], "damage");
        item_equipment_info_data.item_add_option.all_stat = SafeGetString(item_equipment_info[i]["item_add_option"], "all_stat");
        item_equipment_info_data.item_add_option.equipment_level_decrease = SafeGetString(item_equipment_info[i]["item_add_option"], "equipment_level_decrease");

        item_equipment_info_data.growth_exp = SafeGetString(item_equipment_info[i], "growth_exp");
        item_equipment_info_data.growth_level = SafeGetString(item_equipment_info[i], "growth_level");
        item_equipment_info_data.scroll_upgrade = SafeGetString(item_equipment_info[i], "scroll_upgrade");
        item_equipment_info_data.cuttable_count = SafeGetString(item_equipment_info[i], "cuttable_count");
        item_equipment_info_data.scroll_resilience_count = SafeGetString(item_equipment_info[i], "scroll_resilience_count");
        item_equipment_info_data.scroll_upgradeable_count = SafeGetString(item_equipment_info[i], "scroll_upgradeable_count");
        item_equipment_info_data.soul_name = SafeGetString(item_equipment_info[i], "soul_name");
        item_equipment_info_data.soul_option = SafeGetString(item_equipment_info[i], "soul_option");

        item_equipment_info_data.item_etc_option.stat_str = SafeGetString(item_equipment_info[i]["item_etc_option"], "str");
        item_equipment_info_data.item_etc_option.stat_dex = SafeGetString(item_equipment_info[i]["item_etc_option"], "dex");
        item_equipment_info_data.item_etc_option.stat_int = SafeGetString(item_equipment_info[i]["item_etc_option"], "int");
        item_equipment_info_data.item_etc_option.stat_luk = SafeGetString(item_equipment_info[i]["item_etc_option"], "luk");
        item_equipment_info_data.item_etc_option.max_hp = SafeGetString(item_equipment_info[i]["item_etc_option"], "max_hp");
        item_equipment_info_data.item_etc_option.max_mp = SafeGetString(item_equipment_info[i]["item_etc_option"], "max_mp");
        item_equipment_info_data.item_etc_option.attack_power = SafeGetString(item_equipment_info[i]["item_etc_option"], "attack_power");
        item_equipment_info_data.item_etc_option.magic_power = SafeGetString(item_equipment_info[i]["item_etc_option"], "magic_power");
        item_equipment_info_data.item_etc_option.armor = SafeGetString(item_equipment_info[i]["item_etc_option"], "armor");
        item_equipment_info_data.item_etc_option.speed = SafeGetString(item_equipment_info[i]["item_etc_option"], "speed");
        item_equipment_info_data.item_etc_option.jump = SafeGetString(item_equipment_info[i]["item_etc_option"], "jump");

        item_equipment_info_data.starforce = SafeGetString(item_equipment_info[i], "starforce");
        item_equipment_info_data.starforce_scroll_flag = SafeGetString(item_equipment_info[i], "starforce_scroll_flag");

        item_equipment_info_data.item_starforce_option.stat_str = SafeGetString(item_equipment_info[i]["item_starforce_option"], "str");
        item_equipment_info_data.item_starforce_option.stat_dex = SafeGetString(item_equipment_info[i]["item_starforce_option"], "dex");
        item_equipment_info_data.item_starforce_option.stat_int = SafeGetString(item_equipment_info[i]["item_starforce_option"], "int");
        item_equipment_info_data.item_starforce_option.stat_luk = SafeGetString(item_equipment_info[i]["item_starforce_option"], "luk");
        item_equipment_info_data.item_starforce_option.max_hp = SafeGetString(item_equipment_info[i]["item_starforce_option"], "max_hp");
        item_equipment_info_data.item_starforce_option.max_mp = SafeGetString(item_equipment_info[i]["item_starforce_option"], "max_mp");
        item_equipment_info_data.item_starforce_option.attack_power = SafeGetString(item_equipment_info[i]["item_starforce_option"], "attack_power");
        item_equipment_info_data.item_starforce_option.magic_power = SafeGetString(item_equipment_info[i]["item_starforce_option"], "magic_power");
        item_equipment_info_data.item_starforce_option.armor = SafeGetString(item_equipment_info[i]["item_starforce_option"], "armor");
        item_equipment_info_data.item_starforce_option.speed = SafeGetString(item_equipment_info[i]["item_starforce_option"], "speed");
        item_equipment_info_data.item_starforce_option.jump = SafeGetString(item_equipment_info[i]["item_starforce_option"], "jump");

        item_equipment_info_data.special_ring_level = SafeGetString(item_equipment_info[i], "special_ring_level");
        item_equipment_info_data.date_expire = SafeGetString(item_equipment_info[i], "date_expire");

        DataManager::GetInstance()->GetItemEquipmentData().item_equipment.push_back(item_equipment_info_data);
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
                ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

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

void Scene::ShowEquipment(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(u8"장비", p_open))
    {
        ImGui::End();
        return;
    }

    struct ItemEquipmentData& item_equipment_data = DataManager::GetInstance()->GetItemEquipmentData();
    for (auto& item_equipment : item_equipment_data.item_equipment)
    {
        ImGui::Image(item_equipment.icon, ImVec2(item_equipment.icon_width, item_equipment.icon_height));
        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::Text(u8"%s", item_equipment.item_name.c_str());

        if (item_equipment.starforce.compare("0") != 0)
        {
            ImGui::Image(star_icon, ImVec2(16, 16));
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
                ImGui::Image(star_icon, ImVec2(16, 16));
                ImGui::SameLine();
                ImGui::Text(u8"%s", item_equipment.starforce.c_str());
            }
            
            std::string item_name = item_equipment.item_name + "(+" + item_equipment.scroll_upgrade + ")";
            SetAlignCenter(item_name);
            ImGui::Text(u8"%s", item_name.c_str());

            if (!item_equipment.potential_option_grade.empty())
            {
                std::string item_grade = "(" + item_equipment.potential_option_grade + u8" 아이템)";
                SetAlignCenter(item_grade);
                ImGui::Text(u8"%s", item_grade.c_str());
            }
            
            ImGui::Separator();
            ImGui::Image(item_equipment.icon, ImVec2(item_equipment.icon_width, item_equipment.icon_height));
            ImGui::Separator();
            ImGui::Text(u8"장비분류: %s", item_equipment.item_equipment_part.c_str());

            DrawOption(u8"STR", item_equipment.item_total_option.stat_str, item_equipment.item_base_option.stat_str, item_equipment.item_add_option.stat_str, item_equipment.item_etc_option.stat_str, item_equipment.item_starforce_option.stat_str);
            DrawOption(u8"DEX", item_equipment.item_total_option.stat_dex, item_equipment.item_base_option.stat_dex, item_equipment.item_add_option.stat_dex, item_equipment.item_etc_option.stat_dex, item_equipment.item_starforce_option.stat_dex);
            DrawOption(u8"INT", item_equipment.item_total_option.stat_int, item_equipment.item_base_option.stat_int, item_equipment.item_add_option.stat_int, item_equipment.item_etc_option.stat_int, item_equipment.item_starforce_option.stat_int);
            DrawOption(u8"LUK", item_equipment.item_total_option.stat_luk, item_equipment.item_base_option.stat_luk, item_equipment.item_add_option.stat_luk, item_equipment.item_etc_option.stat_luk, item_equipment.item_starforce_option.stat_luk);
            DrawOption(u8"최대 HP", item_equipment.item_total_option.max_hp, item_equipment.item_base_option.max_hp, item_equipment.item_add_option.max_hp, item_equipment.item_etc_option.max_hp, item_equipment.item_starforce_option.max_hp);
            DrawOption(u8"최대 MP", item_equipment.item_total_option.max_mp, item_equipment.item_base_option.max_mp, item_equipment.item_add_option.max_mp, item_equipment.item_etc_option.max_mp, item_equipment.item_starforce_option.max_mp);
            DrawOption(u8"최대 HP", item_equipment.item_total_option.max_hp_rate, item_equipment.item_base_option.max_hp_rate, "0", "0", "0", true);
            DrawOption(u8"최대 MP", item_equipment.item_total_option.max_mp_rate, item_equipment.item_base_option.max_mp_rate, "0", "0", "0", true);
            DrawOption(u8"공격력", item_equipment.item_total_option.attack_power, item_equipment.item_base_option.attack_power, item_equipment.item_add_option.attack_power, item_equipment.item_etc_option.attack_power, item_equipment.item_starforce_option.attack_power);
            DrawOption(u8"마력", item_equipment.item_total_option.magic_power, item_equipment.item_base_option.magic_power, item_equipment.item_add_option.magic_power, item_equipment.item_etc_option.magic_power, item_equipment.item_starforce_option.magic_power);
            DrawOption(u8"방어력", item_equipment.item_total_option.armor, item_equipment.item_base_option.armor, item_equipment.item_add_option.armor, item_equipment.item_etc_option.armor, item_equipment.item_starforce_option.armor);
            DrawOption(u8"이동속도", item_equipment.item_total_option.speed, item_equipment.item_base_option.speed, item_equipment.item_add_option.speed, item_equipment.item_etc_option.speed, item_equipment.item_starforce_option.speed);
            DrawOption(u8"점프력", item_equipment.item_total_option.jump, item_equipment.item_base_option.jump, item_equipment.item_add_option.jump, item_equipment.item_etc_option.jump, item_equipment.item_starforce_option.jump);
            DrawOption(u8"보스 몬스터 공격 시 데미지", item_equipment.item_total_option.boss_damage, item_equipment.item_base_option.boss_damage, item_equipment.item_add_option.boss_damage, "0", "0", true);
            DrawOption(u8"몬스터 방어율 무시", item_equipment.item_total_option.ignore_monster_armor, item_equipment.item_base_option.ignore_monster_armor, "0", "0", "0", true);
            DrawOption(u8"올스탯", item_equipment.item_total_option.all_stat, item_equipment.item_base_option.all_stat, item_equipment.item_add_option.all_stat, "0", "0", true);
            DrawOption(u8"데미지", item_equipment.item_total_option.damage, "0", item_equipment.item_add_option.damage, "0", "0", true);

            if (!item_equipment.potential_option_grade.empty())
            {
                ImGui::Separator();
                
                ImVec4 potential_value_color = GetColorByGrade(item_equipment.potential_option_grade);
                ImGui::TextColored(potential_value_color, u8"잠재옵션");

                if (!item_equipment.potential_option_1.empty()) ImGui::Text(u8"%s", item_equipment.potential_option_1.c_str());
                if (!item_equipment.potential_option_2.empty()) ImGui::Text(u8"%s", item_equipment.potential_option_2.c_str());
                if (!item_equipment.potential_option_3.empty()) ImGui::Text(u8"%s", item_equipment.potential_option_3.c_str());
            }

            if (!item_equipment.additional_potential_option_grade.empty())
            {
                ImGui::Separator();
                
                ImVec4 additional_potential_value_color = GetColorByGrade(item_equipment.additional_potential_option_grade);
                ImGui::TextColored(additional_potential_value_color, u8"에디셔널 잠재옵션");

                if (!item_equipment.additional_potential_option_1.empty()) ImGui::Text(u8"%s", item_equipment.additional_potential_option_1.c_str());
                if (!item_equipment.additional_potential_option_2.empty()) ImGui::Text(u8"%s", item_equipment.additional_potential_option_2.c_str());
                if (!item_equipment.additional_potential_option_3.empty()) ImGui::Text(u8"%s", item_equipment.additional_potential_option_3.c_str());
            }

            if (item_equipment.item_name.compare(item_equipment.item_shape_name) != 0)
            {
                ImVec4 add_option_color = ADD_OPTION();
                
                ImGui::Separator();
                // ImGui::Text(u8"신비의 모루에 의해 [%s]의 외형이 합성됨", item_equipment.item_shape_name.c_str());
                ImGui::TextColored(add_option_color, u8"신비의 모루에 의해 [%s]의 외형이 합성됨", item_equipment.item_shape_name.c_str());
            }
            
            ImGui::EndTooltip();
        }

        ImGui::Separator();
    }

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
