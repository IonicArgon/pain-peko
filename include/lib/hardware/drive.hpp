//* Driver header file
//* Created: 1/27/2021
//* Last modified: 1/27/2021

#ifndef H_CHASSIS_HPP
#define H_CHASSIS_HPP

#include "api.h"

class Chassis
{
public:
    Chassis(
        int prm_mtr_lf, int prm_mtr_lb, int prm_mtr_rf, int prm_mtr_rb,
        int prm_trk_1a, int prm_trk_1b, int prm_trk_2a, int prm_trk_2b,
        pros::motor_brake_mode_e    prm_brake = pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST,
        pros::motor_encoder_units_e prm_unit = pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS,
        pros::motor_gearset_e       prm_cart = pros::motor_gearset_e::E_MOTOR_GEARSET_18
    );
    Chassis& set_brake(pros::motor_brake_mode_e prm_brake);
    Chassis& set_unit(pros::motor_encoder_units_e prm_unit);
    Chassis& set_cart(pros::motor_gearset_e prm_cart);
    void drive_mov(int prm_mov_l, int prm_mov_r);
    void drive_vel(int prm_vel_l, int prm_vel_r);
    void drive_vol(int prm_vol_l, int prm_vol_r);

private:
    int m_mtr_lf, m_mtr_lb, m_mtr_rf, m_mtr_rb;
    int m_trk_1a, m_trk_1b, m_trk_2a, m_trk_2b;
    pros::motor_brake_mode_e        m_brake;
    pros::motor_encoder_units_e     m_unit;
    pros::motor_gearset_e           m_cart;

};

#endif // H_CHASSIS_HPP