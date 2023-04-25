#pragma once
#define WIN32_LEAN_AND_MEAN
#define UNICODE

#include <windows.h>
#include <iostream>

class Window
{
    friend class Graphics;

    friend class Input;

    public:

    inline static bool hasFocus = true;

    static void Init();

    inline static WNDCLASSEXW windowClass = {};
    
    inline static HINSTANCE handleInstance = {};
    
    inline static HWND windowHandle = {};
};
