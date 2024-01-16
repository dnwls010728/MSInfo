#pragma once
#include "Window.h"

class ItemEquipmentWindow : public Window
{
public:
    ItemEquipmentWindow() = default;
    virtual ~ItemEquipmentWindow() final = default;

    virtual void Draw() final;
    
};
