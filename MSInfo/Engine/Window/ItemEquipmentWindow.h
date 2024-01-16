#pragma once
#include "Window.h"

class ItemEquipmentWindow : public Window
{
public:
    ItemEquipmentWindow() = default;
    virtual ~ItemEquipmentWindow() final = default;

    virtual void Draw() final;

    void DrawOption(std::string option, std::string total, std::string base, std::string add, std::string etc, std::string starforce, bool is_percent = false);
    
};
