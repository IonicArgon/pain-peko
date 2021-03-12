//* Async header file
//* Created: 3/12/2021
//* Last modified: 3/12/2021

#ifndef H_ASYNC_HPP
#define H_ASYNC_HPP

#include "api.h"
#include <queue>
#include <functional>

extern void do_async(std::function<void()> func);

#endif