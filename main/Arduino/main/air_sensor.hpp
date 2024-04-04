#ifndef INCLUDE_DLIB_AIR_SENSOR_HPP
#define INCLUDE_DLIB_AIR_SENSOR_HPP

#include "pin_definition.hpp"
#include <SPI.h>

#define SS 10

namespace Dcon
{

class Airsensor
{
public:
  Airsensor(byte channel_);
  float getAirPressure();
private:
  byte m_channel;
  SPISettings m_settings;
};

}

#endif // INCLUDE_DLIB_AIR_SENSOR_HPP