//* Async header source
//* Created: 3/12/2021
//* Last modified: 3/12/2021

#include "main.h"

#include "lib\utility\async.hpp"

static std::queue<std::function<void()>> async_queue;

static pros::Task async_task {[]() {
        while (!pros::competition::is_disabled() && async_task.notify_take(0, TIMEOUT_MAX)) {
            auto run = async_queue.front();
            async_queue.pop();
            run();
            pros::delay(1);
        }
    }
};

void do_async(std::function<void()> func) {
    async_queue.push(func);
    async_task.notify();
}