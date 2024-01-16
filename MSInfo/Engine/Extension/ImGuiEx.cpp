#include "ImGuiEx.h"

#include "imgui/imgui.h"

void ImGui::AlignCenter(std::string text)
{
    float window_width = GetWindowSize().x;
    float text_width = CalcTextSize(text.c_str()).x;
    float text_pos_x = (window_width - text_width) * .5f;

    SetCursorPosX(text_pos_x);
}
