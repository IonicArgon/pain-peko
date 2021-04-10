//* Timer source

#include "main.h"

#include "lib/utility/timer.hpp"

Timer::Timer() {
    start_time = pros::millis();
    std::cout << "[UTIL] Timer created at: " << start_time << " ms\n";
}

Timer::~Timer() {
    std::cout << "[UTIL] Timer destroyed at: " << pros::millis() << " ms\n";
}

uint32_t Timer::get() {
    return pros::millis() - start_time;
}