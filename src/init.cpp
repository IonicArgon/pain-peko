//* Initialization code
//* Created: 1/27/2021
//* Last modified: 1/27/2021

//* Main header
#include "main.h"

//* Subsystem headers
#include "lib/hardware/globals.hpp"


//* Initialization callback
void initialize(void)
{
    chassis_obj = new Chassis{19, 20, -9, -10, 5, 1};
    shooter_obj = new Shooter{-15, -8, 17, -7};
}

//* Competition Initialize callback
void competition_initialize(void)
{

}

//* Disbled callback
void disabled(void)
{

}