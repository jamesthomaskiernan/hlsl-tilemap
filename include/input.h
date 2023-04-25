#pragma once
#include <windows.h>
#include <SimpleMath.h>
#include <stdint.h>
#include "window.h"

enum KeyCode
{
    Backspace =	0x08,
    Tab =	0x09,
    Clear = 0x0C,
    Enter =	0x0D,
    Shift = 0x10,
    Control = 0x11,
    Alt = 0x12,
    Pause = 0x13,
    CapsLock = 0x14,
    Esc = 0x1B,
    Space = 0x20,
    PageUp = 0x21,
    PageDown = 0x22,
    End = 0x23,
    Home = 0x24,
    Left = 0x25,
    Up = 0x26,
    Right = 0x27,
    Down = 0x28,
    Keypad0 = 0x60,
    Keypad1 = 0x61,	
    Keypad2 = 0x62,	
    Keypad3 = 0x63,	
    Keypad4 = 0x64,	
    Keypad5 = 0x65,	
    Keypad6 = 0x66,
    Keypad7 = 0x67,	
    Keypad8 = 0x68,	
    Keypad9 = 0x69,	
};

struct Button
{
    bool down;
    bool downNow;
    bool up;
};

class Input
{
    friend class Window;

    public:

    // update all inputs for the current frame
    static void Update();

    // returns true while the user holds down the key
    static bool GetKey(char key);

    // returns true during the frame the user starts pressing down the key
    static bool GetKeyDown(char key);

    // returns true during the frame the user releases the key
    static bool GetKeyUp(char key);

    // returns whether the given mouse button is held down
    static bool GetMouseButton(int button);

    // returns true during the frame the user pressed the given mouse button
    static bool GetMouseButtonDown(int button);

    // returns true during the frame the user releases the given mouse button
    static bool GetMouseButtonUp(int button);

    // returns the direction of the mouse's scroll wheel (1 is up, -1 is down, 0 is stationary)
    static int GetMouseScroll();

    // returns position of mouse on window
    static float2 GetMousePosition();

    private:

    static LRESULT CALLBACK WindowProc(HWND givenHandle, UINT msg, WPARAM wparam, LPARAM lparam);

    static inline Button mouse[3] = {};

    static inline Button keyboard[256] = {};

    static inline int mouseWheel;
};