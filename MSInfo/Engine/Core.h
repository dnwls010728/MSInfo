#pragma once
#include "Singleton.h"
#include "../framework.h"

class Core : public Singleton<Core>
{
public:
    Core();
    virtual ~Core() final = default;

    ATOM MyRegisterClass(HINSTANCE hInstance);
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

    bool InitWindow(HINSTANCE hInstance, int nCmdShow);

private:
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    LPCWSTR class_name_;

    POINT resolution_;
    RECT window_area_;

    HWND hWnd_;
    
};
