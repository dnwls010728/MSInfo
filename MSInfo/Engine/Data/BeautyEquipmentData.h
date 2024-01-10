#pragma once
#include <string>

struct BeautyEquipmentData
{
    struct BeautyEquipment
    {
        std::string name;
        std::string base_color;
        std::string mix_color;
        std::string mix_rate;
    };

    struct BeautyEquipment character_hair;
    struct BeautyEquipment character_face;
    struct BeautyEquipment additional_character_hair;
    struct BeautyEquipment additional_character_face;
    
    std::string character_skin_name;
    std::string additional_character_skin_name;
};
