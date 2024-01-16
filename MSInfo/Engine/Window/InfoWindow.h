#pragma once
#include "Window.h"

class InfoWindow : public Window
{
public:
    InfoWindow() = default;
    virtual ~InfoWindow() final = default;

    virtual void Draw() final;
    
};
