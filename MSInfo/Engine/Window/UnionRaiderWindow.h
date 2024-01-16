#pragma once
#include "Window.h"

class UnionRaiderWindow : public Window
{
public:
    UnionRaiderWindow() = default;
    virtual ~UnionRaiderWindow() final = default;

    virtual void Draw() final;

    void UnionBlock(struct ImDrawList* draw_list, struct ImVec2 position, signed int col);
    
};
