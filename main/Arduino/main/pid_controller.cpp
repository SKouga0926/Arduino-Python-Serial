#include "pid_controller.hpp"

namespace Dcon
{

PidController::PidController(double kp_, double ki_, double kd_) : m_kp(kp_), m_ki(ki_), m_kd(kd_)
{
  if (kp_ < 0) m_kp = 0;
  if (ki_ < 0) m_ki = 0;
  if (kd_ < 0) m_kd = 0;

}

void PidController::init()
{   
  m_enable_inte_range = 10; // 試験が始まり次第決める

  m_error_now = 0;
  m_error_last = 0;
  m_integral_error = 0;
  m_differential_error = 0;
}

double PidController::calc(double target_value_, double present_value_, double dt_)
{
  m_error_last = m_error_now;
  m_error_now = target_value_ - present_value_;

  if (fabsf(m_integral_error) < m_enable_inte_range)
  {
    m_integral_error += ((m_error_last + m_error_now) / 2.0) * dt_;
  }

  else
  {
    if (m_integral_error > 0) m_integral_error = m_enable_inte_range;
    if (m_integral_error < 0) m_integral_error = -m_enable_inte_range;
  }

  m_differential_error = (m_error_now - m_error_last) / dt_;

  double output = m_kp * m_error_now + m_ki * m_integral_error + m_kd * m_differential_error;

  return output;
}

}
