//* Globals storage header file
//* Created: 1/30/2021
//* Last modified: 1/30/2021

#ifndef H_GLOBALS_HPP
#define H_GLOBALS_HPP

#include "api.h"
#include "lib/hardware/drive.hpp"
#include "lib/hardware/succ.hpp"

extern Chassis chassis_obj;
extern Shooter shooter_obj;
extern pros::Controller ctrl_m;

enum class auto_select
{
    LIVE,
    SKILLS
};

extern auto_select path_selection;

#endif // H_GLOBALS_HPP
