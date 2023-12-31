#pragma once

#include <string>
#include <d3d11.h>

struct LinkSkillData
{
    std::string skill_name;
    std::string skill_description;
    std::string skill_level;
    std::string skill_effect;

    int icon_width;
    int icon_height;

    ID3D11ShaderResourceView* icon;
};
