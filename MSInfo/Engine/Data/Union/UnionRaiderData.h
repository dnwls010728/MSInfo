#pragma once
#include <string>
#include <vector>

#include "UnionBlockData.h"

struct UnionRaiderData
{
    struct UnionInnerStat
    {
        std::string stat_field_id;
        std::string stat_field_effect;
    };
    
    std::vector<std::string> union_raider_stat;
    std::vector<std::string> union_occupied_stat;
    std::vector<struct UnionBlockData> union_block;
    std::vector<UnionInnerStat> union_inner_stat;
};
