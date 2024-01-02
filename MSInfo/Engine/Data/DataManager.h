#pragma once

#include <vector>

#include "../Singleton.h"
#include "CharacterData.h"
#include "StatData.h"
#include "AbilityData.h"
#include "HyperStatData.h"
#include "SetEffectData.h"
#include "SkillData.h"

class DataManager : public Singleton<DataManager>
{
public:
    DataManager();
    virtual ~DataManager() final = default;

    std::string GetDataDate();
    std::string FormatUnit(int value);
    std::string FormatComma(long value);
    std::string SafeFormatUnit(std::string value);
    std::string SafeFormatComma(std::string value);

    inline void SetOcid(const std::string& ocid) { ocid_ = ocid; }
    inline std::string GetOcid() { return ocid_; }

    inline void SetCharacterData(const struct CharacterData& character_data) { character_data_ = character_data; }
    inline struct CharacterData GetCharacterData() { return character_data_; }

    inline void SetStatData(const struct StatData& stat_data) { stat_data_ = stat_data; }
    inline struct StatData GetStatData() { return stat_data_; }

    inline void SetAbilityData(const struct AbilityData& ability_data) { ability_data_ = ability_data; }
    inline struct AbilityData GetAbilityData() { return ability_data_; }

    inline void SetHyperStatData(const struct HyperStatData& hyper_stat_data) { hyper_stat_data_ = hyper_stat_data; }
    inline struct HyperStatData GetHyperStatData() { return hyper_stat_data_; }

    inline void SetSetEffectData(const struct SetEffectData& set_effect_data) { set_effect_data_ = set_effect_data; }
    inline struct SetEffectData GetSetEffectData() { return set_effect_data_; }

    inline std::vector<struct SkillData>& GetLinkSkillData() { return link_skill_data_; }
    
    std::vector<struct SkillData> skill_data[2];

private:
    std::string ocid_;
    
    struct CharacterData character_data_;
    struct StatData stat_data_;
    struct AbilityData ability_data_;
    struct HyperStatData hyper_stat_data_;
    struct SetEffectData set_effect_data_;

    std::vector<struct SkillData> link_skill_data_;
    
};
