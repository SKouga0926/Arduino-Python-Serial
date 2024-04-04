class BluetoothServer:

# public:

    def __init__(self, ser_):
        self.__m_message = ""

        self.__m_id = 0
        self.__m_max_id = 1000

        self.__m_airPressure = []

        self.__m_ser = ser_

    def setAirPressureRslt(self, rslt_):
        self.__m_airPressure = rslt_

    def responseMessage(self):

        self.__m_message = f"response predictedAirPressure value {' '.join(map(str, self.__m_airPressure[0]))}"
        self.__incrementId()
        self.__appendId()
        self.__appendDelimiter("\n")

        self.__m_ser.write(self.__m_message.encode("utf-8"))
        print("Python send :" + self.__m_message)

    def resendMessage(self):

        self.__m_message = f"response predictedAirPressure value {' '.join(map(str, self.__m_airPressure))}"
        self.__appendId()
        self.__appendDelimiter("\n")

        self.__m_ser.write(self.__m_message.encode("utf-8"))
        print("Python send :" + self.__m_message)

    def getId(self):
        return self.__m_id

# private:
    
    def __incrementId(self):
        self.__m_id = (self.__m_id % self.__m_max_id) + 1
    
    def __appendId(self):
        self.__m_message = f"{self.__m_message} id {self.__m_id}"
    
    def __appendDelimiter(self, delimiter="\n"):
        self.__m_message = self.__m_message + delimiter