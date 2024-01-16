#pragma once
#include <d3d11.h>

struct Texture
{
    int width;
    int height;

    ID3D11ShaderResourceView* texture;
};
