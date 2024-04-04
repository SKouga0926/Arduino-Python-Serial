#include "communication_status_manager.hpp"

namespace Dcon
{

namespace Manager
{

CommunicationStatusManager::CommunicationStatusManager(const char* delimiter_) : m_parser(delimiter_)
{

}

void CommunicationStatusManager::init()
{

}

bool CommunicationStatusManager::isSuccess(const String &message_)
{
  m_parser.setMessage(message_);

  if(m_parser.getAsStringAt(0) == "response")
  {
    if(m_parser.getAsStringAt(1) == "airPressure")
    {
      int16_t numberOfArgs = m_parser.getNumberOfArgs("value");
      if(numberOfArgs == -1) return false; 

      if(numberOfArgs == NUM_OF_ARGS_AIR_PRESSURE)
      {
        float airPressure[numberOfArgs];

        for (int i = 0; i < numberOfArgs; i++)
        {
          airPressure[i] = m_parser.getAsFloatAt("value", i);

          if(airPressure[i] != -1)
          {
            
            return true;
          } 

          else
          {
            return false;
          }

        }
      }

      else
      {
        return false;
      }
    }

    else
    {
      return false;
    }

  }

  else
  {
    return false;
  }
}

}

}