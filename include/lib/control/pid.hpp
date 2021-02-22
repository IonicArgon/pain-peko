//* PID header file
//* Created: 1/27/2021
//* Last modified: 2/9/2021

#ifndef H_PID_HPP
#define H_PID_HPP

#include "api.h"

struct PID_gains
{
    double prm_gain_kP;
    double prm_gain_kI;
    double prm_gain_kD;
    int prm_min_Dt;
    int prm_max_integrate;
};

class PID
{
public:
    explicit PID(PID_gains prm_gains);
    PID& reset(void);
    PID& set_gains(PID_gains prm_gains);
    int calculate(int prm_target, int prm_current);
    double get_derv(void);


private:
    double m_gain_kP;
    double m_gain_kI;
    double m_gain_kD;
    int m_min_Dt;

    int m_err;
    int m_last_err;

    double m_derv;
    int m_last_time;
    int m_Dt;

    double m_integral;
    int m_max_integrate;

    int m_output;
};

#endif // H_PID_HP