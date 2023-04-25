#include "application.h"
#include "window.h"
#include "input.h"
#include "graphics.h"
#include "pixel_shader.h"
#include "vertex_shader.h"
#include <vector>
#include "struct_buffer.h"
#include "vertex_buffer.h"
#include "camera_controller.h"
#include "texture.h"
#include "compute_shader.h"
#include "stdint.h"
#include "tilemap.h"

bool Application::IsRunning()
{
    return running;
}

void Application::Kill()
{
    running = false;
}

void Application::Start()
{
    Application::running = true;
    Window::Init();
    Graphics::Init();
    CameraController::Init();
    Tilemap::Init();
}

void Application::Update()
{
    RefreshDeltaTime();
    PrintFPS();
    Input::Update();
    CameraController::Update(deltaTime);
    Tilemap::Update();
    Graphics::RenderFrame();
}

void Application::RefreshDeltaTime()
{
    deltaTime = gameClock.GetMilisecondsElapsed();
    gameClock.Restart();
}

float Application::GetDeltaTime()
{
    return deltaTime;
}

void Application::PrintFPS()
{
    if (fpsClock.GetMilisecondsElapsed() > 1)
    {
        fpsClock.Stop();
        std::cout << "FPS: " << fpsCount << std::endl;
        fpsCount = 0;
        fpsClock.Restart();
    }

    ++fpsCount;
}