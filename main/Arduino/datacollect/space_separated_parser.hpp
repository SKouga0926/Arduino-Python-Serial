#ifndef INCLUDE_DLIB_SPACE_SEPARATED_PARSER_HPP
#define INCLUDE_DLIB_SPACE_SEPARATED_PARSER_HPP

#include <Arduino.h>

namespace Dcon
{

namespace Parser
{

class SpaceSeparatedParser
{
public:

  SpaceSeparatedParser(const char* delimiter_);
  void init();

  inline void setMessage(const String &message_) { m_message = message_; }

  float getAsFloatAt(const String &targetString_, const int16_t taregetNumber_) const;
  int16_t getAsIntAt(const String &targetString_, const int16_t taregetNumber_) const;
  String getAsStringWithParams(const String &targetString_) const;
  String getAsStringAt(const int16_t targetNumber_) const;
  int16_t getNumberOfString() const;
  int16_t getNumberOfArgs(const String &targetString_) const;

  bool isContainsString(const String &message_, const String &targetString_) const;
private:
  
  String getAsStringAt(const String &message_, const int16_t targetNumber_) const;
  int16_t getNumberOfString(const String &message_) const;
  

private:
  String m_message;
  const char* m_delimiter;

};

}

}

#endif // INCLUDE_DLIB_SPACE_SEPARATED_PARSER_HPP