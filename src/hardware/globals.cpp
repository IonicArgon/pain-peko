//* Globals storage source file
//* Created: 1/30/2021
//* Last modified: 1/30/2021

//* Main header
#include "main.h"

//* Hardware header
#include "lib/hardware/globals.hpp"

Chassis chassis_obj {19, 20, -9, -10, -5, 1};
Shooter shooter_obj {-15, -8, 17, -7, 4, 3};

//* Global controller because i need it
pros::Controller ctrl_m{pros::controller_id_e_t::E_CONTROLLER_MASTER};

//* funny auto select thing
auto_select path_selection;