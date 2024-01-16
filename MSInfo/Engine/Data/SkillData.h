#pragma once

#include <string>

#include "../Texture.h"

struct SkillData
{
    std::string skill_name;
    std::string skill_description;
    std::string skill_level;
    std::string skill_effect;

    struct Texture skill_texture;
};
