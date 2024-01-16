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

    inline bool IsSearching() const { return is_searching_; }

    inline void SetProgress(float progress) { progress_ = progress; }
    inline float GetProgress() const { return progress_; }

private:
    void SetAlignCenter(std::string text);
    void DrawOption(std::string option, std::string total, std::string base, std::string add, std::string etc, std::string starforce, bool is_percent = false);
    void SearchCharacter(const std::string& character_name);
    void ShowItemEquipment(bool* p_open);
    void ShowCashItemEquipment(bool* p_open);
    void UnionRaider(bool* p_open);
    void ShowVersion(bool* p_open);
    void ShowInfo(bool* p_open);
    void UnionBlock(struct ImDrawList* draw_list, struct ImVec2 position, signed int col);

    std::string SafeGetString(const rapidjson::Value& value, const std::string& key);

    class ImVec4 GetColorByGrade(const std::string& grade);

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
    struct Texture star_texture_;
    struct Texture union_board_texture_;

    float progress_ = 0.f;

    HANDLE thread_handle_;

    std::string character_name_;

    std::unique_ptr<class LinkSkillWindow> link_skill_window_;
    std::unique_ptr<class SkillWindow> skill_window_;
    
};
