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
    m_err = 0.0;
    m_last_err = 0.0;
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
    return *this;
}

int PID::calculate(int prm_target, int prm_current)
{
    // calculate error
    m_err = (double)prm_target - (double)prm_current;

    // calc derivative
    m_derv = (m_err - m_last_err) / m_min_Dt;

    m_integral += m_err * m_min_Dt;

    // dont integrate if error is too large
    if (std::abs(m_err) > m_max_integrate || approx_float_eq(m_err, 0.0, 1e-12, 1e-8))
    {
        m_integral = 0.0;
    }


    // calc output
    m_output = (m_err * m_gain_kP) + (m_integral * m_gain_kI) + (m_derv * m_gain_kD);
    pros::lcd::print(2, "ERR: %f\t DIR: %f", m_err, m_derv);
    pros::lcd::print(3, "INT: %f\t OUT: %i", m_integral, m_output);

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