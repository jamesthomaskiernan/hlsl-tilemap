#include "input.h"
#include "window.h"
#include <algorithm>
#include "application.h"

void Input::Update()
{
    MSG message = {};
    
    // TODO: make a queue which stores key changes, so you don't have to clear the whole array;
    //       this is pretty inefficient

    // update keyboard array
    for (int i = 0; i < 256; i ++)
    {
        if (keyboard[i].downNow)
        {
            keyboard[i].downNow = false;
        }
         
        else if (keyboard[i].up)
        {
            keyboard[i].up = false;
        }
    }

    // update mouse array
    for (int i = 0; i < 3; i++)
    {
        if (mouse[i].downNow)
        {
            mouse[i].downNow = false;
        }
         
        else if (mouse[i].up)
        {
            mouse[i].up = false;
        }
    }

    // update scroll wheel
    if (mouseWheel != 0)
    {
        mouseWheel = 0;
    }

    // dispatch for new messages
    while(PeekMessageW(&message, 0, 0, 0, PM_REMOVE))
    {
        if(message.message == WM_QUIT)
            Application::Kill();
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

bool Input::GetKey(char key)
{
    return keyboard[key].down;
}

bool Input::GetKeyDown(char key)
{
    return keyboard[key].downNow;
}

bool Input::GetKeyUp(char key)
{
    return keyboard[key].up;
}

bool Input::GetMouseButton(int button)
{
    return mouse[button].down;
}

bool Input::GetMouseButtonDown(int button)
{
    return mouse[button].downNow;
}

bool Input::GetMouseButtonUp(int button)
{
    return mouse[button].up;
}

int Input::GetMouseScroll()
{
    return mouseWheel;
}

float2 Input::GetMousePosition()
{
    // get mouse cursor position on monitor
    POINT result;
    GetCursorPos(&result);

    // convert that to position on window
    ScreenToClient(Window::windowHandle, &result);
    
    // do this conversion to prevent positions outside of the window
    float x = (std::max)((std::min)((float)result.x, 1920.0f), (float)0);
    float y = (std::max)((std::min)((float)result.y, 1080.0f), (float)0);

    return float2(x, y);
}

LRESULT CALLBACK Input::WindowProc(HWND givenHandle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;

    // TODO: make hasFocus more efficient

    // key pressed
    if (msg == WM_KEYDOWN)
    {
        if(Window::hasFocus)
        {
            bool key_is_down;
            bool key_was_down;
            key_is_down  = ((lparam & (1 << 31)) == 0);
            key_was_down = ((lparam & (1 << 30)) != 0);
            
            keyboard[(uint8_t)wparam].down = key_is_down;
            keyboard[(uint8_t)wparam].downNow = key_is_down && !key_was_down;
            keyboard[(uint8_t)wparam].up = !key_is_down;
        }
    }
    
    // key released
    else if (msg == WM_KEYUP)
    {
        if(Window::hasFocus)
        {
            keyboard[(uint8_t)wparam].up = true;
            keyboard[(uint8_t)wparam].down = false;
        }
    }

    // if mouse wheel scrolled
    else if (msg == WM_MOUSEWHEEL)
    {
        if(Window::hasFocus)
        {
            mouseWheel = wparam & 0b10000000000000000000000000000000 ? -1 : 1;
        }
    }

    // if mouse pressed
    else if (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN)
    {
        if(Window::hasFocus)
        {
            if (msg == WM_LBUTTONDOWN)
            {
                mouse[0].down = true;
                mouse[0].downNow = true;
            }

            else if (msg == WM_MBUTTONDOWN)
            {
                mouse[1].down = true;
                mouse[1].downNow = true;
            }

            else if (msg == WM_RBUTTONDOWN)
            {
                mouse[2].down = true;
                mouse[2].downNow = true;
            }
        }
    }

    // if mouse released
    else if (msg == WM_LBUTTONUP || msg == WM_MBUTTONUP || msg == WM_RBUTTONUP)
    {
        if(Window::hasFocus)
        {
            if (msg == WM_LBUTTONUP)
            {
                mouse[0].down = false;
                mouse[0].up = true;
            }

            else if (msg == WM_MBUTTONUP)
            {
                mouse[1].down = false;
                mouse[1].up = true;
            }

            else if (msg == WM_RBUTTONUP)
            {
                mouse[2].down = false;
                mouse[2].up = true;
            }
        }
    }

    // if tabbing out, clear keyboard to prevent erroneous presses
    else if (msg == WM_KILLFOCUS)
    {
        Window::hasFocus = false;
        memset(keyboard, 0, 256 * sizeof(keyboard[0]));
    }

    // if tabbing back in, enable presses again
    else if (msg == WM_SETFOCUS)
    {
        Window::hasFocus = true;
    }

    // if window closed
    else if (msg == WM_CLOSE)
    {
        DestroyWindow(givenHandle);
        PostQuitMessage(0);
        Application::Kill();
    }

    else
    {
        result = DefWindowProcW(givenHandle, msg, wparam, lparam);
    }
    
    return result;
}