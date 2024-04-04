#ifndef INCLUDE_DLIB_PIN_DEFINITON_HPP
#define INCLUDE_DLIB_PIN_DEFINITON_HPP
#include "Arduino.h"
#include <stdint.h>

namespace Pin {

    enum class SolenoidValve : uint8_t {
      Out1 = 8,
      Out2 = 7,
      Out3 = 4,
      Out4 = 3,
      Out5 = 2,
      Out6 = A7,
      Out7 = A6,
    };

    enum class AirPump : uint8_t {
      Forward = 6,
      Reverse = 5,
    };

    enum class LoadCell : uint8_t {
      In1 = A2,
      In2 = A3,
      In3 = A1,
      In4 = A0,
      Clk = 9,
    };

}
#endif // INCLUDE_DLIB_PIN_DEFINITON_HPP