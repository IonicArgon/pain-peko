//* PID source file
//* Created: 1/27/2021
//* Last modified: 1/31/2021

//* Main header
#include "main.h"

//* Hardware header
#include "lib/control/pid.hpp"
#include "lib/hardware/drive.hpp"
#include "lib/hardware/globals.hpp"
#include "lib/utility/misc.hpp"

//* public defs

pid_control::pid_control(pid_gains gains)
    : m_gain_kP {gains.gain_kP}, m_gain_kD {gains.gain_kD}, m_gain_delta_t {gains.gain_delta_t}, 
      m_gain_tick_range {gains.gain_tick_range}, m_gain_slew_rate {gains.gain_slew_rate},
      task_calc_dist {std::bind(&pid_control::calc_dist, this)},
      task_calc_turn {std::bind(&pid_control::calc_turn, this)},
      task_calc_crve {std::bind(&pid_control::calc_crve, this)}
{
    reset();
}

pid_control& pid_control::reset()
{
    m_target_dist = 0;
    m_target_rot = 0.0;
    m_targ_l = 0.0;
    m_targ_r = 0.0;
    m_err_l = 0.0;
    m_err_r = 0.0;
    m_last_err_l = 0.0;
    m_last_err_r = 0.0;
    m_derv_l = 0.0;
    m_derv_r = 0.0;
    m_old_vel_l = 0;
    m_old_vel_r = 0;
    m_waiting_for_settle = false;

    task_calc_dist.suspend();
    task_calc_turn.suspend();
    task_calc_crve.suspend();

    chassis_obj->reset_trk();

    return *this;
}

pid_control& pid_control::wait_until_settled()
{
    m_waiting_for_settle = true;
    return *this;
}

pid_control& pid_control::set_gains(pid_gains gains)
{
    m_gain_kP = gains.gain_kP;
    m_gain_kD = gains.gain_kD;
    m_gain_delta_t = gains.gain_delta_t;
    m_gain_tick_range = gains.gain_tick_range;
    m_gain_slew_rate = gains.gain_slew_rate;
    return *this;
}

pid_control& pid_control::set_rot_targ(int rot_ticks)
{
    m_target_rot = rot_ticks;
    return *this;
}

pid_control& pid_control::set_dist_targ(int dist_ticks)
{
    m_target_dist = dist_ticks;
    return *this;
}

void pid_control::start()
{
    if (!(approx_float_eq(m_target_dist, 0.0, 1e-12, 1e-8)) && !(approx_float_eq(m_target_rot, 0.0, 1e-12, 1e-8)))
    {
        task_calc_crve.resume();
        task_calc_crve.notify();
        if (m_waiting_for_settle == true)
            while (task_calc_crve.get_state() == pros::E_TASK_STATE_RUNNING)
                pros::delay(1);
    }
    else if (!(approx_float_eq(m_target_dist, 0.0, 1e-12, 1e-8)) && approx_float_eq(m_target_rot, 0.0, 1e-12, 1e-8))
    {
        task_calc_dist.resume();
        task_calc_dist.notify();
        if (m_waiting_for_settle == true)
            while (task_calc_dist.get_state() == pros::E_TASK_STATE_RUNNING)
                pros::delay(1);
    }
    else if (approx_float_eq(m_target_dist, 0.0, 1e-12, 1e-8) && !(approx_float_eq(m_target_rot, 0.0, 1e-12, 1e-8)))
    {
        task_calc_turn.resume();
        task_calc_turn.notify();
        if (m_waiting_for_settle == true)
            while (task_calc_turn.get_state() == pros::E_TASK_STATE_RUNNING)
                pros::delay(1);
    }

    reset();
}

//TODO: write the actual PD loops for straight, point turn, and curve turn