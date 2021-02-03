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
      task_calc_dist_turn {std::bind(&pid_control::calc_dist_turn, this)},
      task_calc_crve {std::bind(&pid_control::calc_crve, this)}
{
    reset();
}

pid_control& pid_control::reset()
{
    m_target_dist = 0;
    m_target_rot_ticks = 0;
    m_target_rot_degs = 0.0;
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

    task_calc_dist_turn.suspend();
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
    m_target_rot_ticks = rot_ticks;
    return *this;
}

pid_control& pid_control::set_rot_targ(double rot_degs)
{
    m_target_rot_degs = rot_degs;
    return *this;
}

pid_control& pid_control::set_dist_targ(int dist_ticks)
{
    m_target_dist = dist_ticks;
    return *this;
}

void pid_control::start()
{
    if (m_target_dist && !(approx_float_eq(m_target_rot_degs, 0.0, 1e-12, 1e-8)))
    {
        task_calc_crve.resume();
        task_calc_crve.notify();
        if (m_waiting_for_settle == true)
            while (task_calc_crve.get_state() == pros::E_TASK_STATE_RUNNING)
                pros::delay(1);
    }
    else if (m_target_dist)
    {
        m_targ_l = m_target_dist;
        m_targ_r = m_target_dist;

        task_calc_dist_turn.resume();
        task_calc_dist_turn.notify();
        if (m_waiting_for_settle == true)
            while (task_calc_dist_turn.get_state() == pros::E_TASK_STATE_RUNNING)
                pros::delay(1);
    }
    else if (m_target_rot_ticks)
    {
        m_targ_l = m_target_rot_ticks;
        m_targ_r = -m_target_rot_ticks;

        task_calc_dist_turn.resume();
        task_calc_dist_turn.notify();
        if (m_waiting_for_settle == true)
            while (task_calc_dist_turn.get_state() == pros::E_TASK_STATE_RUNNING)
                pros::delay(1);
    }

    reset();
}

//TODO: figure out how to do PID curve later

void pid_control::calc_dist_turn()
{
    while (task_calc_dist_turn.notify_take(true, TIMEOUT_MAX))
    {
        while (std::abs(m_target_dist - ((chassis_obj->get_trk('l') + chassis_obj->get_trk('r')) / 2)) >= m_gain_tick_range)
        {
            // calc errors
            m_err_l = m_targ_l - chassis_obj->get_trk('l');
            m_err_r = m_targ_r - chassis_obj->get_trk('r');

            // calc derivatives
            m_derv_l = (m_err_l - m_last_err_l) / m_gain_tick_range;
            m_derv_r = (m_err_r - m_last_err_r) / m_gain_tick_range;

            // assign outputs
            int output_l = static_cast<int>(std::round((m_err_l * m_gain_kP) + (m_derv_l * m_gain_kD)));
            int output_r = static_cast<int>(std::round((m_err_r * m_gain_kP) + (m_derv_r * m_gain_kD)));

            // clamp values to max and min values
            output_l = std::copysign(
                std::clamp(std::abs(output_l), 3000, 12000),
                output_l
            );
            output_r = std::copysign(
                std::clamp(std::abs(output_r), 3000, 12000),
                output_r
            );

            // slew vals to lower jerk
            output_l = std::clamp(output_l, m_old_vel_l - m_gain_slew_rate, m_old_vel_l + m_gain_slew_rate);
            output_r = std::clamp(output_r, m_old_vel_r - m_gain_slew_rate, m_old_vel_r + m_gain_slew_rate);
            
            // set prev errors
            m_last_err_l = m_err_l;
            m_last_err_r = m_err_r;

            // save old output
            m_old_vel_l = output_l;
            m_old_vel_r = output_r;        

            chassis_obj->drive_vol(output_l, output_r);
            pros::delay(m_gain_delta_t);
        }
    }
}