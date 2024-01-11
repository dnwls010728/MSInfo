#pragma once
#include <string>
#include <vector>

struct UnionBlockData
{
    struct BlockPosition
    {
        std::string x;
        std::string y;
    };
    
    std::string block_type;
    std::string block_class;
    std::string block_level;

    struct BlockPosition block_control_point;
    std::vector<struct BlockPosition> block_position;
};
