#include "loadcell.hpp"

namespace Dcon
{

Loadcell::Loadcell(Pin::LoadCell pin_in_1, Pin::LoadCell pin_in_2, Pin::LoadCell pin_in_3, Pin::LoadCell pin_in_4) 
: m_pin_in_1((uint8_t)pin_in_1), m_pin_in_2((uint8_t)pin_in_2), m_pin_in_3((uint8_t)pin_in_3), m_pin_in_4((uint8_t)pin_in_4) 
{
  pinMode(m_pin_in_1, INPUT);
  pinMode(m_pin_in_2, INPUT);
  pinMode(m_pin_in_3, INPUT);
  pinMode(m_pin_in_4, INPUT);
}

void Loadcell::init()
{
    m_offset_value_1 = 0;
    m_offset_value_2 = 0;
    m_offset_value_3 = 0;
    m_offset_value_4 = 0;

    m_pressure_1 = 0;
    m_pressure_2 = 0;
    m_pressure_3 = 0;
    m_pressure_4 = 0;

    for (char i = 0; i < 24; i++)
    {
        digitalWrite((uint8_t)Pin::LoadCell::Clk, 1);
        delayMicroseconds(1);
        digitalWrite((uint8_t)Pin::LoadCell::Clk, 0);
        delayMicroseconds(1);
        m_offset_value_1 = (m_offset_value_1 << 1) | (digitalRead(m_pin_in_1));
        m_offset_value_2 = (m_offset_value_2 << 1) | (digitalRead(m_pin_in_2));
        m_offset_value_3 = (m_offset_value_3 << 1) | (digitalRead(m_pin_in_3));
        m_offset_value_4 = (m_offset_value_4 << 1) | (digitalRead(m_pin_in_4));
    }

    m_offset_value_1 = m_offset_value_1 ^ 0x800000; // 2の補数で出力されるアナログ値を符号付の整数に変換
    m_offset_value_2 = m_offset_value_2 ^ 0x800000;
    m_offset_value_3 = m_offset_value_3 ^ 0x800000;
    m_offset_value_4 = m_offset_value_4 ^ 0x800000;
}

void Loadcell::calcPressure()
{
  long raw_value_1 = 0; // 測定した圧力
  long raw_value_2 = 0;
  long raw_value_3 = 0;
  long raw_value_4 = 0;
  
    for (char i = 0; i < 24; i++)
    {
        digitalWrite((uint8_t)Pin::LoadCell::Clk, 1);
        delayMicroseconds(1);
        digitalWrite((uint8_t)Pin::LoadCell::Clk, 0);
        delayMicroseconds(1);
        raw_value_1 = (raw_value_1 << 1) | (digitalRead(m_pin_in_1));
        raw_value_2 = (raw_value_2 << 1) | (digitalRead(m_pin_in_2));
        raw_value_3 = (raw_value_3 << 1) | (digitalRead(m_pin_in_3));
        raw_value_4 = (raw_value_4 << 1) | (digitalRead(m_pin_in_4));
    }
    raw_value_1 = raw_value_1 ^ 0x800000; // 2の補数で出力されるアナログ値を符号付の整数に変換
    raw_value_2 = raw_value_2 ^ 0x800000;
    raw_value_3 = raw_value_3 ^ 0x800000;
    raw_value_4 = raw_value_4 ^ 0x800000;

    m_pressure_1 = ((raw_value_1 - m_offset_value_1) / 1000) * 2.2;
    m_pressure_2 = ((raw_value_2 - m_offset_value_2) / 1000) * 2.2;
    m_pressure_3 = ((raw_value_3 - m_offset_value_3) / 1000) * 2.2;
    m_pressure_4 = ((raw_value_4 - m_offset_value_4) / 1000) * 2.2;

    if (m_pressure_1 < 0) m_pressure_1 = -m_pressure_1;
    if (m_pressure_2 < 0) m_pressure_2 = -m_pressure_2;
    if (m_pressure_3 < 0) m_pressure_3 = -m_pressure_3;
    if (m_pressure_4 < 0) m_pressure_4 = -m_pressure_4;

}

long Loadcell::fetchPressure_1()
{
  return m_pressure_1;   
}

long Loadcell::fetchPressure_2()
{
  return m_pressure_2;   
}

long Loadcell::fetchPressure_3()
{
  return m_pressure_3;   
}

long Loadcell::fetchPressure_4()
{
  return m_pressure_4;   
}

}
