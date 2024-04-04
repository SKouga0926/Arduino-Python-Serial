#include "bluetooth_server.hpp"

namespace Dcon
{

BluetoothServer::BluetoothServer()
{

}

void BluetoothServer::init()
{
  m_message = " ";

  m_id = 0;
  m_max_id = 1000;

  m_loadcell[LOAD_CELL_ARRAY_SIZE] = {0};

}

void BluetoothServer::setLoadCellRslt(const double rslt_[])
{
  for (int i = 0; i < LOAD_CELL_ARRAY_SIZE; i++)
  {
    m_loadcell[i] = rslt_[i];
  }
}

void BluetoothServer::responseMessage()
{
  m_message = "response loadcell value";

  for (int i = 0; i < LOAD_CELL_ARRAY_SIZE; i++)
  {
    m_message = m_message + " " + String(m_loadcell[i]);
  }

  incrementId();
  appendId();
  appendDelimiter("\n");
  
  Serial.print(m_message);
}

void BluetoothServer::resendMessage()
{
  m_message = "response loadcell value";

  for (int i = 0; i < LOAD_CELL_ARRAY_SIZE; i++)
  {
    m_message = m_message + " " + String(m_loadcell[i]);
  }

  appendId();
  appendDelimiter("\n");
  
  Serial.print(m_message);
}

int16_t BluetoothServer::getId()
{
  return m_id;
}

void BluetoothServer::incrementId()
{
  m_id = (m_id % m_max_id) + 1;
}

void BluetoothServer::appendId()
{
  m_message = m_message + " id " + String(m_id); 
}

void BluetoothServer::appendDelimiter(const char* delimiter)
{
  m_message = m_message + *delimiter;
}


}