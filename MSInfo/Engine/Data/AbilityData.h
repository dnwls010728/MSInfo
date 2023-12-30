#pragma once
#include <string>

struct AbilityData
{
    struct Ability
    {
        std::string ability_grade;
        std::string ability_value;
    };
    
    std::string ability_grade;
    std::vector<Ability> abilities;
};
