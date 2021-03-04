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
PID_gains pnt_turn_gains {10.0, 0.0, 150.0, 10, 100};
PID_gains crv_turn_gains {10.0, 0.0, 150.0, 10, 100};

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

    old_vol_left = 0;
    old_vol_right = 0;
    chassis_obj.drive_vol(0, 0);
    chassis_obj.reset_trk();
}

void pnt_turn_func(int target)
{
    int start_time {static_cast<int>(pros::millis())};
    left_side.reset().set_gains(pnt_turn_gains);
    chassis_obj.reset_trk();

    while (1)
    {
        int travel = chassis_obj.get_trk('l');
        int vol = std::clamp(left_side.calculate(target, travel), old_vol_left - 450, old_vol_left + 450);

        chassis_obj.drive_vol(vol, -vol);
        pros::lcd::print(0, "LT: %i\t LV: %i", chassis_obj.get_trk('l'), vol);
        pros::lcd::print(1, "RT: %i\t RV: %i", chassis_obj.get_trk('r'), vol);

        old_vol_left = vol;
        old_vol_right = -vol;

        if (within_range(target, travel, 5))
            break;
        else if (pros::millis() - start_time > max_time)
            ;

        pros::delay(10);
    }

    old_vol_left = 0;
    old_vol_right = 0;
    chassis_obj.drive_vol(0, 0);
    chassis_obj.reset_trk();
}

void crv_turn_func(double theta, int dist)
{
    double  radius = (double)dist / theta;
    double  radius_l = radius + 5.75,
            radius_r = radius - 5.75;
    double  arc_len_l = radius_l * theta,
            arc_len_r = radius_r * theta;
    
    int len_l_tick = std::round((arc_len_l / 3.25) * (360.0 / 3.25));
    int len_r_tick = std::round((arc_len_r / 3.25) * (360.0 / 3.25));
    
    int start_time {static_cast<int>(pros::millis())};
    left_side.reset().set_gains(crv_turn_gains);
    right_side.reset().set_gains(crv_turn_gains);
    chassis_obj.reset_trk();

    while (1)
    {
        int travel_l = chassis_obj.get_trk('l'),
            travel_r = chassis_obj.get_trk('r');
        
        int vol_l = std::clamp(left_side.calculate(len_l_tick, travel_l), old_vol_left - 450, old_vol_left + 450);
        int vol_r = std::clamp(right_side.calculate(len_r_tick, travel_r), old_vol_right - 450, old_vol_right + 450);

        chassis_obj.drive_vol(vol_l, vol_r);
        pros::lcd::print(0, "LT: %i\t LV: %i", chassis_obj.get_trk('l'), vol_l);
        pros::lcd::print(1, "RT: %i\t RV: %i", chassis_obj.get_trk('r'), vol_r);

        old_vol_left = vol_l;
        old_vol_right = vol_r;

        if (within_range(len_l_tick, travel_l, 5) && within_range(len_r_tick, travel_r, 5))
            break;
        else if (pros::millis() - start_time > max_time)
            ;

        pros::delay(10);
    }

    old_vol_left = 0;
    old_vol_right = 0;
    chassis_obj.drive_vol(0, 0);
    chassis_obj.reset_trk();
}