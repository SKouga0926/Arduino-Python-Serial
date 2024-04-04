#include "air_sensor.hpp"

Airsensor::Airsensor(byte channel_): m_channel(channel_)
{
  m_settings = SPISettings(1000000, MSBFIRST, SPI_MODE0);
}

float Airsensor::getAirPressure()
{
  byte channel_data_H2 = ((m_channel & 0x0f) + 8) >> 2;
  byte channel_data_L2 = m_channel & 0x03;
  SPI.beginTransaction(m_settings);

  digitalWrite(SS, LOW);
  SPI.transfer(channel_data_H2 + 4);                  // Start bit 1 + D2bit
  byte high_byte = SPI.transfer(channel_data_L2 << 6); // singleEnd D1,D0 bit
  byte low_byte = SPI.transfer(0x00);                // dummy
  digitalWrite(SS, HIGH);

  SPI.endTransaction();
  uint32_t data_ch = ((high_byte & 0x0f) << 8) + low_byte; // 8ビットずつずらす
  float volts = (data_ch * 5.0) / 4096 * 3;

  return volts;

  // Serial.println("CH" + String(m_channel) + ": " + String(volts, 3) + "V");
  // Serial.println();
  // delay(1000);
}