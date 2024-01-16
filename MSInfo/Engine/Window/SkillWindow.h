#pragma once
#include "Window.h"

class SkillWindow : public Window
{
public:
    SkillWindow() = default;
    virtual ~SkillWindow() final = default;

    virtual void Draw() final;
    
};
