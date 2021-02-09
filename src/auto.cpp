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
PID_gains straight_gains {50, 100, 100, 10, 100};
PID_gains p_turn_gains {50, 100, 100, 10, 100};

//* globals
int straight_target {0}, turn_target {0};
int old_vol_left {0}, old_vol_right{0};
int max_time{0};
PID left_side{{}};
PID right_side{{}};
void straight_func(void);
void p_turn_func(void);
void reset_all(void);
void wait_until_settled(void);

pros::Task task_straight{straight_func};
pros::Task task_p_turn{p_turn_func};


//* skills auto
void skills(void)
{

}

//* live auto
void live(void)
{
    reset_all();
    task_straight.resume();
    straight_target = 36.0_in_to_tick;
    task_straight.notify();
    wait_until_settled();
    reset_all();
}

//* Autonomous callback
void autonmous(void)
{
    task_straight.suspend();
    task_p_turn.suspend();

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

//* turning funcs and stuff

void reset_all(void)
{
    straight_target = 0;
    turn_target = 0;

    left_side.reset();
    right_side.reset();
}

void wait_until_settled(void)
{
    while (task_straight.get_state() == pros::E_TASK_STATE_RUNNING || task_p_turn.get_state() == pros::E_TASK_STATE_RUNNING)
    {pros::delay(5);}
}

void straight_func(void)
{
    while (task_straight.notify_take(true, TIMEOUT_MAX))
    {
        int start_time {pros::millis()};
        left_side.reset().set_gains(straight_gains);
        right_side.reset().set_gains(straight_gains);

        while (
            std::abs(straight_target - 
            ((chassis_obj->get_trk('l') + chassis_obj->get_trk('r')) / 2)) > 5 
            || (left_side.get_derv() > 10 && right_side.get_derv() > 10)
            || pros::millis() - start_time < max_time)
        {
            int left_vol = std::clamp(left_side.calculate(straight_target, ((chassis_obj->get_trk('l') + chassis_obj->get_trk('r')) / 2)), 
            old_vol_left - 250, old_vol_left + 250);
            int right_vol = std::clamp(right_side.calculate(straight_target, ((chassis_obj->get_trk('l') + chassis_obj->get_trk('r')) / 2)), 
            old_vol_right - 250, old_vol_right + 250);

            chassis_obj->drive_vol(left_vol, right_vol);

            old_vol_left = left_vol;
            old_vol_right = right_vol;
        }

        chassis_obj->reset_trk();
    }
}

void p_turn_func(void)
{
    while (task_p_turn.notify_take(true, TIMEOUT_MAX))
    {
        
    }
}