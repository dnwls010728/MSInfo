#pragma once
#include "CashItemEquipmentInfoData.h"

struct CashItemEquipmentData
{
    std::string preset_no;

    std::vector<struct CashItemEquipmentInfoData> presets[3];
};
