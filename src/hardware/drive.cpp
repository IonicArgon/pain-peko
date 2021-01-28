//* Driver source file
//* Created: 1/27/2021
//* Last modified: 1/27/2021

//* Main header
#include "main.h"

//* Hardware header
#include "lib/hardware/drive.hpp"

Chassis::Chassis(
    int prm_mtr_lf, int prm_mtr_lb, int prm_mtr_rf, int prm_mtr_rb,
    int prm_trk_1a, int prm_trk_1b, int prm_trk_2a, int prm_trk_2b,
    pros::motor_brake_mode_e    prm_brake = pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST,
    pros::motor_encoder_units_e prm_unit = pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS,
    pros::motor_gearset_e       prm_cart = pros::motor_gearset_e::E_MOTOR_GEARSET_18
)   : m_mtr_lf {prm_mtr_lf}, m_mtr_lb {prm_mtr_lb}, m_mtr_rf {prm_mtr_rf}, m_mtr_rb {prm_mtr_rb},
      m_trk_1a {prm_trk_1a}, m_trk_1b {prm_trk_1b}, m_trk_2a {prm_trk_2a}, m_trk_2b {prm_trk_2b},
      m_brake  {prm_brake},  m_unit   {prm_unit},   m_cart   {prm_cart}
{
    set_brake(prm_brake);
    set_unit(prm_unit);
    set_cart(prm_cart);
}

Chassis& Chassis::set_brake(pros::motor_brake_mode_e prm_brake)
{
    pros::c::motor_set_brake_mode(m_mtr_lf, prm_brake);
    pros::c::motor_set_brake_mode(m_mtr_lb, prm_brake);
    pros::c::motor_set_brake_mode(m_mtr_rf, prm_brake);
    pros::c::motor_set_brake_mode(m_mtr_rb, prm_brake);
    return *this;
}

Chassis& Chassis::set_unit(pros::motor_encoder_units_e prm_unit)
{
    pros::c::motor_set_encoder_units(m_mtr_lf, prm_unit);
    pros::c::motor_set_encoder_units(m_mtr_lb, prm_unit);
    pros::c::motor_set_encoder_units(m_mtr_rf, prm_unit);
    pros::c::motor_set_encoder_units(m_mtr_rb, prm_unit);
    return *this;
}

Chassis& Chassis::set_cart(pros::motor_gearset_e prm_cart)
{
    pros::c::motor_set_gearing(m_mtr_lf, prm_cart);
    pros::c::motor_set_gearing(m_mtr_lb, prm_cart);
    pros::c::motor_set_gearing(m_mtr_rf, prm_cart);
    pros::c::motor_set_gearing(m_mtr_rb, prm_cart);
    return *this;
}

void Chassis::drive_mov(int prm_mov_l, int prm_mov_r)
{
    pros::c::motor_move(m_mtr_lf, prm_mov_l);
    pros::c::motor_move(m_mtr_lb, prm_mov_l);
    pros::c::motor_move(m_mtr_rf, prm_mov_r);
    pros::c::motor_move(m_mtr_rb, prm_mov_r);
}

void Chassis::drive_vel(int prm_vel_l, int prm_vel_r)
{
    pros::c::motor_move_velocity(m_mtr_lf, prm_vel_l);
    pros::c::motor_move_velocity(m_mtr_lb, prm_vel_l);
    pros::c::motor_move_velocity(m_mtr_rf, prm_vel_r);
    pros::c::motor_move_velocity(m_mtr_rb, prm_vel_r);
}

void Chassis::drive_vol(int prm_vol_l, int prm_vol_r)
{
    pros::c::motor_move_voltage(m_mtr_lf, prm_vol_l);
    pros::c::motor_move_voltage(m_mtr_lb, prm_vol_l);
    pros::c::motor_move_voltage(m_mtr_rf, prm_vol_r);
    pros::c::motor_move_voltage(m_mtr_rb, prm_vol_r);
}