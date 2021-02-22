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
PID_gains straight_gains {18.0, 0.0, 150.0, 10, 100};

//* globals
int old_vol_left {0}, old_vol_right{0};
int max_time{5000};
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
    straight_func(37.0_in_to_tick);
    clear_screen();
    pros::delay(1000);
    straight_func(-37.0_in_to_tick);
    clear_screen();
}

//* Autonomous callback
void autonomous(void)
{
    chassis_obj.set_brake(pros::E_MOTOR_BRAKE_COAST);
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

bool within_range(double target, double val, double range)
{
    return (val > target - range) && (val < target + range);
}

void straight_func(int target)
{
    int start_time {static_cast<int>(pros::millis())};
    left_side.reset().set_gains(straight_gains);
    chassis_obj.reset_trk();

    while (1)
    {
        int avg_sides = (chassis_obj.get_trk('l') + chassis_obj.get_trk('r')) / 2;
        int vol = std::clamp(left_side.calculate(target, avg_sides), old_vol_left - 450, old_vol_left + 450);

        chassis_obj.drive_vol(vol, vol);
        pros::lcd::print(0, "LT: %i\t LV: %i", chassis_obj.get_trk('l'), vol);
        pros::lcd::print(1, "RT: %i\t RV: %i", chassis_obj.get_trk('r'), vol);

        old_vol_left = vol;
        old_vol_right = vol;

        
        if (within_range(target, avg_sides, 5))
            break;
        else if (pros::millis() - start_time > max_time)
            ;

        pros::delay(10);
    }

    chassis_obj.drive_vol(0, 0);
    chassis_obj.reset_trk();
}