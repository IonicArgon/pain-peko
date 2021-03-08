//* Misc header file
//* Created: 1/31/2021
//* Last modified: 1/31/2021

#ifndef H_MISC_HPP
#define H_MISC_HPP

#include "api.h"

extern double operator"" _d_to_r (long double deg);
extern int operator"" _deg_to_tick (long double deg);
extern int operator"" _in_to_tick (long double in);
bool approx_float_eq(double a, double b, double abs_eps, double rel_eps);
void clear_screen(void);

#endif // H_MISC_HPP