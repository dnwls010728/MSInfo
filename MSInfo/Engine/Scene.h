#pragma once
#include <d3d11.h>

#include "Singleton.h"
#include "rapidjson/document.h"

class Scene : public Singleton<Scene>
{
public:
    Scene();
    virtual ~Scene() final = default;
    
    void Tick(float delta_time);
    void Render();

private:
    void SetAlignCenter(std::string text);
    void DrawOption(std::string option, std::string total, std::string base, std::string add, std::string etc, std::string starforce, bool is_percent = false);
    void SearchCharacter(const std::string& character_name);
    void ShowLinkSkill(bool* p_open);
    void ShowSkill(bool* p_open);
    void ShowEquipment(bool* p_open);

    std::string SafeGetString(const rapidjson::Value& value, const std::string& key);

    class ImVec4 GetColorByGrade(const std::string& grade);
    
    std::string date_;

    int character_image_width;
    int character_image_height;
    int star_icon_width;
    int star_icon_height;

    ID3D11ShaderResourceView* character_image;
    ID3D11ShaderResourceView* star_icon;
    
};
