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
        int prm_trk_l, int prm_trk_r,
        pros::motor_brake_mode_e    prm_brake = pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST,
        pros::motor_encoder_units_e prm_unit = pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS,
        pros::motor_gearset_e       prm_cart = pros::motor_gearset_e::E_MOTOR_GEARSET_18
    );
    Chassis& set_brake(pros::motor_brake_mode_e prm_brake);
    Chassis& set_unit(pros::motor_encoder_units_e prm_unit);
    Chassis& set_cart(pros::motor_gearset_e prm_cart);
    Chassis& set_revr(int prm_mtr_lf, int prm_mtr_lb, int prm_mtr_rf, int prm_mtr_rb);
    Chassis& conf_trk(int prm_trk_l, int prm_trk_r);
    void drive_mov(int prm_mov_l, int prm_mov_r);
    void drive_vel(int prm_vel_l, int prm_vel_r);
    void drive_vol(int prm_vol_l, int prm_vol_r);
    void reset_trk(void);
    int get_trk(char prm_trk_side);

private:
    int m_mtr_lf, m_mtr_lb, m_mtr_rf, m_mtr_rb;
    pros::c::adi_encoder_t          m_trk_l, m_trk_r;
    pros::motor_brake_mode_e        m_brake;
    pros::motor_encoder_units_e     m_unit;
    pros::motor_gearset_e           m_cart;

};

#endif // H_CHASSIS_HPP