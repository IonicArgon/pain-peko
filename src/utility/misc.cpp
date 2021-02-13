//* Misc source file
//* Created: 1/31/2021
//* Last modified: 1/31/2021

//* Main header
#include "main.h"

//* misc header
#include "lib/utility/misc.hpp"

//* local globals for the funnies
double wheelbase_length {11.5};
double wheel_diam {3.25};
double tick_ratio {360.0};

double deg_to_rad(long double deg)
{
    return deg * (180 / M_PI);
}

int operator"" _deg_to_tick (long double deg)
{
    double arclen {(wheelbase_length / 2) * deg_to_rad(deg)};
    return std::round((arclen / wheel_diam) * (tick_ratio / wheel_diam));
}

int operator"" _in_to_tick (long double in)
{
    return std::round((in / wheel_diam) * (tick_ratio / wheel_diam));
}

//* thank the lord Donald Knuth
bool approx_float_eq(double a, double b, double abs_eps, double rel_eps)
{
    double diff{std::abs(a - b)};
    if (diff <= abs_eps)
        return true;
    
    return (diff <= (std::max(std::abs(a), std::abs(b)) * rel_eps));
}

//* clear screen
void clear_screen(void)
{
    for (int i{0}; i < 7; ++i)
        pros::lcd::clear_line(i);
}