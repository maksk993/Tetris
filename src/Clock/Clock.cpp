#include "Clock.hpp"

void Clock::restart() {
    start_time = std::chrono::high_resolution_clock::now();
}

float Clock::getElapsedTime() {
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = current_time - start_time;
    return elapsed.count();
}