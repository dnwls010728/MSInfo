﻿#pragma once
#include <vector>

#include "../Singleton.h"
#include "CharacterData.h"
#include "DojangData.h"
#include "StatData.h"
#include "AbilityData.h"
#include "HyperStatData.h"
#include "SetEffectData.h"
#include "SkillData.h"
#include "ItemEquipment/ItemEquipmentData.h"
#include "CashItemEquipment/CashItemEquipmentData.h"
#include "BeautyEquipmentData.h"
#include "Union/UnionData.h"
#include "Union/UnionRaiderData.h"

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

    inline void SetPopularity(const std::string& popularity) { popularity_ = popularity; }
    inline std::string GetPopularity() { return popularity_; }

    inline struct CharacterData& GetCharacterData() { return character_data_; }
    inline struct DojangData& GetDojangData() { return dojang_data_; }
    inline struct StatData& GetStatData() { return stat_data_; }
    inline struct AbilityData& GetAbilityData() { return ability_data_; }
    inline struct HyperStatData& GetHyperStatData() { return hyper_stat_data_; }
    inline struct SetEffectData& GetSetEffectData() { return set_effect_data_; }
    inline struct ItemEquipmentData& GetItemEquipmentData() { return item_equipment_data_; }
    inline struct CashItemEquipmentData& GetCashItemEquipmentData() { return cash_item_equipment_data_; }
    inline struct BeautyEquipmentData& GetBeautyEquipmentData() { return beauty_equipment_data_; }
    inline struct UnionData& GetUnionData() { return union_data_; }
    inline struct UnionRaiderData& GetUnionRaiderData() { return union_raider_data_; }
    
    inline std::vector<struct SkillData>& GetLinkSkillData() { return link_skill_data_; }
    
    std::vector<struct SkillData> skill_data[2];

private:
    std::string ocid_;
    std::string popularity_;
    
    struct CharacterData character_data_;
    struct DojangData dojang_data_;
    struct StatData stat_data_;
    struct AbilityData ability_data_;
    struct HyperStatData hyper_stat_data_;
    struct SetEffectData set_effect_data_;
    struct ItemEquipmentData item_equipment_data_;
    struct CashItemEquipmentData cash_item_equipment_data_;
    struct BeautyEquipmentData beauty_equipment_data_;
    struct UnionData union_data_;
    struct UnionRaiderData union_raider_data_;

    std::vector<struct SkillData> link_skill_data_;
    
};
