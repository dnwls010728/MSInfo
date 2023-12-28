#include "Scene.h"

#include <string>

#include "DataManager.h"
#include "API/APIManager.h"
#include "API/DownloadManager.h"
#include "Graphics/Graphics.h"
#include "ItemEquip.h"

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

        if (DataManager::GetInstance()->character_document != rapidjson::Document())
        {
            std::string character_name = DataManager::GetInstance()->character_document["character_name"].GetString();
            std::string world_name = DataManager::GetInstance()->character_document["world_name"].GetString();
            std::string character_class = DataManager::GetInstance()->character_document["character_class"].GetString();
            std::string character_level = std::to_string(DataManager::GetInstance()->character_document["character_level"].GetInt());
            std::string character_guild_name = DataManager::GetInstance()->character_document["character_guild_name"].GetString();
            
            ImGui::Text(u8"%s (%s)\nLv.%s | %s | %s",
                character_name.c_str(),
                world_name.c_str(),
                character_level.c_str(),
                character_class.c_str(),
                character_guild_name.c_str());
        }

        ImGui::Separator();
    }

    ImGui::End();
#pragma endregion

#pragma region 장비
    if (ImGui::Begin(u8"장비"))
    {
        for (int i = 0; i < DataManager::GetInstance()->items.size(); i++)
        {
            std::unique_ptr<ItemEquip>& item = DataManager::GetInstance()->items[i];
            ImGui::Image(item->image, ImVec2(item->image_width, item->image_height));
        }
    }
    
    ImGui::End();
#pragma endregion
}

void Scene::SearchCharacter(const std::string& character_name)
{
#pragma region 캐릭터 식별자
    rapidjson::Document id_document = APIManager::GetInstance()->RequestID(character_name);
    DataManager::GetInstance()->id_document = std::move(id_document);
#pragma endregion

#pragma region 캐릭터 정보
    rapidjson::Document character_document = APIManager::GetInstance()->RequestCharacter(DataManager::GetInstance()->id_document["ocid"].GetString(), date_);
    
    std::string character_image_url = character_document["character_image"].GetString();
    DownloadManager::GetInstance()->DownloadFile(character_image_url, CHARACTER_IMAGE_PATH);
    
    bool ret = Graphics::GetInstance()->LoadTexture(CHARACTER_IMAGE_PATH, &character_image, &character_image_width, &character_image_height);
    IM_ASSERT(ret);
    
    DataManager::GetInstance()->character_document = std::move(character_document);
#pragma endregion
    
#pragma region 장비
    rapidjson::Document equipment_document = APIManager::GetInstance()->RequestItemEquip(DataManager::GetInstance()->id_document["ocid"].GetString(), date_);
    
    const int size = equipment_document["item_equipment"].Size();
    rapidjson::Value& item_equipment = equipment_document["item_equipment"].GetArray();

    DataManager::GetInstance()->items.clear();
    
    for (int i = 0; i < size; i++)
    {
        rapidjson::Value& item = item_equipment[i];
    
        std::unique_ptr<ItemEquip> item_equip = std::make_unique<ItemEquip>();
    
        std::string item_image_url = item["item_shape_icon"].GetString();
        DownloadManager::GetInstance()->DownloadFile(item_image_url, ITEM_EQUIP_PATH + std::to_string(i) + ".png");
    
        bool ret = Graphics::GetInstance()->LoadTexture(ITEM_EQUIP_PATH + std::to_string(i) + ".png", &item_equip->image, &item_equip->image_width, &item_equip->image_height);
        IM_ASSERT(ret);
        
        item_equip->item_info = std::move(item);
    
        DataManager::GetInstance()->items.push_back(std::move(item_equip));
    }
#pragma endregion
}
