#pragma once
#include <d3d11.h>
#include <shared_mutex>

#include "Singleton.h"
#include "Texture.h"
#include "rapidjson/document.h"

class Scene : public Singleton<Scene>
{
public:
    Scene();
    virtual ~Scene() final = default;

    virtual void Release() final;
    
    void Tick(float delta_time);
    void Render();
    
    class ImVec4 GetColorByGrade(const std::string& grade);

    inline bool IsSearching() const { return is_searching_; }

    inline void SetProgress(float progress) { progress_ = progress; }
    inline float GetProgress() const { return progress_; }

private:
    void SearchCharacter(const std::string& character_name);

    std::string SafeGetString(const rapidjson::Value& value, const std::string& key);

    static DWORD WINAPI SearchThread(LPVOID lpParam);

    bool is_searching_ = false;
    
    bool show_link_skill_ = false;
    bool show_skill_ = false;
    bool show_item_equipment_ = false;
    bool show_cash_item_equipment_ = false;
    bool show_union_raider_ = false;
    bool show_info_ = false;
    bool show_version_ = false;

    std::string search_content_;
    std::string date_;

    struct Texture character_texture_;

    float progress_ = 0.f;

    HANDLE thread_handle_;

    std::string character_name_;

    std::unique_ptr<class LinkSkillWindow> link_skill_window_;
    std::unique_ptr<class SkillWindow> skill_window_;
    std::unique_ptr<class ItemEquipmentWindow> item_equipment_window_;
    std::unique_ptr<class CashItemEquipmentWindow> cash_item_equipment_window_;
    std::unique_ptr<class UnionRaiderWindow> union_raider_window_;
    std::unique_ptr<class VersionWindow> version_window_;
    std::unique_ptr<class InfoWindow> info_window_;
    
};
