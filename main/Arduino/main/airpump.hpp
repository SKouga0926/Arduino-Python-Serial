#ifndef INCLUDE_DLIB_AIRPUMP_HPP
#define INCLUDE_DLIB_AIRPUMP_HPP

#include "pin_definition.hpp"

namespace Dcon
{

class AirPump
{
public:
  AirPump(Pin::AirPump pin_forward_, Pin::AirPump pin_reverse_);

  void motorStop();
  void motorForward();
  void motorReverse();
  void motorBrake();

  void motorSetPower(float power_);

private:
  //const uint8_t m_pin_in;
  const uint8_t m_pin_forward;    //forward
  const uint8_t m_pin_reverse;    //reverse
};

}

#endif // INCLUDE_DLIB_AIRPUMP_HPP