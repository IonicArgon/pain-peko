//* Conveyor and intake source file
//* Created: 1/27/2021
//* Last modified: 1/30/2021

//* Main header
#include "main.h"

//* Hardware header
#include "lib/hardware/succ.hpp"

Shooter::Shooter(
    int prm_mtr_ct, int prm_mtr_cb, int prm_mtr_il, int prm_mtr_ir,
    int prm_lin_t,  int prm_lin_b,
    pros::motor_brake_mode_e    prm_brake,
    pros::motor_encoder_units_e prm_unit,
    pros::motor_gearset_e       prm_cart
)   : m_mtr_ct {std::abs(prm_mtr_ct)}, m_mtr_cb {std::abs(prm_mtr_cb)},
      m_mtr_il {std::abs(prm_mtr_il)}, m_mtr_ir {std::abs(prm_mtr_ir)},
      adi_lin_t {prm_lin_t}, adi_lin_b {prm_lin_b},
      m_brake {prm_brake}, m_unit {prm_unit}, m_cart {prm_cart}
{
    set_brake(prm_brake)
        .set_unit(prm_unit)
        .set_cart(prm_cart)
        .set_revr(prm_mtr_ct, prm_mtr_cb, prm_mtr_il, prm_mtr_ir)
        .conf_sen(prm_lin_t, prm_lin_b);
}

Shooter& Shooter::set_brake(pros::motor_brake_mode_e prm_brake)
{
    pros::c::motor_set_brake_mode(m_mtr_ct, prm_brake);
    pros::c::motor_set_brake_mode(m_mtr_cb, prm_brake);
    pros::c::motor_set_brake_mode(m_mtr_il, prm_brake);
    pros::c::motor_set_brake_mode(m_mtr_ir, prm_brake);
    return *this;
}

Shooter& Shooter::set_unit(pros::motor_encoder_units_e prm_unit)
{
    pros::c::motor_set_encoder_units(m_mtr_ct, prm_unit);
    pros::c::motor_set_encoder_units(m_mtr_cb, prm_unit);
    pros::c::motor_set_encoder_units(m_mtr_il, prm_unit);
    pros::c::motor_set_encoder_units(m_mtr_ir, prm_unit);
    return *this;
}

Shooter& Shooter::set_cart(pros::motor_gearset_e prm_cart)
{
    pros::c::motor_set_gearing(m_mtr_ct, prm_cart);
    pros::c::motor_set_gearing(m_mtr_cb, prm_cart);
    pros::c::motor_set_gearing(m_mtr_il, prm_cart);
    pros::c::motor_set_gearing(m_mtr_ir, prm_cart);
    return *this;
}

Shooter& Shooter::set_revr(int prm_mtr_ct, int prm_mtr_cb, int prm_mtr_il, int prm_mtr_ir)
{
    pros::c::motor_set_reversed(m_mtr_ct, (prm_mtr_ct > 0) ? false : true);
    pros::c::motor_set_reversed(m_mtr_cb, (prm_mtr_cb > 0) ? false : true);
    pros::c::motor_set_reversed(m_mtr_il, (prm_mtr_il > 0) ? false : true);
    pros::c::motor_set_reversed(m_mtr_ir, (prm_mtr_ir > 0) ? false : true);
    return *this;
}

Shooter& Shooter::conf_sen(int prm_lin_t, int prm_lin_b) {
    pros::c::adi_port_set_config(prm_lin_t, pros::E_ADI_ANALOG_IN);
    pros::c::adi_port_set_config(prm_lin_b, pros::E_ADI_ANALOG_IN);
    pros::c::adi_analog_calibrate(prm_lin_t);
    pros::c::adi_analog_calibrate(prm_lin_b);
    return *this;
}

int Shooter::get_line(char prm_line) {
    switch (prm_line) {
        case 't':
            return pros::c::adi_analog_read_calibrated(adi_lin_t);
        case 'b':
            return pros::c::adi_analog_read_calibrated(adi_lin_b);
    }
    return 0;
}

double Shooter::get_torq(char prm_mtr) {
    switch (prm_mtr) {
        case 't':
            return pros::c::motor_get_torque(m_mtr_ct);
        case 'b':
            return pros::c::motor_get_torque(m_mtr_cb);
        case 'r':
            return pros::c::motor_get_torque(m_mtr_ir);
        case 'l':
            return pros::c::motor_get_torque(m_mtr_il);
    };
    return 0.0;
}

void Shooter::shooter_set(int prm_vel_ct, int prm_vel_cb, int prm_vel_ik)
{
    pros::c::motor_move_velocity(m_mtr_ct, prm_vel_ct);
    pros::c::motor_move_velocity(m_mtr_cb, prm_vel_cb);
    pros::c::motor_move_velocity(m_mtr_il, prm_vel_ik);
    pros::c::motor_move_velocity(m_mtr_ir, prm_vel_ik);
}