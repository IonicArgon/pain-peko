//* Timer header file

#ifndef H_TIMER_HPP
#define H_TIMER_HPP

#include "api.h"

class Timer
{
public:
    Timer(void);
    ~Timer();
    uint32_t get(void);
private:
    uint32_t start_time;
};

#endif