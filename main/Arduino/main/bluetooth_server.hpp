#ifndef INCLUDE_DLIB_BLUETOOTH_SERVER_HPP
#define INCLUDE_DLIB_BLUETOOTH_SERVER_HPP

#include <Arduino.h>

namespace Dcon
{

constexpr int16_t LOAD_CELL_ARRAY_SIZE = 4;

class BluetoothServer
{

public:

  BluetoothServer();
  void init();

  void setLoadCellRslt(const double rslt_[]);
  void responseMessage();
  void resendMessage();

  int16_t getId();

private:

  void incrementId();
  void appendId();
  void appendDelimiter(const char* delimiter_);

private:

  String m_message;

  int16_t m_id;
  int16_t m_max_id;

  double m_loadcell[LOAD_CELL_ARRAY_SIZE];

};


}

#endif // INCLUDE_DLIB_BLUETOOTH_SERVER_HPP