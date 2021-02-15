//* Teleop control code
//* Created: 1/27/2021
//* Last modified: 1/30/2021

//* Main header
#include "main.h"

//* Subsystem headers
#include "lib/hardware/drive.hpp"
#include "lib/hardware/succ.hpp"
#include "lib/hardware/globals.hpp"

//* local globals for funnies
int deadzone {10};
int cycle_delay {15};
int shoot_delay {15};
int counter{0};

//* macro forward declarations
void indexing(void);
void cycling(void);
void shooting(void);
void set_itk(int speed);
void eject(bool itk_yes);

int check_deadzone(int val)
{
    return (std::abs(val) > deadzone) ? val : 0;
}

void driving(void)
{
    while (!(pros::competition::is_autonomous() || pros::competition::is_disabled()))
    {
        int pow {check_deadzone(ctrl_m.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y))},
            trn {check_deadzone(ctrl_m.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X))};
        chassis_obj.drive_mov(pow + trn, pow - trn);
        pros::delay(10);
    }
}

void controls(void)
{
    while (!(pros::competition::is_autonomous() || pros::competition::is_disabled()))
    {
        if (ctrl_m.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
            indexing();
        else if (ctrl_m.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
            cycling();
        else if (ctrl_m.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
            shooting();
        else if (ctrl_m.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
            set_itk(600);
        else if (ctrl_m.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
            eject(true);
        else if (ctrl_m.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
            eject(false);
        else if (ctrl_m.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
            set_itk(-600);
        else if (ctrl_m.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
            set_itk(-200);
        else
        {
            shooter_obj.shooter_set(0, 0, 0);
            counter = 0;
        }
        pros::delay(10);
    }
}

//* Teleop callback
void opcontrol(void)
{
    chassis_obj.set_brake(pros::E_MOTOR_BRAKE_COAST);
    pros::Task task_drive{driving};
    pros::Task task_ctrll{controls};
}


//* all macros

void indexing(void)
{
    //TODO: add more precise indexing when we get light sensors
    shooter_obj.shooter_set(0, 600, 600);
}

void cycling(void)
{
    ++counter;
    shooter_obj.shooter_set(
        600,
        (counter > cycle_delay) ? 600 : 0,
        (counter > cycle_delay) ? 600 : 0
    );
}

void shooting(void)
{
    ++counter;
    shooter_obj.shooter_set(
        600,
        (counter > cycle_delay) ? 600 : 0
    );
}

void set_itk(int speed)
{
    shooter_obj.shooter_set(0, 0, speed);
}

void eject(bool itk_yes)
{
    shooter_obj.shooter_set(
        -600,
        -600,
        (itk_yes == true) ? -600 : 0
    );
}