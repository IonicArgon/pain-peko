//* Autonomous control code
//* Created: 1/27/2021
//* Last modified: 2/9/2021

//* Main header
#include "main.h"

//* Subsystem headers
#include "lib/control/pid.hpp"
#include "lib/hardware/globals.hpp"
#include "lib/utility/misc.hpp"
#include "lib/utility/async.hpp"

//* gains
PID_gains straight_gains {17.75, 0.0, 150.0, 10, 100};
PID_gains pnt_turn_gains {30.0, 0.0, 200.0, 10, 100};
//PID_gains crv_turn_gains {5.0, 0.0, 150.0, 10, 100};

//* globals
int old_vol_left {0}, old_vol_right{0};
int max_time{5000};
PID left_side{{}};
PID right_side{{}};
void straight_func(int target);
void pnt_turn_func(int target);
//void crv_turn_func(double theta, int dist);

//* skills auto
void skills(void)
{

}

//* live auto
void live(void)
{
    do_async(std::bind(straight_func, 60.0_in_to_tick), 1000);
    do_async(std::bind(pnt_turn_func, 90.0_deg_to_tick), 2000);
    do_async(std::bind(pnt_turn_func, 90.0_deg_to_tick), 2000);
    do_async(std::bind(straight_func, 60.0_in_to_tick));
    shooter_obj.shooter_set(600, 600, 600);
    pros::delay(2000);
    shooter_obj.shooter_set(0, 0, 0);
    pros::delay(1000);
    shooter_obj.shooter_set(-600, -600, -600);
    pros::delay(2000);
    shooter_obj.shooter_set(0, 0, 0);
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
        int travel = (chassis_obj.get_trk('l') - chassis_obj.get_trk('r')) / 2;
        int vol = std::clamp(left_side.calculate(target, travel), old_vol_left - 450, old_vol_left + 450);

        chassis_obj.drive_vol(vol, -vol);
        pros::lcd::print(0, "LT:%i\tLV:%i\tTar:%i", chassis_obj.get_trk('l'), vol, target);
        pros::lcd::print(1, "RT:%i\tRV:%i", chassis_obj.get_trk('r'), vol);

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

/*
void crv_turn_func(double theta, int dist)
{
    double  radius = (double)dist / theta;
    double  radius_l = radius + 5.75,
            radius_r = radius - 5.75;
    int arc_len_l = std::round(radius_l * theta),
        arc_len_r = std::round(radius_r * theta);
    
    int start_time {static_cast<int>(pros::millis())};
    left_side.reset().set_gains(crv_turn_gains);
    right_side.reset().set_gains(crv_turn_gains);
    chassis_obj.reset_trk();

    while (1)
    {
        int travel_l = chassis_obj.get_trk('l'),
            travel_r = chassis_obj.get_trk('r');
        
        int vol_l = std::clamp(left_side.calculate(arc_len_l, travel_l), old_vol_left - 450, old_vol_left + 450);
        int vol_r = std::clamp(right_side.calculate(arc_len_r, travel_r), old_vol_right - 450, old_vol_right + 450);

        chassis_obj.drive_vol(vol_l, vol_r);
        pros::lcd::print(0, "LT: %i\t LV: %i", chassis_obj.get_trk('l'), vol_l);
        pros::lcd::print(1, "RT: %i\t RV: %i", chassis_obj.get_trk('r'), vol_r);

        old_vol_left = vol_l;
        old_vol_right = vol_r;

        if (within_range(arc_len_l, travel_l, 5) && within_range(arc_len_r, travel_r, 5))
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
*/