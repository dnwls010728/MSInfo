#include "VersionWindow.h"

#include <vector>

#include "imgui/imgui.h"
#include "../Extension/ImGuiEx.h"

void VersionWindow::Draw()
{
    std::vector<std::string> log = {
        u8"v2.2",
        u8"자동 업데이트 기능 제거",
        u8"다중 실행 해제",
        u8"최적화 및 버그 수정",
        u8"창 활성화 여부 저장 기능 추가",
        u8"v2.1",
        u8"최적화",
        u8"인기도 정보 추가",
        u8"무릉도장 최고 기록 정보 추가",
        u8"v2.0",
        u8"캐시 장비 메뉴 추가",
        u8"헤어, 성형, 피부 추가",
        u8"유니온 메뉴 추가",
        u8"v1.15",
        u8"API Key 변경",
        u8"v1.1",
        u8"프로그램 최적화",
        u8"도움말 메뉴 추가"
    };

    for (int i = 0; i < log.size(); i++)
    {
        ImGui::AlignCenter(log[i]);
        ImGui::Text(u8"%s", log[i].c_str());

        if (i + 1 < log.size() - 1 && log[i + 1].find("v") != std::string::npos)
        {
            ImGui::Separator();
        }
    }
}
