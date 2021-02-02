//* PID header file
//* Created: 1/27/2021
//* Last modified: 1/31/2021

#ifndef H_PID_HPP
#define H_PID_HPP

#include "api.h"

struct pid_gains
{
    double gain_kP;
    double gain_kD;
    double gain_delta_t;
    double gain_tick_range;
    int gain_slew_rate;
};

class pid_control
{
public:
    pid_control(pid_gains gains);
    pid_control& reset();
    pid_control& wait_until_settled();
    pid_control& set_gains(pid_gains gains);
    pid_control& set_rot_targ(int rot_ticks);
    pid_control& set_rot_targ(double rot_degs);
    pid_control& set_dist_targ(int dist_ticks);
    void start();

private:
    double  m_gain_kP,
            m_gain_kD,
            m_gain_delta_t,
            m_gain_tick_range;
    int     m_gain_slew_rate;
    
    int m_target_dist;
    int m_target_rot_ticks;
    double m_target_rot_degs;

    double m_targ_l, m_targ_r;
    double m_err_l, m_err_r;
    double m_last_err_l, m_last_err_r;
    double m_derv_l, m_derv_r;
    int m_old_vel_l, m_old_vel_r;

    bool m_waiting_for_settle;

    void calc_dist_turn();
    void calc_crve();

    pros::Task task_calc_dist_turn;
    pros::Task task_calc_crve;
};

#endif // H_PID_HP