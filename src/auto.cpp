//* Autonomous control code

//! these turn on/off sections of code
#define __P1 ;
#define __P2 ;
#define __P3 ;
#define __P4 ;

//* Main header
#include "main.h"

//* Subsystem headers
#include "lib/control/pid.hpp"
#include "lib/hardware/globals.hpp"
#include "lib/utility/misc.hpp"
#include "lib/utility/async.hpp"
#include "lib/utility/timer.hpp"

//* gains
PID_gains straight_gains {18.0, 0.0, 175.0, 10, 100};
PID_gains pnt_turn_gains {30.0, 0.0, 200.0, 10, 100};

//* globals
int old_vol_left {0}, old_vol_right{0};
int max_time{2500};                 //? current max time set to 5s
int light_sensor_threshold {-10};   //? the succ, towards 0 = more sensitive, must be negative always
int the_succ_delay {45};            //? the succ, changes sample rate in ms
PID main_pid{{}};
void move_func(int target, bool mode, uint32_t delay = 1);

//* some macros
void the_succ(int range);
void the_succ_02(int delay);        //? time based, supply delay in ms

//* skills auto
void skills(void) {

//VEBRATO TOMATO, PART 1 INIT IS GOOD TO GO WITH 100% ACC
//* part 1
#if defined(__P1)
    clear_screen();
    //! deploy the hood also scores goal D
    shooter_obj.shooter_set(600, 0);
    pros::delay(300);
    shooter_obj.shooter_set(0, 0);
    //! suck in the first ball and move to goal A and shoot
    do_async(std::bind(the_succ, light_sensor_threshold));  //? asynchronous intaking
    // do_async(std::bind(the_succ_02, 5000));              //? example change
    move_func(37.0_in_to_tick, false, 40);
    move_func(-74.1_deg_to_tick, true, 10);
    move_func(17.5_in_to_tick, false, 50);
    shooter_obj.shooter_set(600, 600);  //? does the shooting
    pros::delay(600);                   //?
    shooter_obj.shooter_set(0, 0);      //?
    //! back out drive and suck ball and move to goal B and shoot
    move_func(-12.7_in_to_tick, false, 50); //current change
    move_func(134.9_deg_to_tick, true, 10);
    do_async(std::bind(the_succ, light_sensor_threshold)); 
    move_func(48.9_in_to_tick, false, 50);
    move_func(-95.6_deg_to_tick, true, 10);
    move_func(6.4_in_to_tick, false, 50); //offset change
    shooter_obj.shooter_set(600, 600);
    pros::delay(600);
    shooter_obj.shooter_set(0, 0);
    //! back out and turn
    move_func(-16.5_in_to_tick, false, 50);
    move_func(96.7_deg_to_tick, true, 10);
#endif




//* part 2
#if defined(__P1) && defined(__P2)
    //! drive forward to suck ball and move to goal C and shoot
    do_async(std::bind(the_succ, light_sensor_threshold)); //threshold acting kinda gae though
    move_func(45.1_in_to_tick, false, 50);
    move_func(-64.2_deg_to_tick, true, 10);
    move_func(23.9_in_to_tick, false, 50);
    shooter_obj.shooter_set(600, 600);
    pros::delay(600);
    shooter_obj.shooter_set(0, 0);
    //! turn around drive forward suck ball drive to goal F and shoot ball
    move_func(-14.5_in_to_tick, false, 50);
    move_func(176.5_deg_to_tick, true, 10);
    do_async(std::bind(the_succ, light_sensor_threshold));
    move_func(50.1_in_to_tick, false, 50); 
    move_func(-125.5_deg_to_tick, true, 10);
    move_func(31.9_in_to_tick, false, 50);
    shooter_obj.shooter_set(600, 600);
    pros::delay(600);
    shooter_obj.shooter_set(0, 0);
    //! back out and turn
    move_func(-6.7_in_to_tick, false, 50);
    move_func(92.4_deg_to_tick, true, 10);
#endif

//* part 3
#if defined(__P1) && defined(__P2) && defined(__P3)
    //! drive forward suck the ball in and go to goal I and shoot
    do_async(std::bind(the_succ, light_sensor_threshold));
    move_func(50.0_in_to_tick, false, 80);
    move_func(-57.4_deg_to_tick, true, 10);
    move_func(12.6_in_to_tick, false, 80);
    shooter_obj.shooter_set(600, 600);
    pros::delay(600);
    shooter_obj.shooter_set(0, 0);
    //! back out turn around and drive and suck ball and go to goal H and shoot
    move_func(-15.0_in_to_tick, false, 80);
    move_func(138.7_deg_to_tick, true, 10);
    do_async(std::bind(the_succ, light_sensor_threshold));
    move_func(46.3_in_to_tick, false, 80);
    move_func(-93.4_deg_to_tick, true, 10);
    move_func(7.8_in_to_tick, false, 80);
    shooter_obj.shooter_set(600, 600);
    pros::delay(600);
    shooter_obj.shooter_set(0, 0);
    //! back out and turn
    move_func(-17.1_in_to_tick, false, 80);
    move_func(95.4_deg_to_tick, true, 10);
#endif

//* part 4
#if defined(__P1) && defined(__P2) && defined(__P3) && defined(__P4)
    //! drive forward and suck in ball and drive to goal G to shoot
    do_async(std::bind(the_succ, light_sensor_threshold));
    move_func(45.1_in_to_tick, false, 80);
    move_func(-70.5_deg_to_tick, true, 10);
    move_func(22.6_in_to_tick, false, 80);
    shooter_obj.shooter_set(600, 600);
    pros::delay(600);
    shooter_obj.shooter_set(0, 0);
    //! turn around and suck in the ball
    move_func(-10.1_in_to_tick, false, 80);
    move_func(177.7_deg_to_tick, true, 10);
    do_async(std::bind(the_succ, light_sensor_threshold));
    move_func(68.0_in_to_tick, false, 80);
    //! reorient robot to be at angle with goal E and try poking out all the balls in the goal
    move_func(91.5_deg_to_tick, true, 50);     //? turn
    shooter_obj.shooter_set(0, 0, -600);
    move_func(16.0_in_to_tick, false, 80);     //? drive into the goal poke out first ball
    shooter_obj.shooter_set(0, 0, 0);
    shooter_obj.shooter_set(600, 600);          //? shoot the ball
    pros::delay(600);
    shooter_obj.shooter_set(0, 0);
    move_func(-15.0_in_to_tick, false, 100);
#endif
}

//* live auto
void live(void) {
    do_async(std::bind(the_succ, light_sensor_threshold));
    
    // move_func(36.0_in_to_tick, false, 100);
    // move_func(90.0_deg_to_tick, true, 100);
    // clear_screen();
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
    Timer timer{};
    main_pid.reset().set_gains((mode) ? pnt_turn_gains : straight_gains);
    chassis_obj.reset_trk();

    while (1) {
        int travel = (mode) ? 
            (chassis_obj.get_trk('l') - chassis_obj.get_trk('r')) / 2 :
            (chassis_obj.get_trk('l') + chassis_obj.get_trk('r')) / 2;
        int vol = std::clamp(main_pid.calculate(target, travel), old_vol_left - 450, old_vol_left + 450);

        chassis_obj.drive_vol(vol, (mode) ? -vol : vol);

        old_vol_left = vol;
        old_vol_right = (mode) ? -vol : vol;

        
        if (within_range(target, travel, 17))
            break;
        else if (timer.get() > max_time)
            break;

        pros::delay(10);
    }

    old_vol_left = 0;
    old_vol_right = 0;
    chassis_obj.drive_vol(0, 0);
    chassis_obj.reset_trk();
    pros::delay(delay);
}

//* macro defs
void the_succ(int range) {
    shooter_obj.shooter_set(600, 600, 600);
    pros::delay(10);
    int curr {}, prev {};
    prev = shooter_obj.get_light('b', true);
    while (1) {
        curr = shooter_obj.get_light('b', true);
        int diff = curr - prev;
        if (diff < range)
            break;
        prev = curr;
        pros::lcd::print(0, "b light: %i    ", curr);
        pros::lcd::print(1, "diff: %i    ", diff);
        pros::delay(the_succ_delay);
    }
    pros::delay(100);
    shooter_obj.shooter_set(0, 0, 0);
}

void the_succ_02(int delay) {
    shooter_obj.shooter_set(600, 600, 600);
    pros::delay(delay);
    shooter_obj.shooter_set(0, 0, 0);
}