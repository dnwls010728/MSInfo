#pragma once
#include <string>
#include <vector>

struct UnionBlockData
{
    struct BlockPoint
    {
        std::string x;
        std::string y;
    };

    std::string block_type;
    std::string block_class;
    std::string block_level;
    
    struct BlockPoint block_control_point;
    
    std::vector<BlockPoint> block_position;
};
