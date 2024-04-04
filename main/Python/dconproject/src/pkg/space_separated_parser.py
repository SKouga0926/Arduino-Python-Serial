import re

class SpaceSeparatedParser:

# public:
    
    def __init__(self):
        self.__m_message = ""
        self.__m_valueLength = 4
        self.__m_idLength = 1

        self.__m_isFormatted = False

        self.__m_valueFloatList = []
        self.__m_idFloatList = []

    def loadsMessage(self, message_):
        self.__m_message = message_
        self.__m_isFormatted = self.__checkFormat("value", "id")

        if(self.__m_isFormatted == True):
            return True
        else:
            return False

    def get(self, target_):
        if(self.__m_isFormatted == True):
            if(target_ == "value"):
                return self.__m_valueFloatList
            elif(target_ == "id"):
                return self.__m_idFloatList

    
# private:
        
    def __extractNumberAsString(self, target_):
        pattern = rf'{target_}\s+((?:[-+]?\d*\.?\d+|\d+)(?:\s+(?:[-+]?\d*\.?\d+|\d+))*)'
        match = re.search(pattern, self.__m_message)

        if match:
            stringNumber = match.group(1).split()
            return stringNumber
        else:
            return "Not Found"
        
    def __convertListToFloat(self, stringList_):
        return [float(val) for val in stringList_]
    
    def __convertListToInt(self, stringList_):
        return [int(val) for val in stringList_]
    
    def __checkFormat(self, *target_):

        isValueFormatted = False
        isIdFormatted = False

        for target in target_:
            stringValueList = self.__extractNumberAsString(target)
            if(stringValueList != "Not Found"):
                if(len(stringValueList) == self.__m_valueLength):
                    self.__m_valueFloatList = self.__convertListToFloat(stringValueList)
                    isValueFormatted = True
                    # print(f"value: {floatValueList}")

                elif(len(stringValueList) == self.__m_idLength):
                    self.__m_idFloatList = self.__convertListToInt(stringValueList)
                    isIdFormatted = True
                    # print(f"id: {floatValueList}")

        if(isValueFormatted == True and isIdFormatted == True):
           return True
        else:
           return False

        

if __name__ == "__main__":


    # Example

    inputString = "value -1  -20.0  0 1 id 1\n"

    Server = SpaceSeparatedParser()

    Server.loadsMessage(inputString)
    print(Server.get("value"))
    print(Server.get("id"))

    # print(Server.getValueList("value"))
    # print(Server.getLength("value"))

