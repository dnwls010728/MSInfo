#define _CRTDBG_MAP_ALLOC

#include "AutoUpdater.h"
#include "Engine/Core.h"

#include <crtdbg.h>

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    if (lpCmdLine != nullptr && wcscmp(lpCmdLine, L"update") != 0)
    {
        return 0;
    }
    
    HANDLE mutex_handle = CreateMutex(nullptr, FALSE, L"AutoUpdater");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(mutex_handle);
        MessageBox(Core::GetInstance()->GetWindowHandle(), L"이미 업데이트 중입니다.", L"Error", MB_OK);
        return 0;
    }

    if (!Core::GetInstance()->InitWindow(hInstance, nCmdShow)) return 0;

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}