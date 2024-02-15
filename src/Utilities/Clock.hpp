#pragma once

#include <chrono>

class Clock {
public:
    void restart();
    float getElapsedTime();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};