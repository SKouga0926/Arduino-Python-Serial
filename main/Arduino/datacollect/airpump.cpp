#include "airpump.hpp"

AirPump::AirPump(Pin::AirPump pin_forward_, Pin::AirPump pin_reverse_) : m_pin_forward((uint8_t)pin_forward_), m_pin_reverse((uint8_t)pin_reverse_)
{
  pinMode(m_pin_forward, OUTPUT);
  pinMode(m_pin_reverse, OUTPUT);
}


void AirPump::motorStop()
{
  digitalWrite(m_pin_forward, LOW);
  digitalWrite(m_pin_reverse, LOW);
}

void AirPump::motorForward()
{
  digitalWrite(m_pin_forward, HIGH);
  digitalWrite(m_pin_reverse, LOW);
}

void AirPump::motorReverse()
{
  digitalWrite(m_pin_forward, LOW);
  digitalWrite(m_pin_reverse, HIGH);
}

void AirPump::motorBrake()
{
  digitalWrite(m_pin_forward, HIGH);
  digitalWrite(m_pin_reverse, HIGH);
}

void AirPump::motorSetPower(float power_)
{
  if (power_ >= 100)
  {
    power_ = 100;
  }

  if (power_ < 0)
  {
    power_ = 0;
  }

  int volts = (int)((255 / 100.0) * power_);

  digitalWrite(m_pin_forward, volts);
  digitalWrite(m_pin_reverse, 0);


  // Serial.print("power_ :");
  // Serial.println(power_);
  // Serial.print("volts :");
  // Serial.println(volts);

}

