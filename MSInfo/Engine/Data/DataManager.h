#pragma once

#include <vector>

#include "../Singleton.h"
#include "CharacterData.h"
#include "StatData.h"
#include "AbilityData.h"

class DataManager : public Singleton<DataManager>
{
public:
    DataManager();
    virtual ~DataManager() final = default;

    std::string GetDataDate();
    std::string FormatUnit(int val);
    std::string FormatComma(long val);
    
    std::vector<struct AbilityData> ability_info;

    inline void SetOcid(const std::string& ocid) { ocid_ = ocid; }
    inline std::string GetOcid() { return ocid_; }

    inline void SetCharacterInfo(const struct CharacterData& character_info) { character_info_ = character_info; }
    inline struct CharacterData GetCharacterInfo() { return character_info_; }

    inline void SetStatInfo(const struct StatData& stat_info) { stat_info_ = stat_info; }
    inline struct StatData GetStatInfo() { return stat_info_; }

private:
    std::string ocid_;
    
    struct CharacterData character_info_;
    struct StatData stat_info_;
    
};
