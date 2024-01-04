#pragma once
#include <string>
#include <vector>

#include "UnionBlockPointData.h"

struct UnionBlockData
{
    std::string block_type;
    std::string block_class;
    std::string block_level;
    
    struct UnionBlockPointData block_control_point;
    
    std::vector<UnionBlockPointData> block_position;
};
