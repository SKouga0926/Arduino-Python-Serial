#include "space_separated_parser.hpp"
#include "air_sensor.hpp"
#include "airpump.hpp"
#include "loadcell.hpp"
#include "solenoid_valve.hpp"

// create instance

Dcon::Parser::SpaceSeparatedParser parser("\n");

// 空気圧センサー
#define SS 10
auto airsensor_1 = Airsensor(0);
auto airsensor_2 = Airsensor(1);
auto airsensor_3 = Airsensor(2);
auto airsensor_4 = Airsensor(5);
auto airsensor_5 = Airsensor(4);
auto airsensor_6 = Airsensor(3);
//

// 小型ポンプ
AirPump airpump(Pin::AirPump::Forward, Pin::AirPump::Reverse);
//

// ロードセル
Loadcell loadcell(Pin::LoadCell::In1, Pin::LoadCell::In2, Pin::LoadCell::In3, Pin::LoadCell::In4);
//

// 電磁弁
SolenoidValve solenoidvalve_1(Pin::SolenoidValve::Out1);
SolenoidValve solenoidvalve_2(Pin::SolenoidValve::Out2);
SolenoidValve solenoidvalve_3(Pin::SolenoidValve::Out3);
SolenoidValve solenoidvalve_4(Pin::SolenoidValve::Out4);
SolenoidValve solenoidvalve_5(Pin::SolenoidValve::Out5);
SolenoidValve solenoidvalve_6(Pin::SolenoidValve::Out6);
SolenoidValve solenoidvalve_7(Pin::SolenoidValve::Out7);
//

void setup() 
{
  Serial.begin(9600);

  // 空気圧センサー
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  SPI.begin();
  //

  // ロードセル
  pinMode((uint8_t)Pin::LoadCell::Clk, OUTPUT); // 9

  loadcell.init();
  //

  // 通信を確立するための処理
  while(true)
  {
    String message = Serial.readStringUntil('\n') + "\n";
    if(message == "request connection\n") break;
  }

  Serial.print("connection established\n");
  //
  
}

float airPumpPower = 0;
int16_t isSolenoidvalveOpen = 0;
int16_t airBagPlace = 0;
int16_t id = 0;

long pressure[4] = {0};
float airPressure[6] = {0};

unsigned long start_time = 0;

void loop() 
{
  // 常時受け取る
  // 改行文字を含まない，一つ前までなので最後に足す
  String message = Serial.readStringUntil('\n') + "\n";
  parser.setMessage(message);

  float parsedAirPumpPower = parser.getAsFloatAt("airPump", 0);
  int16_t parsedIsSolenoidvalveOpen = parser.getAsIntAt("valve", 0);
  int16_t parsedAirBagPlace = parser.getAsIntAt("place", 0);
  int16_t parsedId = parser.getAsIntAt("id", 0);
  

  if (parser.getAsStringAt(0) == "airPump")
  {
    if (parsedAirPumpPower != -1 && parsedIsSolenoidvalveOpen != -1 && parsedAirBagPlace != -1 && parsedId != -1)
    {
      if (abs(parsedAirPumpPower) < 101 && parsedAirBagPlace != 0 && abs(parsedAirBagPlace) < 8 && abs(parsedId) < 101)
      {
        if (parsedIsSolenoidvalveOpen == 0 || parsedIsSolenoidvalveOpen == 1)
        {
          airPumpPower = parsedAirPumpPower;
          isSolenoidvalveOpen = parsedIsSolenoidvalveOpen;
          airBagPlace = parsedAirBagPlace;
          id = parsedId;

          Serial.print("retval 0 " + parser.getAsStringWithParams("id") + "\n");

          start_time = millis();

        }

        else
        {
          Serial.print("retval -1 " + parser.getAsStringWithParams("id") + "\n");  
        }
      }

      else
      {
        Serial.print("retval -1 " + parser.getAsStringWithParams("id") + "\n");  
      }

    }

    else 
    {
      Serial.print("retval -1 " + parser.getAsStringWithParams("id") + "\n");
    }
  }

  

  loadcell.calcPressure();
  pressure[0] = loadcell.fetchPressure_1();
  pressure[1] = loadcell.fetchPressure_2();
  pressure[2] = loadcell.fetchPressure_3();
  pressure[3] = loadcell.fetchPressure_4();

  airPressure[0] = airsensor_1.getAirPressure();
  airPressure[1] = airsensor_2.getAirPressure();
  airPressure[2] = airsensor_3.getAirPressure();
  airPressure[3] = airsensor_4.getAirPressure();
  airPressure[4] = airsensor_5.getAirPressure();
  airPressure[5] = airsensor_6.getAirPressure();


  if (message == "S\n")
  {  

    String temp = "";

    temp = "pressure";

    for (int i = 0; i < 4; i++)
    {
      temp = temp + " " + String(pressure[i]);
    }

    temp = temp + " " + "airPressure";

    for (int i = 0; i < 6; i++)
    {
      temp = temp + " " + String(airPressure[i]);
    }

    temp = temp + "\n";

    Serial.print(temp);
   
  }






  if (millis() - start_time > 3000)
  {
    airPumpPower = 0;
  }

  // 小型ポンプ
  airpump.motorSetPower(airPumpPower);
  //


  // 閉める処理
  if (isSolenoidvalveOpen == 0)
  {
    switch(airBagPlace)
    {
      case 1:
        solenoidvalve_1.closeValve();
        break;

      case 2:
        solenoidvalve_2.closeValve();
        break;

      case 3:
        solenoidvalve_3.closeValve();
        break;

      case 4:
        solenoidvalve_4.closeValve();
        break;

      case 5:
        solenoidvalve_5.closeValve();
        break;

      case 6:
        solenoidvalve_6.closeValve();
        break;

      case 7:
        solenoidvalve_7.closeValve();


      default:
        break;
    }
  }

  // 開く処理
  if (isSolenoidvalveOpen == 1)
  {
    switch(airBagPlace)
    {
      case 1:
        solenoidvalve_1.openValve();
        break;

      case 2:
        solenoidvalve_2.openValve();
        break;

      case 3:
        solenoidvalve_3.openValve();
        break;

      case 4:
        solenoidvalve_4.openValve();
        break;

      case 5:
        solenoidvalve_5.openValve();
        break;

      case 6:
        solenoidvalve_6.openValve();
        break;

      case 7:
        solenoidvalve_7.openValve();

      default:
        break;
    }

  }


  // info
  String info_airPump = "";
  info_airPump = String("info") + " airPump power" + " " + String(airPumpPower) + "\n";
  Serial.print(info_airPump);

  String info_pressure = "";
  info_pressure = String("info") + " loadcell pressure";

  for (int i = 0; i < 4; i++)
  {
    info_pressure = info_pressure + " " + String(pressure[i]);
  }

  info_pressure = info_pressure + "\n";
  Serial.print(info_pressure);

  String info_airPressure = "";
  info_airPressure = String("info") + " air pressure";

  for (int i = 0; i < 6; i++)
  {
    info_airPressure = info_airPressure + " " + String(airPressure[i]);
  }

  info_airPressure = info_airPressure + "\n";
  Serial.print(info_airPressure);
  //

}
