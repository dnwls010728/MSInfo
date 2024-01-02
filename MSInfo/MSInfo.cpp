#define _CRTDBG_MAP_ALLOC

#include <Windows.h>

#include "MSInfo.h"
#include "Engine/Core.h"

#include <crtdbg.h>

#include "Engine/API/APIManager.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    HANDLE mutex_handle = CreateMutex(nullptr, FALSE, L"MSInfo");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(mutex_handle);
        MessageBox(Core::GetInstance()->GetWindowHandle(), L"이미 실행 중입니다.", L"Error", MB_OK);
        return 0;
    }

    std::string version = "1.0";
    
    rapidjson::Document version_document = APIManager::GetInstance()->Request("https://my-bundle-bucket.s3.ap-northeast-2.amazonaws.com/version.json");
    std::string latest_version = version_document["version"].GetString();
    if (version.compare(latest_version) != 0)
    {
        MessageBox(Core::GetInstance()->GetWindowHandle(), L"최신 버전이 아닙니다. 업데이트를 진행합니다.", L"Update", MB_OK);
        ShellExecute(Core::GetInstance()->GetWindowHandle(), L"open", L"AutoUpdater.exe", L"update", nullptr, SW_SHOW);
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
