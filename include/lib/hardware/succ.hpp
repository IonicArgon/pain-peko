//* Conveyor and intake header file

#ifndef H_SUCC_HPP
#define H_SUCC_HPP

#include "api.h"

class Shooter
{
public:
    Shooter(
        int prm_mtr_ct, int prm_mtr_cb, int prm_mtr_il, int prm_mtr_ir,
        int prm_lht_t,  int prm_lht_b,
        pros::motor_brake_mode_e    prm_brake = pros::motor_brake_mode_e::E_MOTOR_BRAKE_BRAKE,
        pros::motor_encoder_units_e prm_unit = pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS,
        pros::motor_gearset_e       prm_cart = pros::motor_gearset_e::E_MOTOR_GEARSET_06
    );
    Shooter& set_brake(pros::motor_brake_mode_e prm_brake);
    Shooter& set_unit(pros::motor_encoder_units_e prm_unit);
    Shooter& set_cart(pros::motor_gearset_e prm_cart);
    Shooter& set_revr(int prm_mtr_ct, int prm_mtr_cb, int prm_mtr_il, int prm_mtr_ir);
    Shooter& conf_sen(int prm_lht_t, int prm_lht_b);
    Shooter& cali_sen(void);
    int get_light(char prm_lht, bool uncali = false);
    double get_torq(char prm_mtr);
    void shooter_set(int prm_vel_ct, int prm_vel_cb, int prm_vel_ik = 0);

private:
    int m_mtr_ct, m_mtr_cb, m_mtr_il, m_mtr_ir;
    int adi_lht_t, adi_lht_b;
    pros::motor_brake_mode_e        m_brake;
    pros::motor_encoder_units_e     m_unit;
    pros::motor_gearset_e           m_cart;

};


#endif // H_SUCC_HPP