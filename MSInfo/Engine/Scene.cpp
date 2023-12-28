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

        if (ImGui::BeginTable("##캐릭터 정보", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text(u8"전투력");

            ImGui::TableSetColumnIndex(1);

            if (!DataManager->GetStatInfo().combat_power.empty())
            {
                std::string combat_power = DataManager->GetStatInfo().combat_power;
                std::string format_unit = DataManager->FormatUnit(std::stol(combat_power));
                ImGui::Text(u8"%s", format_unit.c_str());
            }

            ImGui::EndTable();
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
        std::to_string(character_document["character_level"].GetInt64()),
        character_document["character_guild_name"].GetString()
    };

    DataManager->SetCharacterInfo(character_info);
#pragma endregion

#pragma region 스텟
    rapidjson::Document stat_document = APIManager::GetInstance()->RequestStat(DataManager->GetOcid(), date_);

    struct StatData stat_data = {
        stat_document["final_stat"][42]["stat_value"].GetString(),
    };

    DataManager->SetStatInfo(stat_data);
#pragma endregion
}
