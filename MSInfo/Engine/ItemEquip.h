#pragma once
#include <d3d11.h>

#include "rapidjson/document.h"

class ItemEquip
{
public:
    int image_width;
    int image_height;

    ID3D11ShaderResourceView* image;
    
    rapidjson::Value item_info;
};
