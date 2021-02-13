//* Autonomous control code
//* Created: 1/27/2021
//* Last modified: 2/9/2021

//* Main header
#include "main.h"

//* Subsystem headers
#include "lib/control/pid.hpp"
#include "lib/hardware/globals.hpp"
#include "lib/utility/misc.hpp"

//* gains
PID_gains straight_gains {5, 0, 100, 10, 100};

//* globals
int old_vol_left {0}, old_vol_right{0};
int max_time{99999};
PID left_side{{}};
PID right_side{{}};
void straight_func(int target);

//* skills auto
void skills(void)
{

}

//* live auto
void live(void)
{
    straight_func(24.0_in_to_tick);
    clear_screen();
}

//* Autonomous callback
void autonomous(void)
{
    switch (path_selection)
    {
        case auto_select::LIVE:
            pros::lcd::print(0, "live selected");
            live();
            break;
        case auto_select::SKILLS:
            skills();
            break;
    }
}

//* turning funcs and stuff

bool within_range(int target)
{
    int avg {(chassis_obj.get_trk('l') + chassis_obj.get_trk('r')) / 2};
    return ((avg - (target + 5)) * (avg - (target - 5)) <= 0);
}

void straight_func(int target)
{
    int start_time {pros::millis()};
    left_side.reset().set_gains(straight_gains);
    right_side.reset().set_gains(straight_gains);
    chassis_obj.reset_trk();

    while (1)
    {
        int left_vol = std::clamp(left_side.calculate(target, ((chassis_obj.get_trk('l') + chassis_obj.get_trk('r')) / 2)), 
        old_vol_left - 500, old_vol_left + 500);
        int right_vol = std::clamp(right_side.calculate(target, ((chassis_obj.get_trk('l') + chassis_obj.get_trk('r')) / 2)), 
        old_vol_right - 500, old_vol_right + 500);

        chassis_obj.drive_vol(left_vol, right_vol);
        pros::lcd::print(0, "LT: %i\t LV: %i", chassis_obj.get_trk('l'), left_vol);
        pros::lcd::print(1, "RT: %i\t RV: %i", chassis_obj.get_trk('r'), right_vol);

        old_vol_left = left_vol;
        old_vol_right = right_vol;

        if (within_range(target))
            break;
        else if ((left_side.get_derv() < 0.001 && right_side.get_derv() < 0.001) && (pros::millis() - start_time > 1000))
            break;
        else if (pros::millis() - start_time > max_time)
            break;

        pros::delay(10);
    }

    chassis_obj.drive_vol(0, 0);
    chassis_obj.reset_trk();
}