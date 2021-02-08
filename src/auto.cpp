//* Autonomous control code
//* Created: 1/27/2021
//* Last modified: 1/27/2021

//* Main header
#include "main.h"

//* Subsystem headers
#include "lib/control/pid.hpp"
#include "lib/hardware/globals.hpp"


//* skills auto
void skills(void)
{

}

//* live auto
void live(void)
{

}

//* Autonomous callback
void autonmous(void)
{
    switch (path_selection)
    {
    case auto_select::LIVE:
        live();
        break;
    case auto_select::SKILLS:
        skills();
        break;
    }
}