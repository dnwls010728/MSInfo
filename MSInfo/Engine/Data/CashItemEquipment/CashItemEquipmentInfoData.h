#pragma once
#include <string>
#include <vector>

struct CashItemEquipmentInfoData
{
    struct CashItemOption
    {
        std::string option_type;
        std::string option_value;
    };

    struct CashItemColoringPrism
    {
        std::string color_range;
        std::string hue;
        std::string saturation;
        std::string value;
    };

    struct Texture cash_item_texture;

    std::string cash_item_equipment_part;
    std::string cash_item_equipment_slot;
    std::string cash_item_name;
    std::string cash_item_description;
    std::string date_expire;
    std::string date_option_expire;
    std::string cash_item_label;
    std::string base_preset_item_disable_flag;

    struct CashItemColoringPrism cash_item_coloring_prism;

    std::vector<struct CashItemOption> cash_item_option;
};
