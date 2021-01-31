//* Globals storage source file
//* Created: 1/30/2021
//* Last modified: 1/30/2021

//* Main header
#include "main.h"

//* Hardware header
#include "lib/hardware/globals.hpp"

Chassis* chassis_obj;
Shooter* shooter_obj;

//* Global controller because i need it
pros::Controller ctrl_m{pros::controller_id_e_t::E_CONTROLLER_MASTER};