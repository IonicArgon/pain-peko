//* Async header source

#include "main.h"

#include "lib\utility\async.hpp"

// thanks manas for the idea

static std::queue<std::pair<std::function<void()>, uint32_t>> async_queue;

static pros::Task async_task {[]() {
        while (!pros::competition::is_disabled() && async_task.notify_take(0, TIMEOUT_MAX)) {
            auto pair = async_queue.front();
            async_queue.pop();
            pair.first();
            pros::delay(pair.second);
        }
    }
};

void do_async(std::function<void()> func, uint32_t delay) {
    async_queue.push(std::make_pair(func, delay));
    async_task.notify();
}