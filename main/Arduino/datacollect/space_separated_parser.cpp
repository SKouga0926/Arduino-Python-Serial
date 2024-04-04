#include "space_separated_parser.hpp"

namespace Dcon
{

namespace Parser
{

SpaceSeparatedParser::SpaceSeparatedParser(const char* delimiter_) : m_message(" "), m_delimiter(delimiter_)
{
  
}

void SpaceSeparatedParser::init()
{

}

float SpaceSeparatedParser::getAsFloatAt(const String &targetString_, const int16_t targetNumber_) const
{
  if (isContainsString(m_message, targetString_) == false) return -1;

  String splitedMessage = getAsStringWithParams(targetString_);
  int16_t numberOfString = getNumberOfString(splitedMessage) - 1;

  if ((0 > targetNumber_) || (targetNumber_ >= numberOfString))
  {
    return -1;
  }

  float number = getAsStringAt(splitedMessage,  targetNumber_ + 1).toFloat();

  if(number == 0)
  {
    if(getAsStringAt(splitedMessage,  targetNumber_ + 1) == "0")
    {
      return number;
    }

    else
    {
      return -1;
    }
  }

  return number;
}

int16_t SpaceSeparatedParser::getAsIntAt(const String &targetString_, const int16_t targetNumber_) const
{
  if (isContainsString(m_message, targetString_) == false) return -1;

  String splitedMessage = getAsStringWithParams(targetString_);
  int16_t numberOfString = getNumberOfString(splitedMessage) - 1;

  if ((0 > targetNumber_) || (targetNumber_ >= numberOfString))
  {
    return -1;
  }

  int16_t number = getAsStringAt(splitedMessage,  targetNumber_ + 1).toInt();

  if(number == 0)
  {
    if(getAsStringAt(splitedMessage,  targetNumber_ + 1) == "0")
    {
      return number;
    }

    else
    {
      return -1;
    }
  }

  return number;
}

String SpaceSeparatedParser::getAsStringWithParams(const String &targetString_) const
{
  if (isContainsString(m_message, targetString_) == false) return "Not Found";

  int16_t targetIndex = m_message.indexOf(targetString_);
  int16_t targetSize = targetString_.length();

  String remainingOriginal = m_message.substring(targetIndex + targetSize);
  
  String numericString = "";

  for (char tempChar : remainingOriginal) {
    if (tempChar == ' ' || (tempChar >= '0' && tempChar <= '9') || tempChar == '-') 
    {
      numericString += tempChar;
    } 

    else 
    {
      break;
    }
  }
  String targetStringWithParams = targetString_ + numericString;

  targetStringWithParams.trim();

  return targetStringWithParams;
}

String SpaceSeparatedParser::getAsStringAt(const int16_t targetNumber_) const
{
  int16_t numberOfString = getNumberOfString(m_message);

  if ((0 > targetNumber_) || (targetNumber_ >= numberOfString))
  {
    return "Not Found";
  }

  String currentWord = "";
  
  int16_t spaceCount = 0;
  
  for (int16_t i = 0; i < m_message.length(); i++) 
  {
    char currentChar = m_message.charAt(i);

    if (currentChar == *m_delimiter) 
    {
      return currentWord;
    }

    else if (currentChar == ' ') 
    {
    
      while (i + 1 < m_message.length() && m_message.charAt(i + 1) == ' ') 
      {
        i++;
      }

      if (spaceCount == targetNumber_)
      {
        return currentWord;
      }

      spaceCount++;

      currentWord = "";
    } 
    
    else 
    {
      currentWord += currentChar;
    }
  }
}

String SpaceSeparatedParser::getAsStringAt(const String &message_, const int16_t targetNumber_) const
{
  int16_t numberOfString = getNumberOfString(message_);

  if((0 > targetNumber_) || (targetNumber_ >= numberOfString))
  {
    return "Not Found";
  }

  String currentWord = "";
  
  int16_t spaceCount = 0;
  
  for (int16_t i = 0; i <= message_.length(); i++) 
  {
    char currentChar = message_.charAt(i);

    if(currentChar == '\0') 
    {
      return currentWord;
    }

    else if (currentChar == ' ') 
    {
    
      while (i + 1 < message_.length() && message_.charAt(i + 1) == ' ') 
      {
        i++;
      }

      if (spaceCount == targetNumber_) 
      {
        return currentWord;
      }

      spaceCount++;

      currentWord = "";
    } 
    
    else 
    {  
      currentWord += currentChar;
    }
  }
}

int16_t SpaceSeparatedParser::getNumberOfString() const
{
  String currentWord = "";
  
  int16_t spaceCount = 0;
  
  for (int16_t i = 0; i < m_message.length(); i++) 
  {
    char currentChar = m_message.charAt(i);

    if (currentChar == ' ')
    {
      
      spaceCount++;
      
      while (i + 1 < m_message.length() && m_message.charAt(i + 1) == ' ') 
      {
        i++;
      }

      currentWord = "";
    } 
    
    else 
    {
      currentWord += currentChar;
    }
  }

  return spaceCount + 1;
}

int16_t SpaceSeparatedParser::getNumberOfString(const String &message_) const
{
  String currentWord = "";
  
  int16_t spaceCount = 0;
  
  for (int16_t i = 0; i < message_.length(); i++) 
  {
    char currentChar = message_.charAt(i);

    if (currentChar == ' ') 
    {
      
      spaceCount++;
      
      while (i + 1 < message_.length() && message_.charAt(i + 1) == ' ') 
      {
        i++;
      }

      currentWord = "";

    } 
    
    else 
    {
      currentWord += currentChar;
    }
  }

  return spaceCount + 1;
}

int16_t SpaceSeparatedParser::getNumberOfArgs(const String &targetString_) const
{
  if (isContainsString(m_message, targetString_) == false) return -1;

  String splitedMessage = getAsStringWithParams(targetString_);
  int16_t numberOfArgs = getNumberOfString(splitedMessage) - 1;

  return numberOfArgs;
}

bool SpaceSeparatedParser::isContainsString(const String &message_, const String &targetString_) const
{

  int16_t numberOfString = getNumberOfString(message_);
  
  for (int i = 0; i < numberOfString; i++)
  {
    if (getAsStringAt(message_, i) == targetString_)
    {
      return true;
    }
  }

  return false;
}

}

}