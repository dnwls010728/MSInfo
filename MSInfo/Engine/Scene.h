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
    
    inline std::string GetDate() const { return date_; }

    inline int* GetCharacterImageWidth() { return &character_image_width; }
    inline int* GetCharacterImageHeight() { return &character_image_height; }

    inline ID3D11ShaderResourceView** GetCharacterImage() { return &character_image; }

    inline void SetSearchContent(const std::string& search_content) { search_content_ = search_content; }
    inline std::string GetSearchContent() const { return search_content_; }

    inline void SetProgress(float progress) { progress_ = progress; }
    inline float GetProgress() const { return progress_; }

private:
    void SetAlignCenter(std::string text);
    void DrawOption(std::string option, std::string total, std::string base, std::string add, std::string etc, std::string starforce, bool is_percent = false);
    void SearchCharacter(const std::string& character_name);
    void ShowLinkSkill(bool* p_open);
    void ShowSkill(bool* p_open);
    void ShowEquipment(bool* p_open);
    void ShowVersion(bool* p_open);
    void ShowInfo(bool* p_open);

    std::string SafeGetString(const rapidjson::Value& value, const std::string& key);

    class ImVec4 GetColorByGrade(const std::string& grade);

    static DWORD WINAPI SearchThread(LPVOID lpParam);

    std::string search_content_;
    std::string date_;

    int character_image_width;
    int character_image_height;
    int star_icon_width;
    int star_icon_height;

    ID3D11ShaderResourceView* character_image;
    ID3D11ShaderResourceView* star_icon;

    float progress_ = 0.f;

    HANDLE thread_handle_;

    std::string character_name_;
    
};
