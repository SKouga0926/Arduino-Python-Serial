#ifndef INCLUDE_DLIB_LOADCELL_HPP
#define INCLUDE_DLIB_LOADCELL_HPP

#include "pin_definition.hpp"

namespace Dcon
{


class Loadcell
{
public:
    Loadcell(Pin::LoadCell pin_in_1, Pin::LoadCell pin_in_2, Pin::LoadCell pin_in_3, Pin::LoadCell pin_in_4);
    void init();

    void calcPressure();

    long fetchPressure_1();
    long fetchPressure_2();
    long fetchPressure_3();
    long fetchPressure_4();

private:
    const uint8_t m_pin_in_1;
    const uint8_t m_pin_in_2;
    const uint8_t m_pin_in_3;
    const uint8_t m_pin_in_4;

    long m_offset_value_1;
    long m_offset_value_2;
    long m_offset_value_3;
    long m_offset_value_4;

    long m_pressure_1;
    long m_pressure_2;
    long m_pressure_3;
    long m_pressure_4;
};

}

#endif // INCLUDE_DLIB_LOADCELL_HPP