#pragma once
#include <string>
#include <vector>

struct HyperStatData
{
    struct HyperStat
    {
        std::string stat_type;
        std::string stat_point;
        std::string stat_level;
        std::string stat_increase;
    };
    
    std::string use_preset_no;
    std::vector<HyperStat> preset_1;
    std::vector<HyperStat> preset_2;
    std::vector<HyperStat> preset_3;
};
