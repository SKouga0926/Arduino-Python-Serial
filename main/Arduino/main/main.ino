#include "space_separated_parser.hpp"
#include "communication_status_manager.hpp"
#include "bluetooth_server.hpp"
#include "air_sensor.hpp"
#include "airpump.hpp"
#include "loadcell.hpp"
#include "solenoid_valve.hpp"
#include "pid_controller.hpp"

constexpr int16_t MAX_WAIT_TIME_MS = 5000;

// create instance

Dcon::SpaceSeparatedParser parser("\n");
Dcon::CommunicationStatusManager manager("\n");
Dcon::BluetoothServer server;
Dcon::PidController pidController(500, 0, 0);

// 空気圧センサー
#define SS 10
auto airsensor_1 = Dcon::Airsensor(0);
auto airsensor_2 = Dcon::Airsensor(1);
auto airsensor_3 = Dcon::Airsensor(2);
auto airsensor_4 = Dcon::Airsensor(5);
auto airsensor_5 = Dcon::Airsensor(4);
auto airsensor_6 = Dcon::Airsensor(3);
//

// 小型ポンプ
Dcon::AirPump airpump(Dcon::Pin::AirPump::Forward, Dcon::Pin::AirPump::Reverse);
//

// ロードセル
Dcon::Loadcell loadcell(Dcon::Pin::LoadCell::In1, Dcon::Pin::LoadCell::In2, Dcon::Pin::LoadCell::In3, Dcon::Pin::LoadCell::In4);
//

// 電磁弁
Dcon::SolenoidValve solenoidvalve_1(Dcon::Pin::SolenoidValve::Out1);
Dcon::SolenoidValve solenoidvalve_2(Dcon::Pin::SolenoidValve::Out2);
Dcon::SolenoidValve solenoidvalve_3(Dcon::Pin::SolenoidValve::Out3);
Dcon::SolenoidValve solenoidvalve_4(Dcon::Pin::SolenoidValve::Out4);
Dcon::SolenoidValve solenoidvalve_5(Dcon::Pin::SolenoidValve::Out5);
Dcon::SolenoidValve solenoidvalve_6(Dcon::Pin::SolenoidValve::Out6);
Dcon::SolenoidValve solenoidvalve_7(Dcon::Pin::SolenoidValve::Out7);
//

// 各関数で使う変数
double airPumpPower = 0;
double pressure[4] = {0};
double airPressure[6] = {0}; 

double predictedAirPressure[6] = {0};

bool airPumpTurnOff = false;

double headPositionX =0;
double headPositionY = 0;

double pillowHeight = 43.0;
double pillowWidth = 57.5;
//

void setup() 
{
  Serial.begin(9600);

  // BluetoothServerの初期化
  server.init();

  // 空気圧センサー
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  SPI.begin();
  //

  // ロードセル
  pinMode((uint8_t)Dcon::Pin::LoadCell::Clk, OUTPUT); // 9

  loadcell.init();
  //

  // pidController
  pidController.init();
  //


  // 通信を確立するための処理
  while(true)
  {
    String message = Serial.readStringUntil('\n') + "\n";
    if(message == "request connection\n") break;
  }

  Serial.print("connection established\n");
}

void calculateHeadCoordinate(double pressure_[])
{

  double ratio1 = pressure_[1] / (pressure_[1] + pressure_[0]);
  double x1 = pillowWidth * ratio1;

  double ratio2 = pressure_[3] / (pressure_[3] + pressure_[2]);
  double x2 = pillowWidth * ratio2;

  double r2 = pillowWidth - x1;
  double r4 = pillowWidth - x2;
  double hypotenuse = sqrt(pow((r2 - r4), 2) + pow(pillowHeight, 2));

  double r5 = hypotenuse * (pressure_[0] + pressure_[1]) / (pressure_[0] + pressure_[1] + pressure_[2] + pressure_[3]);

  // deg Serial.println(atan2(1.73, 1) * 180 / 3.14);
  // 0.5 Serial.println(cos(3.14/3));

  if (x1 > x2)
  {
    double theta_rad = atan2(pillowHeight, r4 - r2);
    headPositionX = x2 + r5 * cos(theta_rad);
    headPositionY = r5 * sin(theta_rad);

    Serial.println(headPositionX);
    delay(100);
    Serial.println(headPositionY);
  }

  else
  {
    double theta_rad = atan2(pillowHeight, r2 - r4);
    headPositionX = x2 - r5 * cos(theta_rad);
    headPositionY = r5 * sin(theta_rad);

    Serial.println(headPositionX);
    delay(100);
    Serial.println(headPositionY);
  }
}


// --------------------------------------------------------

constexpr float points[4][2] = {{0, 43}, {57, 43}, {0, 0}, {57, 0}};

// ZMPを計算する関数
void calculateZMP(double forces[], int num_points, float &zmp_x, float &zmp_y) {

  // 合力と各点の重み付き座標の合計を初期化
  float total_force = 0;
  float weighted_sum_x = 0;
  float weighted_sum_y = 0;
  
  // 各点の座標と力を使って合力と重み付き座標の合計を計算
  for (int i = 0; i < num_points; i++) {
    total_force += forces[i];
    weighted_sum_x += points[i][0] * forces[i];
    weighted_sum_y += points[i][1] * forces[i];
  }
  
  // ZMPの座標を計算
  zmp_x = weighted_sum_x / total_force;
  zmp_y = weighted_sum_y / total_force;

  if ( 0 <= zmp_x && zmp_x <= 22 && 33.0 <= zmp_y && zmp_y <= 43)
  {
    Serial.print("Left up ZMP 1\n"); 
  }

  else if ( 22 < zmp_x && zmp_x <= 35.5 && 33.0 <= zmp_y && zmp_y <= 43)
  {
    Serial.print("Center up ZMP 2\n");
  }

  else if ( 35.5 < headPositionX && headPositionX <= 57.5 && 33.0 <= zmp_y && zmp_y <= 43)
  {
    Serial.print("Right up ZMP 3\n");
  }

  else if ( 0 <= zmp_x && zmp_x <= 22 && 0 <= zmp_y && zmp_y < 33.0)
  {
    Serial.print("Left down ZMP 4\n");
  }

  else if (22 < zmp_x && zmp_x <= 35.5 && 0 <= zmp_y && zmp_y < 33.0)
  {
    Serial.print("Center down 5 ZMP\n");
  }

  else if (35.5 < zmp_x && zmp_x <= 57.5 && 0 <= zmp_y && zmp_y < 33.0)
  {
    Serial.print("Right down 6 ZMP\n");
  }

   // ZMPの座標をシリアルモニタに出力
  Serial.print("ZMPのx座標: ");
  Serial.println(zmp_x);
  Serial.print("ZMPのy座標: ");
  Serial.println(zmp_y);
}


// --------------------------------------------------------




void loop() 
{
  // 常時受け取る
  // 改行文字を含まない，一つ前までなので最後に足す
  String message = Serial.readStringUntil('\n') + "\n";

  // 強制終了用
  if(message == "S\n")
  {
    airPumpTurnOff = true;
    airPumpPower = 0;
    airpump.motorSetPower(0);
    Serial.print("airPump set 0\n");
  }



  // info

  // info airPumpPower
  Serial.print("info airPump power " + String(airPumpPower) + "\n");

  // info pressure
  String info_pressure = "";
  info_pressure = String("info") + " loadcell pressure";

  for (int i = 0; i < 4; i++)
  {
    info_pressure = info_pressure + " " + String(pressure[i]);
  }

  info_pressure = info_pressure + "\n";
  // Serial.print(info_pressure);

  // info airPressure
  String info_airPressure = "";
  info_airPressure = String("info") + " air pressure";

  for (int i = 0; i < 6; i++)
  {
    info_airPressure = info_airPressure + " " + String(airPressure[i]);
  }

  info_airPressure = info_airPressure + "\n";
  // Serial.print(info_airPressure);

  //





  // Pythonからロードセルの値リクエストを受け取る

  if (message == "request loadcell value\n")
  {
    while (true)
    {
      loadcell.calcPressure();
      pressure[0] = loadcell.fetchPressure_1();
      pressure[1] = loadcell.fetchPressure_2();
      pressure[2] = loadcell.fetchPressure_3();
      pressure[3] = loadcell.fetchPressure_4();

      // if (pressure[0] < 20) pressure[0] = 0;
      // if (pressure[1] < 20) pressure[1] = 0;
      // if (pressure[2] < 20) pressure[2] = 0;
      // if (pressure[3] < 20) pressure[3] = 0;

      server.setLoadCellRslt(pressure);
      server.responseMessage();

      int32_t start_time = millis();

      // 5秒成功処理が来なかったら再送
      while(true)
      {
        message = Serial.readStringUntil('\n') + "\n";
        parser.setMessage(message);

        if(message == "S\n")
        {
          airPumpTurnOff = true;
          airpump.motorSetPower(0);
          Serial.print("airPump set 0\n");
          airPumpPower = 0;
        }

        if (parser.getAsStringAt(0) == "retval")
        {
          if (message == ("retval 0 id " + String(server.getId()) + "\n")) break;

          if (millis() - start_time > MAX_WAIT_TIME_MS)
          {
            server.resendMessage();
            break;
          }
        }

      }

      break;
    
    }
  
  }





  if (message == "nofitication prediction completed\n")
  {
    // Pythonに推論処理された空気圧量のリクエストを送る
    Serial.print("request predictedAirPressure value\n");

    while (true)
    {
      message = Serial.readStringUntil('\n') + "\n";
      parser.setMessage(message);

      // 強制終了用
      if(message == "S\n")
      {
        airPumpTurnOff = true;
        airpump.motorSetPower(0);
        Serial.print("airPump set 0\n");
        airPumpPower = 0;
      }

      if (parser.getAsStringAt(0) == "response")
      {
        if (manager.isSuccess(message))
        {
          Serial.print("retval 0 " + parser.getAsStringWithParams("id") + "\n");


          // 小型ポンプで空気圧量を用いて制御する処理，終了したら通知する
          predictedAirPressure[0] = parser.getAsFloatAt("value", 0);
          predictedAirPressure[1] = parser.getAsFloatAt("value", 1);
          predictedAirPressure[2] = parser.getAsFloatAt("value", 2);
          predictedAirPressure[3] = parser.getAsFloatAt("value", 3);
          predictedAirPressure[4] = parser.getAsFloatAt("value", 4);
          predictedAirPressure[5] = parser.getAsFloatAt("value", 5);

          int32_t start_time = millis();

          while (true)
          {
            airPressure[0] = airsensor_1.getAirPressure();
            airPressure[1] = airsensor_2.getAirPressure();
            airPressure[2] = airsensor_3.getAirPressure();
            airPressure[3] = airsensor_4.getAirPressure();
            airPressure[4] = airsensor_5.getAirPressure();
            airPressure[5] = airsensor_6.getAirPressure();

            double maxAirPressure = airPressure[0];
            for (int i = 1; i < 6; i++)
            {
              if (airPressure[i] > maxAirPressure)
              {
                maxAirPressure = airPressure[i];
              }
            }

            loadcell.calcPressure();
            pressure[0] = loadcell.fetchPressure_1();
            pressure[1] = loadcell.fetchPressure_2();
            pressure[2] = loadcell.fetchPressure_3();
            pressure[3] = loadcell.fetchPressure_4();
            // if (pressure[0] < 20) pressure[0] = 0;
            // if (pressure[1] < 20) pressure[1] = 0;
            // if (pressure[2] < 20) pressure[2] = 0;
            // if (pressure[3] < 20) pressure[3] = 0;

            // info airPump power
            Serial.print("info airPump power " + String(airPumpPower) + "\n");
            //

            // info pressure
            info_pressure = String("info") + " loadcell pressure";

            for (int i = 0; i < 4; i++)
            {
              info_pressure = info_pressure + " " + String(pressure[i]);
            }

            info_pressure = info_pressure + "\n";
            Serial.print(info_pressure);
            //

            // info airPressure
            String info_airPressure = "";
            info_airPressure = String("info") + " air pressure";

            for (int i = 0; i < 6; i++)
            {
              info_airPressure = info_airPressure + " " + String(airPressure[i]);
            }

            info_airPressure = info_airPressure + "\n";
            Serial.print(info_airPressure);
            //

            // info predictedAirPressure
            String info_predictedAirPressure = "";
            info_predictedAirPressure = String("info") + " predictedAir pressure";

            for (int i = 0; i < 6; i++)
            {
              info_predictedAirPressure = info_predictedAirPressure + " " + String(predictedAirPressure[i]);
            }

            info_predictedAirPressure = info_predictedAirPressure + "\n";
            Serial.print(info_predictedAirPressure);
            //

            calculateHeadCoordinate(pressure);

            // ZMPを計算
            float zmp_x, zmp_y;
            calculateZMP(pressure, 4, zmp_x, zmp_y);
            

            message = Serial.readStringUntil('\n') + "\n";
            parser.setMessage(message);

            if(message == "S\n")
            {
              airPumpTurnOff = true;
              airpump.motorSetPower(0);
              Serial.print("airPump set 0\n");
              airPumpPower = 0;
            }

            if ( 0 <= headPositionX && headPositionX <= 22 && 33.0 <= headPositionY && headPositionY <= 43)
            // if ( 0 <= headPositionX && headPositionX <= 22 && 33.0 <= headPositionY && headPositionY <= 43 && maxAirPressure == airPressure[0])
            // if ( maxAirPressure == airPressure[0])
            {
              airPumpPower = pidController.calc(predictedAirPressure[0], airPressure[0], 0.1);
              // airPumpPower = pidController.calc(1.15, airPressure[0], 0.1);

              if ( 0 <= airPumpPower)
              {
                solenoidvalve_1.openValve();
                solenoidvalve_2.closeValve();
                solenoidvalve_3.closeValve();
                solenoidvalve_4.closeValve();
                solenoidvalve_5.closeValve();
                solenoidvalve_6.closeValve();
                solenoidvalve_7.closeValve();

                if ( 1.25 < airPressure[0])
                {
                  airPumpPower = 0;
                }
              }

              else
              {
                solenoidvalve_1.openValve();
                solenoidvalve_7.openValve();
              }

              if (airPumpTurnOff == true)
              {
                airPumpPower = 0;
              }

              airpump.motorSetPower(airPumpPower);

              if (millis() - start_time > MAX_WAIT_TIME_MS)
              {              
                break;
              }

              Serial.print("Left up 1\n"); 
            }

            else if ( 22 < headPositionX && headPositionX <= 35.5 && 33.0 <= headPositionY && headPositionY <= 43)
            // else if ( 22 < headPositionX && headPositionX <= 35.5 && 33.0 <= headPositionY && headPositionY <= 43 && maxAirPressure == airPressure[1])
            // else if ( maxAirPressure == airPressure[1])
            {
              airPumpPower = pidController.calc(predictedAirPressure[1], airPressure[1], 0.1);
              // airPumpPower = pidController.calc(1.15, airPressure[1], 0.1);

              if ( 0 <= airPumpPower)
              {
                solenoidvalve_1.closeValve();
                solenoidvalve_2.openValve();
                solenoidvalve_3.closeValve();
                solenoidvalve_4.closeValve();
                solenoidvalve_5.closeValve();
                solenoidvalve_6.closeValve();
                solenoidvalve_7.closeValve();

                if ( 1.25 < airPressure[1])
                {
                  airPumpPower = 0;
                }
              }

              else
              {
                solenoidvalve_2.openValve();
                solenoidvalve_7.openValve();
              }

              if (airPumpTurnOff == true)
              {
                airPumpPower = 0;
              }

              airpump.motorSetPower(airPumpPower);

              if (millis() - start_time > MAX_WAIT_TIME_MS)
              {              
                break;
              }

              Serial.print("Center up 2\n");
            }

            else if ( 35.5 < headPositionX && headPositionX <= 57.5 && 33.0 <= headPositionY && headPositionY <= 43)
            // else if ( 35.5 < headPositionX && headPositionX <= 57.5 && 33.0 <= headPositionY && headPositionY <= 43 && maxAirPressure == airPressure[2])
            // else if ( maxAirPressure == airPressure[2])
            {
              airPumpPower = pidController.calc(predictedAirPressure[2], airPressure[2], 0.1);
              // airPumpPower = pidController.calc(1.15, airPressure[2], 0.1);

              if ( 0 <= airPumpPower)
              {
                solenoidvalve_1.closeValve();
                solenoidvalve_2.closeValve();
                solenoidvalve_3.openValve();
                solenoidvalve_4.closeValve();
                solenoidvalve_5.closeValve();
                solenoidvalve_6.closeValve();
                solenoidvalve_7.closeValve();

                if ( 1.25 < airPressure[2])
                {
                  airPumpPower = 0;
                }
              }

              else
              {
                solenoidvalve_3.openValve();
                solenoidvalve_7.openValve();
              }

              if (airPumpTurnOff == true)
              {
                airPumpPower = 0;
              }

              airpump.motorSetPower(airPumpPower);

              if (millis() - start_time > MAX_WAIT_TIME_MS)
              {              
                break;
              }

              Serial.print("Right up 3\n");
            }

            else if ( 0 <= headPositionX && headPositionX <= 22 && 0 <= headPositionY && headPositionY < 33.0)
            // else if ( 0 <= headPositionX && headPositionX <= 22 && 0 <= headPositionY && headPositionY < 33.0 && maxAirPressure == airPressure[3])
            // else if ( maxAirPressure == airPressure[3])
            {
              airPumpPower = pidController.calc(predictedAirPressure[3], airPressure[3], 0.1);
              // airPumpPower = pidController.calc(1.15, airPressure[3], 0.1);

              if ( 0 <= airPumpPower)
              {
                solenoidvalve_1.closeValve();
                solenoidvalve_2.closeValve();
                solenoidvalve_3.closeValve();
                solenoidvalve_4.openValve();
                solenoidvalve_5.closeValve();
                solenoidvalve_6.closeValve();
                solenoidvalve_7.closeValve();

                if ( 1.25 < airPressure[3])
                {
                  airPumpPower = 0;
                }
              }

              else
              {
                solenoidvalve_4.openValve();
                solenoidvalve_7.openValve();
              }

              if (airPumpTurnOff == true)
              {
                airPumpPower = 0;
              }

              airpump.motorSetPower(airPumpPower);

              if (millis() - start_time > MAX_WAIT_TIME_MS)
              {              
                break;
              }

              Serial.print("Left down 4\n");
            }

            else if (22 < headPositionX && headPositionX <= 35.5 && 0 <= headPositionY && headPositionY < 33.0)
            // else if (22 < headPositionX && headPositionX <= 35.5 && 0 <= headPositionY && headPositionY < 33.0 && maxAirPressure == airPressure[4])
            // else if ( maxAirPressure == airPressure[4])
            {
              airPumpPower = pidController.calc(predictedAirPressure[4], airPressure[4], 0.1);
              // airPumpPower = pidController.calc(1.15, airPressure[4], 0.1);

              if ( 0 <= airPumpPower)
              {
                solenoidvalve_1.closeValve();
                solenoidvalve_2.closeValve();
                solenoidvalve_3.closeValve();
                solenoidvalve_4.closeValve();
                solenoidvalve_5.openValve();
                solenoidvalve_6.closeValve();
                solenoidvalve_7.closeValve();

                if ( 1.25 < airPressure[4])
                {
                  airPumpPower = 0;
                }
              }

              else
              {
                solenoidvalve_5.openValve();
                solenoidvalve_7.openValve();
              }

              if (airPumpTurnOff == true)
              {
                airPumpPower = 0;
              }

              airpump.motorSetPower(airPumpPower);

              if (millis() - start_time > MAX_WAIT_TIME_MS)
              {              
                break;
              }

              Serial.print("Center down 5\n");
            }

            else if (35.5 < headPositionX && headPositionX <= 57.5 && 0 <= headPositionY && headPositionY < 33.0)
            // else if (35.5 < headPositionX && headPositionX <= 57.5 && 0 <= headPositionY && headPositionY < 33.0 && maxAirPressure == airPressure[5])
            // else if ( maxAirPressure == airPressure[5])
            {
              airPumpPower = pidController.calc(predictedAirPressure[5], airPressure[5], 0.1);
              // airPumpPower = pidController.calc(1.15, airPressure[5], 0.1);

              if ( 0 <= airPumpPower)
              {
                solenoidvalve_1.closeValve();
                solenoidvalve_2.closeValve();
                solenoidvalve_3.closeValve();
                solenoidvalve_4.closeValve();
                solenoidvalve_5.closeValve();
                solenoidvalve_6.openValve();
                solenoidvalve_7.closeValve();

                 if ( 1.25 < airPressure[5])
                {
                  airPumpPower = 0;
                }
              }

              else
              {
                solenoidvalve_6.openValve();
                solenoidvalve_7.openValve();
              }

              
              if (airPumpTurnOff == true)
              {
                airPumpPower = 0;
              }

              airpump.motorSetPower(airPumpPower);

              if (millis() - start_time > MAX_WAIT_TIME_MS)
              {              
                break;
              }

              Serial.print("Right down 6\n");
            }

            // if (maxAirPressure < 1.05)
            // {
            //   solenoidvalve_1.closeValve();
            //   solenoidvalve_2.closeValve();
            //   solenoidvalve_3.closeValve();
            //   solenoidvalve_4.closeValve();
            //   solenoidvalve_5.closeValve();
            //   solenoidvalve_6.closeValve();
            //   solenoidvalve_7.closeValve();

            //   airPumpPower = 0;
            // }


            // if (airPumpTurnOff == true)
            // {
            //   airPumpPower = 0;
            // }

            // airpump.motorSetPower(airPumpPower);

            // if (millis() - start_time > MAX_WAIT_TIME_MS)
            // {              
            //   break;
            // }
          }

          airpump.motorSetPower(0);

          Serial.print("nofitication air inflation completed\n");

          break;
        }

        else
        {
          Serial.print("retval -1" + parser.getAsStringWithParams("id") + "\n");
        }

      }
    }   
  }
  
}

