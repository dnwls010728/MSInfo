#pragma once
#include <vector>

#include "ItemEquipmentInfoData.h"
#include "TitleData.h"

struct ItemEquipmentData
{
    std::vector<struct ItemEquipmentInfoData> item_equipment;
    std::vector<struct ItemEquipmentInfoData> dragon_equipment;
    std::vector<struct ItemEquipmentInfoData> mechanic_equipment;

    struct TitleData title;
};
