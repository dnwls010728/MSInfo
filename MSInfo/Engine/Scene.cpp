#include "Scene.h"

#include "DataManager.h"
#include "API/APIManager.h"
#include "API/DownloadManager.h"
#include "Graphics/Graphics.h"
#include "imgui/imgui.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

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

            std::string character_image_url = DataManager::GetInstance()->character_document["character_image"].GetString();
            DownloadManager::GetInstance()->DownloadFile(character_image_url, ".\\Temp\\Character\\character_image.png");
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

    bool ret = Graphics::GetInstance()->LoadTexture(".\\Temp\\Character\\character_image.png", &character_image, &character_image_width, &character_image_height);
    IM_ASSERT(ret);
    
    DataManager::GetInstance()->character_document = std::move(character_document);
#pragma endregion
}
