//* PID source file
//* Created: 1/27/2021
//* Last modified: 2/9/2021

//* Main header
#include "main.h"

//* Hardware header
#include "lib/control/pid.hpp"
#include "lib/hardware/drive.hpp"
#include "lib/hardware/globals.hpp"
#include "lib/utility/misc.hpp"

//* public definiitions

PID::PID(PID_gains prm_gains)
    : m_gain_kP {prm_gains.prm_gain_kP}, m_gain_kI {prm_gains.prm_gain_kI}, 
      m_gain_kD {prm_gains.prm_gain_kD}, m_min_Dt {prm_gains.prm_min_Dt},
      m_last_time {pros::millis()}, m_max_integrate{prm_gains.prm_max_integrate}
    {reset();}

PID& PID::reset(void)
{
    m_err = 0;
    m_last_err = 0;
    m_derv = 0.0;
    m_last_time = pros::millis();
    m_integral = 0.0;
    m_output = 0.0;
    m_Dt = 0;
}

PID& PID::set_gains(PID_gains prm_gains)
{
    m_gain_kP = prm_gains.prm_gain_kP;
    m_gain_kI = prm_gains.prm_gain_kP;
    m_gain_kD = prm_gains.prm_gain_kP;
}

int PID::calculate(int prm_target, int prm_current)
{
    // get current time
    int curr_time = pros::millis();

    // calculate error
    m_err = prm_target - prm_current;

    // calculate delta time
    m_Dt = curr_time - m_last_time;
    // abort if Dt is too small
    if (m_Dt < m_min_Dt) return m_output;
    // calc derivative
    m_derv = (m_err - m_last_time) / m_Dt;

    // dont integrate if error is too large
    if (m_err > m_max_integrate)
        m_integral = 0;
    // calc integral
    m_integral += m_err;


    // calc output
    m_output = std::round(
        (m_err * m_gain_kP) + (m_integral * m_gain_kI) + (m_derv * m_gain_kD)
    );
    // limit output
    if (std::abs(m_output) > 12000)
        m_output = std::copysign(12000, m_output);

    // save previous values
    m_last_err = m_err;
    m_last_time = curr_time;

    return m_output;
}

double PID::get_derv(void)
{
    return m_derv;
}