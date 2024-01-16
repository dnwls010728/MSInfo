#pragma once
#include <string>

class Window
{
public:
    Window() = default;
    virtual ~Window() = default;

    void Open(const std::string& title, bool* p_open);
    virtual void Draw() = 0;
    
};
