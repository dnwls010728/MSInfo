#pragma once
#include <string>
#include <vector>

struct SetEffectData
{
    struct SetEffect
    {
        std::string set_name;
        std::string total_set_count;
    };

    std::vector<SetEffect> set_effects;
};
