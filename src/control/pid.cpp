//* PID source file

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
      m_last_time {(int)pros::millis()}, m_max_integrate{prm_gains.prm_max_integrate}
    {reset();}

PID& PID::reset(void)
{
    m_err = 0;
    m_last_err = 0;
    m_derv = 0.0;
    m_integral = 0.0;
    m_output = 0;
    return *this;
}

PID& PID::set_gains(PID_gains prm_gains)
{
    m_gain_kP = prm_gains.prm_gain_kP;
    m_gain_kI = prm_gains.prm_gain_kP;
    m_gain_kD = prm_gains.prm_gain_kP;
    m_min_Dt = prm_gains.prm_min_Dt;
    m_max_integrate = prm_gains.prm_max_integrate;
    return *this;
}

int PID::calculate(int prm_target, int prm_current)
{
    // calculate error
    m_err = prm_target - prm_current;

    // calc derivative
    m_derv = ((double)m_err - (double)m_last_err) / (double)m_min_Dt;    

    // calc output
    m_output = (m_err * m_gain_kP) /*+ m_integral*/ + (m_derv * m_gain_kD);

    // limit output
    if (std::abs(m_output) > 12000)
        m_output = std::copysign(12000, m_output);

    // save previous values
    m_last_err = m_err;

    return m_output;
}

double PID::get_derv(void)
{
    return m_derv;
}