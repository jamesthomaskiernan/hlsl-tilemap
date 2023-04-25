#include "application.h"

int main()
{
    Application::Start();
    
    while (Application::IsRunning())
    {
        Application::Update();
    }

    return 0;
}