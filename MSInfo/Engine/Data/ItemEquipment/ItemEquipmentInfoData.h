#pragma once
#include <string>

#include "ItemTotalOptionData.h"
#include "ItemBaseOptionData.h"
#include "ItemExceptionalOptionData.h"
#include "ItemAddOptionData.h"
#include "ItemEtcOptionData.h"
#include "ItemStarforceOptionData.h"

struct ItemEquipmentInfoData
{
    int icon_width;
    int icon_height;

    ID3D11ShaderResourceView* icon;
    
    std::string item_equipment_part;
    std::string item_equipment_slot;
    std::string item_name;
    std::string item_description;
    std::string item_shape_name;
    std::string item_gender;
    std::string potential_option_grade;
    std::string additional_potential_option_grade;
    std::string potential_option_1;
    std::string potential_option_2;
    std::string potential_option_3;
    std::string additional_potential_option_1;
    std::string additional_potential_option_2;
    std::string additional_potential_option_3;
    std::string equipment_level_increase;
    std::string growth_exp;
    std::string growth_level;
    std::string scroll_upgrade;
    std::string cuttable_count;
    std::string golden_hammer_flag;
    std::string scroll_resilience_count;
    std::string scroll_upgradeable_count;
    std::string soul_name;
    std::string soul_option;
    std::string starforce;
    std::string starforce_scroll_flag;
    std::string special_ring_level;
    std::string date_expire;
    
    struct ItemTotalOptionData item_total_option;
    struct ItemBaseOptionData item_base_option;
    struct ItemExceptionalOptionData item_exceptional_option;
    struct ItemAddOptionData item_add_option;
    struct ItemEtcOptionData item_etc_option;
    struct ItemStarforceOptionData item_starforce_option;
};
