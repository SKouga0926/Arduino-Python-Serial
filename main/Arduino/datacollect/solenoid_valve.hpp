#ifndef INCLUDE_DLIB_SOLENOID_VALVE_HPP
#define INCLUDE_DLIB_SOLENOID_VALVE_HPP

#include "pin_definition.hpp"


class SolenoidValve
{
public:
    SolenoidValve(Pin::SolenoidValve pin_in_);
    void openValve();
    void closeValve();
private:
    const uint8_t m_pin_in;
};

#endif // INCLUDE_DLIB_SOLENOID_VALVE_HPP