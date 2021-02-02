//* Initialization code
//* Created: 1/27/2021
//* Last modified: 1/31/2021

//* Main header
#include "main.h"

//* Subsystem headers
#include "lib/hardware/drive.hpp"
#include "lib/hardware/succ.hpp"
#include "lib/hardware/globals.hpp"

void clear_screen(void)
{
    for (int i{0}; i < 7; ++i)
        pros::lcd::clear_line(i);
}

//* selection screen
void selection(void)
{
    pros::lcd::print(0, "L1 and L2 to scroll");
    pros::lcd::print(1, "A to select");

    int curr_sel {0};

    while (true)
    {
        switch (curr_sel)
        {
        case 0:
            pros::lcd::print(2, "auto: live  ");
            path_selection = auto_select::LIVE;
            break;
        case 1:
            pros::lcd::print(2, "auto: skills");
            path_selection = auto_select::SKILLS;
            break;
        }

        if (ctrl_m.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
            {if (curr_sel > 0) {--curr_sel; ctrl_m.rumble(".");}}
        else if (ctrl_m.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
            {if (curr_sel < 1) {++curr_sel; ctrl_m.rumble(".");}}
        else if (ctrl_m.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
            {ctrl_m.rumble("-"); break;}
        
        pros::delay(10);
    }

    clear_screen();
}

//* Initialization callback
void initialize(void)
{
    pros::lcd::initialize();
    chassis_obj = new Chassis{19, 20, -9, -10, 5, -1};
    shooter_obj = new Shooter{-15, -8, 17, -7};

    selection();
}

//* Competition Initialize callback
void competition_initialize(void)
{

}

//* Disbled callback
void disabled(void)
{

}