#include "UnionRaiderWindow.h"

#include "../ResourceManager.h"
#include "../Data/DataManager.h"
#include "imgui/imgui.h"
#include "../Extension/ImGuiEx.h"

void UnionRaiderWindow::Draw()
{
    struct UnionData& union_data = DataManager::GetInstance()->GetUnionData();

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

    ImGui::AlignCenter(union_data.union_grade);
    ImGui::Text(u8"%s", union_data.union_grade.c_str());

    ImGui::PopFont();

    ImGui::AlignCenter(union_data.union_level);
    ImGui::Text(u8"Lv.%s", union_data.union_level.c_str());

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    const ImVec2 window_size = ImGui::GetWindowSize();
    const ImVec2 p = ImGui::GetCursorScreenPos();

    const float union_board_width = 308;
    const float union_board_height = 280;
    const float left_padding = (window_size.x - union_board_width) / 2;
    const float top_padding = 14;

    ImU32 col_a = ImGui::GetColorU32(IM_COL32(70, 70, 93, 0));
    ImU32 col_b = ImGui::GetColorU32(IM_COL32(39, 40, 46, 255));
    draw_list->AddRectFilledMultiColor(ImVec2(p.x + left_padding, p.y + top_padding),
                                       ImVec2(p.x + (left_padding + union_board_width),
                                              p.y + (top_padding + union_board_height)), col_b, col_b, col_a, col_a);

    draw_list->AddImage(ResourceManager::GetInstance()->GetTexture("UnionBoard")->texture, ImVec2(p.x + left_padding, p.y + top_padding),
                        ImVec2(p.x + (left_padding + union_board_width), p.y + (top_padding + union_board_height)));

    struct UnionRaiderData& union_raider_data = DataManager::GetInstance()->GetUnionRaiderData();
    for (auto& union_block : union_raider_data.union_block)
    {
        if (union_block.block_color == 0)
        {
            ImU32 rand_col = ImGui::GetColorU32(IM_COL32(rand() % 255, rand() % 255, rand() % 255, 255));
            union_block.block_color = rand_col;
        }

        for (int i = 0; i < union_block.block_position.size(); i++)
        {
            float x = 11 + std::stof(union_block.block_position[i].x);
            float y = 10 + std::stof(union_block.block_position[i].y);

            ImVec2 position = ImVec2((left_padding + 7) + (14 * (1 * x)), (top_padding - 7) + (14 * (1 * y)));
            UnionBlock(draw_list, position, union_block.block_color);

            if (i == union_block.block_position.size() - 1)
            {
                float mouse_x = p.x + position.x;
                float mouse_y = p.y + position.y;

                draw_list->AddCircle(ImVec2(mouse_x, mouse_y), 2, IM_COL32(255, 0, 0, 255), 16, 2.f);

                bool is_hovered = ImGui::IsMouseHoveringRect(ImVec2(mouse_x - 8, mouse_y - 8),
                                                             ImVec2(mouse_x + 8, mouse_y + 8));
                if (is_hovered)
                {
                    ImGui::BeginTooltip();
                    ImGui::Text(u8"%s Lv.%s", union_block.block_class.c_str(), union_block.block_level.c_str());
                    ImGui::EndTooltip();
                }
            }
        }
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + top_padding + union_board_height + 10);
    ImGui::Separator();

    ImGui::BeginGroup();
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImGui::Text(u8"공격대원 효과");
    ImGui::PopFont();
    ImGui::BeginChild("Union Raider Stat", ImVec2(window_size.x / 2, 0), false,
                      ImGuiWindowFlags_AlwaysVerticalScrollbar);
    for (auto& union_raider_stat : union_raider_data.union_raider_stat)
    {
        ImGui::Text(u8"%s", union_raider_stat.c_str());
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImGui::Text(u8"공격대 점령 효과");
    ImGui::PopFont();
    ImGui::BeginChild("Union Occupied Stat", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    for (auto& union_occupied_stat : union_raider_data.union_occupied_stat)
    {
        ImGui::Text(u8"%s", union_occupied_stat.c_str());
    }
    ImGui::EndChild();
    ImGui::EndGroup();
}

void UnionRaiderWindow::UnionBlock(ImDrawList* draw_list, ImVec2 position, int col)
{
    const ImVec2 p = ImGui::GetCursorScreenPos();

    float x = p.x + position.x;
    float y = p.y + position.y;

    draw_list->AddRectFilled(ImVec2(x - 7, y - 7), ImVec2(x + 7, y + 7), col);
}
