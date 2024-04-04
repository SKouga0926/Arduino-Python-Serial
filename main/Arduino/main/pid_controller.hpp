#ifndef INCLUDE_DLIB_PID_CONTROLLER_HPP
#define INCLUDE_DLIB_PID_CONTROLLER_HPP

#include "pin_definition.hpp"

namespace Dcon
{

class PidController
{
public:
    PidController(double kp_, double ki_, double kd_);
    void init();
    double calc(double target_value_, double present_value_, double dt_);

private:
    
    double m_kp;
    double m_ki;
    double m_kd;

    double m_enable_inte_range;

    double m_error_now;
    double m_error_last;
    double m_integral_error;
    double m_differential_error;
    
};

}

#endif // INCLUDE_DLIB_PID_CONTROLLER_HPP