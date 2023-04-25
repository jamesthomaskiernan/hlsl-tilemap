#pragma once

#include <chrono>

class Timer
{
    public:

    Timer();

    void Start();

    void Stop();

    void Restart();

    double GetMilisecondsElapsed();

    private:

    std::chrono::high_resolution_clock::time_point start;
    
    std::chrono::high_resolution_clock::time_point stop;
};