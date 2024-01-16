#pragma once
#include "Window.h"

class VersionWindow : public Window
{
public:
    VersionWindow() = default;
    virtual ~VersionWindow() final = default;

    virtual void Draw() final;
    
};
