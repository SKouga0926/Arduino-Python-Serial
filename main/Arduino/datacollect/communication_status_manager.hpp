#ifndef INCLUDE_DLIB_COMMUNICATION_STATUS_MANAGER_HPP
#define INCLUDE_DLIB_COMMUNICATION_STATUS_MANAGER_HPP

#include <Arduino.h>

#include "space_separated_parser.hpp"

constexpr int16_t NUM_OF_ARGS_AIR_PRESSURE = 4;
constexpr int16_t NUM_OF_ARGS_ID = 1; 

namespace Dcon
{

namespace Manager
{

class CommunicationStatusManager
{
public:
  CommunicationStatusManager(const char* delimiter_);
  void init();

  bool isSuccess(const String &message_);

private:

private:
  Dcon::Parser::SpaceSeparatedParser m_parser;
  
};

}

}

#endif // INCLUDE_DLIB_COMMUNICATION_STATUS_MANAGER_HPP