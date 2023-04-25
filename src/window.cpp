#include "window.h"
#include "input.h"
#include "settings.h"

void Window::Init()
{
    windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = &Input::WindowProc;
    windowClass.hInstance = handleInstance;
    windowClass.hIcon = LoadIconW(0, IDI_APPLICATION);
    windowClass.hCursor = LoadCursorW(0, IDC_ARROW);
    windowClass.lpszClassName = L"WindowClass";
    windowClass.hIconSm = LoadIconW(0, IDI_APPLICATION);

    if(!RegisterClassExW(&windowClass))
    {
        MessageBoxA(0, "RegisterClassEx failed", "Fatal Error", MB_OK);
    }
    
    RECT windRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};            // set the size, but not the position
    AdjustWindowRect(&windRect, WS_OVERLAPPEDWINDOW, FALSE);      // adjust the size

    windowHandle = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW,
                            windowClass.lpszClassName,
                            L"tilemap",
                            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            windRect.right - windRect.left,    // width of the window
                            windRect.bottom - windRect.top,    // height of the window
                            0,
                            0,
                            handleInstance,
                            0);

    if(!windowHandle)
    {
        MessageBoxA(0, "CreateWindowEx failed", "Fatal Error", MB_OK);
    }
}