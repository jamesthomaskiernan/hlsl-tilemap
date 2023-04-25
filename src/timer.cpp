#include "timer.h"

Timer::Timer()
{
    Start();
};

double Timer::GetMilisecondsElapsed()
{
    std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
    return elapsed.count();
}

void Timer::Restart()
{
	start = std::chrono::high_resolution_clock::now();
}

void Timer::Stop()
{
    stop = std::chrono::high_resolution_clock::now();
}

void Timer::Start()
{
    start = std::chrono::high_resolution_clock::now();
    stop = std::chrono::high_resolution_clock::now();
}