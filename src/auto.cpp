//* Autonomous control code
//* Created: 1/27/2021
//* Last modified: 2/9/2021

#define __P 1;

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

//* globals
int old_vol_left {0}, old_vol_right{0};
int max_time{5000};
PID main_pid{{}};
void move_func(int target, bool mode, uint32_t delay = 1);

//* some macros
void the_succ(void);

//* skills auto
void skills(void) {
//* part 1
#if __P == 1 || __P == 2 || __P == 3 || __P == 4
    clear_screen();
    //! deploy the hood also scores goal D
    shooter_obj.shooter_set(-600, 0);
    pros::delay(500);
    shooter_obj.shooter_set(0, 0);
    //! suck in the first ball and move to goal A and shoot
    do_async(the_succ);     //? asynchronous intaking
    move_func(24.0_in_to_tick, false, 100);
    move_func(-70.0_deg_to_tick, true, 100);
    move_func(12.0_in_to_tick, false, 100);
    shooter_obj.shooter_set(600, 600);  //? does the shooting
    pros::delay(750);
    shooter_obj.shooter_set(0, 0);
    //! back out drive and suck ball and move to goal B and shoot
    move_func(10.0_in_to_tick, false, 100);
    move_func(140.0_deg_to_tick, true, 100);
    do_async(the_succ);
    move_func(48.0_in_to_tick, false, 100);
    move_func(-90.0_deg_to_tick, true, 100);
    move_func(2.0_in_to_tick, false, 100);
    shooter_obj.shooter_set(600, 600);
    pros::delay(750);
    shooter_obj.shooter_set(0, 0);
    //! back out and turn
    move_func(-12.0_in_to_tick, false, 100);
    move_func(90.0_deg_to_tick, true, 100);
#endif
//* part 2
#if __P == 2 || __P == 3 || __P == 4
    //! drive forward to suck ball and move to goal C and shoot
    do_async(the_succ);
    move_func(48.0_in_to_tick, false, 100);
    move_func(-70.0_deg_to_tick, true, 100);
    move_func(14.0_in_to_tick, false, 100);
    shooter_obj.shooter_set(600, 600);
    pros::delay(750);
    shooter_obj.shooter_set(0, 0);
    //! turn around drive forward suck ball drive to goal F and shoot ball
    move_func(14.0_in_to_tick, false, 100);
    move_func(180.0_deg_to_tick, true, 100);
    do_async(the_succ);
    move_func(36.0_in_to_tick, false, 100);
    move_func(-130.0_deg_to_tick, true, 100);
    move_func(24.0_in_to_tick, false, 100);
    shooter_obj.shooter_set(600, 600);
    pros::delay(750);
    shooter_obj.shooter_set(0, 0);
    //! back out and turn
    move_func(-2.0_in_to_tick, false, 100);
    move_func(90.0_deg_to_tick, true, 100);
#endif
//* part 3
#if __P == 3 || __P == 4
    //! drive forward suck the ball in and go to goal I and shoot
    do_async(the_succ);
    move_func(48.0_in_to_tick, false, 100);
    move_func(-45.0_deg_to_tick, true, 100);
    move_func(6.0_in_to_tick, false, 100);
    shooter_obj.shooter_set(600, 600);
    pros::delay(750);
    shooter_obj.shooter_set(0, 0);
    //! back out turn around and drive and suck ball and go to goal H and shoot
    move_func(-6.0_in_to_tick, false, 100);
    move_func(135.0_deg_to_tick, true, 100);
    do_async(the_succ);
    move_func(48.0_in_to_tick, false, 100);
    move_func(-90.0_deg_to_tick, true, 100);
    move_func(4.0_in_to_tick, false, 100);
    shooter_obj.shooter_set(600, 600);
    pros::delay(750);
    shooter_obj.shooter_set(0, 0);
    //! back out and turn
    move_func(-16.0_in_to_tick, false, 100);
    move_func(90.0_deg_to_tick, true, 100);
#endif
//* part 4
#if __P == 4
    //! drive forward and suck in ball and drive to goal G to shoot
    do_async(the_succ);
    move_func(48.0_in_to_tick, false, 100);
    move_func(-70.0_deg_to_tick, true, 100);
    move_func(12.0_in_to_tick, false, 100);
    shooter_obj.shooter_set(600, 600);
    pros::delay(750);
    shooter_obj.shooter_set(0, 0);
    //! turn around and suck in the ball
    move_func(10.0_in_to_tick, false, 100);
    move_func(180.0_deg_to_tick, true, 100);
    do_async(the_succ);
    move_func(48.0_in_to_tick, false, 100);
    //! reorient robot to be at angle with goal E and try poking out all the balls in the goal
    move_func(-12.0_in_to_tick, false, 100);    //? back out
    move_func(50.0_deg_to_tick, true, 100);     //? turn
    move_func(30.0_in_to_tick, false, 100);     //? drive into the goal poke out first ball
    move_func(-10.0_in_to_tick, false, 100);
    move_func(10.0_in_to_tick, false, 100);     //? poke second
    move_func(-10.0_in_to_tick, false, 100);
    move_func(10.0_in_to_tick, false, 100);     //? poke third
    shooter_obj.shooter_set(600, 600);          //? shoot the ball
    pros::delay(750);
    shooter_obj.shooter_set(0, 0);
    move_func(-24.0_in_to_tick, false, 100);
#endif
}

//* live auto
void live(void) {
    
}

//* Autonomous callback
void autonomous(void) {
    chassis_obj.set_brake(pros::E_MOTOR_BRAKE_COAST);
    switch (path_selection) {
        case auto_select::LIVE:
            pros::lcd::print(0, "live selected");
            live();
            break;
        case auto_select::SKILLS:
            pros::lcd::print(0, "skills selected");
            skills();
            break;
    }
}

//* turning funcs and stuff

bool within_range(double target, double val, double range) {
    return (val > target - range) && (val < target + range);
}

void move_func(int target, bool mode, uint32_t delay) {
    int start_time {static_cast<int>(pros::millis())};
    main_pid.reset().set_gains((mode) ? pnt_turn_gains : straight_gains);
    chassis_obj.reset_trk();

    while (1) {
        int travel = (mode) ? 
            (chassis_obj.get_trk('l') - chassis_obj.get_trk('r')) / 2 :
            (chassis_obj.get_trk('l') + chassis_obj.get_trk('r')) / 2;
        int vol = std::clamp(main_pid.calculate(target, travel), old_vol_left - 450, old_vol_left + 450);

        chassis_obj.drive_vol(vol, (mode) ? -vol : vol);
        pros::lcd::print(0, "LT: %i\t LV: %i", chassis_obj.get_trk('l'), vol);
        pros::lcd::print(1, "RT: %i\t RV: %i", chassis_obj.get_trk('r'), vol);

        old_vol_left = vol;
        old_vol_right = (mode) ? -vol : vol;

        
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
    pros::delay(delay);
}

//* macro defs
void the_succ(void) {
    shooter_obj.shooter_set(0, 600, 600);
    while (shooter_obj.get_torq('b') < 0.2) {
        pros::delay(20);
    }
    pros::delay(200);
    shooter_obj.shooter_set(0, 0, 0);
}