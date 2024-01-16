#pragma once
#include "Window.h"

class LinkSkillWindow : public Window
{
public:
    LinkSkillWindow() = default;
    virtual ~LinkSkillWindow() final = default;

    virtual void Draw() final;
    
};
