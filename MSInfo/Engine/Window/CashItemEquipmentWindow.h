#pragma once
#include "Window.h"

class CashItemEquipmentWindow : public Window
{
public:
    CashItemEquipmentWindow() = default;
    virtual ~CashItemEquipmentWindow() final = default;

    virtual void Draw() final;
    
};
