//* Driver source file
//* Created: 1/27/2021
//* Last modified: 1/30/2021

//* Main header
#include "main.h"

//* Hardware header
#include "lib/hardware/drive.hpp"

Chassis::Chassis(
    int prm_mtr_lf, int prm_mtr_lb, int prm_mtr_rf, int prm_mtr_rb,
    int prm_trk_l, int prm_trk_r,
    pros::motor_brake_mode_e    prm_brake,
    pros::motor_encoder_units_e prm_unit,
    pros::motor_gearset_e       prm_cart
)   : m_mtr_lf {std::abs(prm_mtr_lf)}, m_mtr_lb {std::abs(prm_mtr_lb)}, m_mtr_rf {std::abs(prm_mtr_rf)}, m_mtr_rb {std::abs(prm_mtr_rb)},
      m_brake {prm_brake}, m_unit {prm_unit}, m_cart {prm_cart}
{
    set_brake(prm_brake);
    set_unit(prm_unit);
    set_cart(prm_cart);
    set_revr(prm_mtr_lf, prm_mtr_lb, prm_mtr_rf, prm_mtr_rb);
    conf_trk(prm_trk_l, prm_trk_r);
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

Chassis& Chassis::set_revr(int prm_mtr_lf, int prm_mtr_lb, int prm_mtr_rf, int prm_mtr_rb)
{
    pros::c::motor_set_reversed(m_mtr_lf, (prm_mtr_lf > 0) ? false : true);
    pros::c::motor_set_reversed(m_mtr_lb, (prm_mtr_lb > 0) ? false : true);
    pros::c::motor_set_reversed(m_mtr_rf, (prm_mtr_rf > 0) ? false : true);
    pros::c::motor_set_reversed(m_mtr_rb, (prm_mtr_rb > 0) ? false : true);
    return *this;
}

Chassis& Chassis::conf_trk(int prm_trk_l, int prm_trk_r)
{
    pros::c::adi_port_set_config(std::abs(prm_trk_l), pros::E_ADI_DIGITAL_IN);
    pros::c::adi_port_set_config(std::abs(prm_trk_l) + 1, pros::E_ADI_DIGITAL_IN);
    pros::c::adi_port_set_config(std::abs(prm_trk_r), pros::E_ADI_DIGITAL_IN);
    pros::c::adi_port_set_config(std::abs(prm_trk_r) + 1, pros::E_ADI_DIGITAL_IN);

    m_trk_l = pros::c::adi_encoder_init(
        std::abs(prm_trk_l),
        std::abs(prm_trk_l) + 1,
        (prm_trk_l > 0) ? false : true
    );
    m_trk_r = pros::c::adi_encoder_init(
        std::abs(prm_trk_r),
        std::abs(prm_trk_r) + 1,
        (prm_trk_r > 0) ? false : true
    );

    reset_trk();
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

void Chassis::reset_trk(void)
{
    pros::c::adi_encoder_reset(m_trk_l);
    pros::c::adi_encoder_reset(m_trk_r);
}

int Chassis::get_trk(char prm_trk_side)
{
    switch (prm_trk_side)
    {
    case 'l':
        return pros::c::adi_encoder_get(m_trk_l);
    case 'r':
        return pros::c::adi_encoder_get(m_trk_r);
    }
}