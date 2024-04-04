#include "solenoid_valve.hpp"

SolenoidValve::SolenoidValve(Pin::SolenoidValve pin_in_) : m_pin_in((uint8_t)pin_in_)
{
    pinMode(m_pin_in, OUTPUT);
}

void SolenoidValve::openValve()
{
    digitalWrite(m_pin_in, HIGH);
}

void SolenoidValve::closeValve()
{
    digitalWrite(m_pin_in, LOW);
}