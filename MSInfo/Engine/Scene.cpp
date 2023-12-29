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
#define ITEM_EQUIP_PATH ".\\Temp\\Character\\ItemEquip\\"

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

        if (!DataManager->GetCharacterInfo().character_name.empty())
        {
            ImGui::Text(u8"%s (%s)\nLv.%s | %s | %s",
                DataManager->GetCharacterInfo().character_name.c_str(),
                DataManager->GetCharacterInfo().world_name.c_str(),
                DataManager->GetCharacterInfo().character_level.c_str(),
                DataManager->GetCharacterInfo().character_class.c_str(),
                DataManager->GetCharacterInfo().character_guild_name.c_str());
        }
        
        ImGui::Separator();

        if (!DataManager->GetStatInfo().min_stat_attack.empty())
        {
            ImGui::Text(u8"전투력");
            ImGui::SameLine();
            
            std::string combat_power = DataManager->GetStatInfo().combat_power;
            std::string format_unit = DataManager->FormatUnit(std::stol(combat_power));

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

                std::string min_stat_attack = DataManager->GetStatInfo().min_stat_attack;
                std::string max_stat_attack = DataManager->GetStatInfo().max_stat_attack;
                std::string format_min_stat_attack = DataManager->FormatComma(std::stol(min_stat_attack));
                std::string format_max_stat_attack = DataManager->FormatComma(std::stol(max_stat_attack));
                ImGui::Text(u8"%s ~ %s", format_min_stat_attack.c_str(), format_max_stat_attack.c_str());

                ImGui::EndTable();
            }
            
            if (ImGui::BeginTable("##캐릭터 정보_2", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"HP");

                ImGui::TableSetColumnIndex(1);
                
                std::string hp = DataManager->GetStatInfo().hp;
                std::string format_comma = DataManager->FormatComma(std::stol(hp));
                ImGui::Text(u8"%s", format_comma.c_str());
                
                ImGui::TableSetColumnIndex(2);
                ImGui::Text(u8"MP");

                ImGui::TableSetColumnIndex(3);

                std::string mp = DataManager->GetStatInfo().mp;
                format_comma = DataManager->FormatComma(std::stol(mp));
                ImGui::Text(u8"%s", format_comma.c_str());
                
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"STR");

                ImGui::TableSetColumnIndex(1);

                std::string stat_str = DataManager->GetStatInfo().stat_str;
                format_comma = DataManager->FormatComma(std::stol(stat_str));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::Text(u8"DEX");

                ImGui::TableSetColumnIndex(3);

                std::string stat_dex = DataManager->GetStatInfo().stat_dex;
                format_comma = DataManager->FormatComma(std::stol(stat_dex));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"INT");

                ImGui::TableSetColumnIndex(1);

                std::string stat_int = DataManager->GetStatInfo().stat_int;
                format_comma = DataManager->FormatComma(std::stol(stat_int));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::Text(u8"LUK");

                ImGui::TableSetColumnIndex(3);

                std::string stat_luk = DataManager->GetStatInfo().stat_luk;
                format_comma = DataManager->FormatComma(std::stol(stat_luk));
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
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"최종 데미지");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().final_damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"무기 숙련도");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().weapon_mastery.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"보스 몬스터 데미지");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().boss_damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"일반 몬스터 데미지");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().normal_monster_damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"방어율 무시");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().ignore_defense.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"공격력");

                ImGui::TableSetColumnIndex(1);
                std::string attack_power = DataManager->GetStatInfo().attack_power;
                std::string format_comma = DataManager->FormatComma(std::stol(attack_power));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"마력");

                ImGui::TableSetColumnIndex(1);
                std::string magic_power = DataManager->GetStatInfo().magic_power;
                format_comma = DataManager->FormatComma(std::stol(magic_power));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"방어력");

                ImGui::TableSetColumnIndex(1);
                std::string defense = DataManager->GetStatInfo().defense;
                format_comma = DataManager->FormatComma(std::stol(defense));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"크리티컬 확률");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().critical_rate.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"크리티컬 데미지");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().critical_damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"버프 지속 시간");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().buff_duration.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"재사용 대기시간 감소");

                ImGui::TableSetColumnIndex(1);
                std::string cooldown_reduction = DataManager->GetStatInfo().cooldown_reduction;
                std::string cooldown_reduction_percent = DataManager->GetStatInfo().cooldown_reduction_percent;
                ImGui::Text(u8"%s초 / %s%%", cooldown_reduction.c_str(), cooldown_reduction_percent.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"재사용 대기시간 미적용");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().cooldown_reduction_not_apply.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"속성 내성 무시");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().status_resistance_ignore.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"상태이상 내성");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s", DataManager->GetStatInfo().status_resistance.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"상태이상 추가 데미지");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().additional_damage.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"소환수 지속시간 증가");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().summon_duration.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"메소 획득량");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().meso_drop_rate.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"아이템 드롭률");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().item_drop_rate.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"추가 경험치 획득");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().additional_exp.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"공격속도");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s단계", DataManager->GetStatInfo().attack_speed.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"이동속도");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().speed.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"점프력");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().jump.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"스탠스");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text(u8"%s%%", DataManager->GetStatInfo().stance.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"스타포스");

                ImGui::TableSetColumnIndex(1);
                std::string star_force = DataManager->GetStatInfo().star_force;
                format_comma = DataManager->FormatComma(std::stol(star_force));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"아케인포스");

                ImGui::TableSetColumnIndex(1);
                std::string arcane_force = DataManager->GetStatInfo().arcane_force;
                format_comma = DataManager->FormatComma(std::stol(arcane_force));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"어센틱포스");

                ImGui::TableSetColumnIndex(1);
                std::string authentic_force = DataManager->GetStatInfo().authentic_force;
                format_comma = DataManager->FormatComma(std::stol(authentic_force));
                ImGui::Text(u8"%s", format_comma.c_str());
                
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 STR");

                ImGui::TableSetColumnIndex(1);
                std::string ap_str = DataManager->GetStatInfo().ap_str;
                format_comma = DataManager->FormatComma(std::stol(ap_str));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 DEX");

                ImGui::TableSetColumnIndex(1);
                std::string ap_dex = DataManager->GetStatInfo().ap_dex;
                format_comma = DataManager->FormatComma(std::stol(ap_dex));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 INT");

                ImGui::TableSetColumnIndex(1);
                std::string ap_int = DataManager->GetStatInfo().ap_int;
                format_comma = DataManager->FormatComma(std::stol(ap_int));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 LUK");

                ImGui::TableSetColumnIndex(1);
                std::string ap_luk = DataManager->GetStatInfo().ap_luk;
                format_comma = DataManager->FormatComma(std::stol(ap_luk));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 HP");

                ImGui::TableSetColumnIndex(1);
                std::string ap_hp = DataManager->GetStatInfo().ap_hp;
                format_comma = DataManager->FormatComma(std::stol(ap_hp));
                ImGui::Text(u8"%s", format_comma.c_str());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(u8"AP 배분 MP");

                ImGui::TableSetColumnIndex(1);
                std::string ap_mp = DataManager->GetStatInfo().ap_mp;
                format_comma = DataManager->FormatComma(std::stol(ap_mp));
                ImGui::Text(u8"%s", format_comma.c_str());
                
                ImGui::EndTable();
            }
            
            ImGui::EndChild();
        }

        if (ImGui::CollapsingHeader(u8"어빌리티"))
        {
        }
        
        if (ImGui::CollapsingHeader(u8"하이퍼 스텟"))
        {
        }
    }

    ImGui::End();
#pragma endregion
}

void Scene::SearchCharacter(const std::string& character_name)
{
    std::shared_ptr<DataManager> DataManager = DataManager::GetInstance();
    
#pragma region 캐릭터 식별자
    rapidjson::Document id_document = APIManager::GetInstance()->RequestID(character_name);
    DataManager->SetOcid(id_document["ocid"].GetString());
#pragma endregion

#pragma region 캐릭터 정보
    rapidjson::Document character_document = APIManager::GetInstance()->RequestCharacter(DataManager->GetOcid(), date_);
    
    std::string character_image_url = character_document["character_image"].GetString();
    DownloadManager::GetInstance()->DownloadFile(character_image_url, CHARACTER_IMAGE_PATH);
    
    bool ret = Graphics::GetInstance()->LoadTexture(CHARACTER_IMAGE_PATH, &character_image, &character_image_width, &character_image_height);
    IM_ASSERT(ret);

    struct CharacterData character_info = {
        character_document["character_name"].GetString(),
        character_document["world_name"].GetString(),
        character_document["character_class"].GetString(),
        std::to_string(character_document["character_level"].GetInt()),
        character_document["character_guild_name"].GetString()
    };

    DataManager->SetCharacterInfo(character_info);
#pragma endregion

#pragma region 스텟
    rapidjson::Document stat_document = APIManager::GetInstance()->RequestStat(DataManager->GetOcid(), date_);

    struct StatData stat_data = {
        stat_document["final_stat"][0]["stat_value"].GetString(),
        stat_document["final_stat"][1]["stat_value"].GetString(),
        stat_document["final_stat"][2]["stat_value"].GetString(),
        stat_document["final_stat"][3]["stat_value"].GetString(),
        stat_document["final_stat"][4]["stat_value"].GetString(),
        stat_document["final_stat"][5]["stat_value"].GetString(),
        stat_document["final_stat"][6]["stat_value"].GetString(),
        stat_document["final_stat"][7]["stat_value"].GetString(),
        stat_document["final_stat"][8]["stat_value"].GetString(),
        stat_document["final_stat"][9]["stat_value"].GetString(),
        stat_document["final_stat"][10]["stat_value"].GetString(),
        stat_document["final_stat"][11]["stat_value"].GetString(),
        stat_document["final_stat"][12]["stat_value"].GetString(),
        stat_document["final_stat"][13]["stat_value"].GetString(),
        stat_document["final_stat"][14]["stat_value"].GetString(),
        stat_document["final_stat"][15]["stat_value"].GetString(),
        stat_document["final_stat"][16]["stat_value"].GetString(),
        stat_document["final_stat"][17]["stat_value"].GetString(),
        stat_document["final_stat"][18]["stat_value"].GetString(),
        stat_document["final_stat"][19]["stat_value"].GetString(),
        stat_document["final_stat"][20]["stat_value"].GetString(),
        stat_document["final_stat"][21]["stat_value"].GetString(),
        stat_document["final_stat"][22]["stat_value"].GetString(),
        stat_document["final_stat"][23]["stat_value"].GetString(),
        stat_document["final_stat"][24]["stat_value"].GetString(),
        stat_document["final_stat"][25]["stat_value"].GetString(),
        stat_document["final_stat"][26]["stat_value"].GetString(),
        stat_document["final_stat"][27]["stat_value"].GetString(),
        stat_document["final_stat"][28]["stat_value"].GetString(),
        stat_document["final_stat"][29]["stat_value"].GetString(),
        stat_document["final_stat"][30]["stat_value"].GetString(),
        stat_document["final_stat"][31]["stat_value"].GetString(),
        stat_document["final_stat"][32]["stat_value"].GetString(),
        stat_document["final_stat"][33]["stat_value"].GetString(),
        stat_document["final_stat"][34]["stat_value"].GetString(),
        stat_document["final_stat"][35]["stat_value"].GetString(),
        stat_document["final_stat"][36]["stat_value"].GetString(),
        stat_document["final_stat"][37]["stat_value"].GetString(),
        stat_document["final_stat"][38]["stat_value"].GetString(),
        stat_document["final_stat"][39]["stat_value"].GetString(),
        stat_document["final_stat"][40]["stat_value"].GetString(),
        stat_document["final_stat"][41]["stat_value"].GetString(),
        stat_document["final_stat"][42]["stat_value"].GetString(),
        stat_document["final_stat"][43]["stat_value"].GetString()
    };

    DataManager->SetStatInfo(stat_data);
#pragma endregion
}
