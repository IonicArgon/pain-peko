//* Autonomous control code
//* Created: 1/27/2021
//* Last modified: 1/27/2021

//* Main header
#include "main.h"

//* Subsystem headers
#include "lib/control/pid.hpp"
#include "lib/hardware/globals.hpp"

//* gains
pid_gains dist_gains {2.0, 15.0, 10, 1, 250};
pid_gains turn_gains {4.0, 30.0, 10, 1, 250};
pid_gains crve_gains {3.0, 22.5, 10, 1, 250};

//* skills auto
void skills(void)
{
    pid_control pid(dist_gains);
}

//* live auto
void live(void)
{
    pid_control pid(dist_gains);
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