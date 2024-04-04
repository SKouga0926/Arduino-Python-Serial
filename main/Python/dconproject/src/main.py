# PC -> Arduino

import serial
import time
import threading
import numpy as np

from pkg import bluetooth_server
from pkg import space_separated_parser 
from pkg import dnn_model

MAX_WAIT_TIME_SEC = 5 


def inputFromTerminal():
    print("小型ポンプ停止コマンド : S")
    while True:
        user_input = input()
        if user_input == "S":
            user_input = user_input + "\n" 
            ser.write(user_input.encode("utf-8"))
            print("Python send :" + user_input)

    
inputThread = threading.Thread(target=inputFromTerminal)
inputThread.start()
    
ser = serial.Serial("COM5", 9600, timeout=0.1)

server = bluetooth_server.BluetoothServer(ser)
parser = space_separated_parser.SpaceSeparatedParser()



# Dnn使用箇所1-------------------------------------------------------------

# Dnnインスタンス作成

dnn_model = dnn_model.DnnModel((4,), 4)
dnn_model.load_model('../model/pressure_model_widgets.h5')

# -------------------------------------------------------------------------



# 通信が確立されるまで処理を待つ
while True:

    # Arduinoから受け取る
    val_byte = ser.readline()
    val_string = val_byte.decode("utf-8")
    print("Python receive :" + val_string)

    # 通信を確立するための処理
    ser.write("request connection\n".encode("utf-8"))
    print("Python send :" + "request connection\n")
    time.sleep(1)

    if(val_string == "connection established\n"):
        break


while True:

    # リクエストをPython側から送る処理
    request = "request loadcell value\n"
    ser.write(request.encode("utf-8"))
    print("Python send :" + request)

    while True:

        # Arduinoから受け取る
        val_byte = ser.readline()
        val_string = val_byte.decode("utf-8")
        print("Python receive :" + val_string)

        time.sleep(1)

        val_spilited = val_string.split()

        if(val_spilited[0] == "response"):
            if(parser.loadsMessage(val_string)):
                id = parser.get("id")
                response = f"retval 0 id {str(id[0])}\n"
                ser.write(response.encode("utf-8"))
                print("Python send :" + response)
                break
            else:
                id = parser.get("id")
                response = f"retval -1 id {str(id[0])}\n"
                ser.write(response.encode("utf-8"))
                print("Python send :" + response)



    # Dnn使用箇所2-------------------------------------------------------------
    
    ## ロードセルの値を使って推論処理
    loadcell_list = parser.get("value")
    new_data = np.array([loadcell_list])
    predicted_air_pressure = dnn_model.predict(new_data)
    server.setAirPressureRslt(predicted_air_pressure)
    ser.write("nofitication prediction completed\n".encode("utf-8"))
    print("Python send :" + "nofitication prediction completed\n")

    # -------------------------------------------------------------------------



    # リクエストがArduino側から来る時の処理
    while True:
        # Arduinoから受け取る
        val_byte = ser.readline()
        val_string = val_byte.decode("utf-8")
        print("Python receive :" + val_string)

        if(val_string == "request predictedAirPressure value\n"):
            server.responseMessage()

            start_time = time.time()

            # 5秒成功処理が来なかったら再送
            while True:

                val_byte = ser.readline()
                val_string = val_byte.decode("utf-8")
                print("Python receive :" + val_string)

                val_spilited = val_string.split()

                if len(val_spilited) > 0 and val_spilited[0] == "retval":

                    if val_string == f"retval 0 id {server.getId()}\n":
                        break

                    if time.time() - start_time > MAX_WAIT_TIME_SEC:
                        print("time out : resend\n")
                        server.resendMessage()
                        break
            
            break



    # 小型ポンプの終了通知が来るまで処理を中断する
    while True:
        # Arduinoから受け取る
        val_byte = ser.readline()
        val_string = val_byte.decode("utf-8")
        print("Python receive :" + val_string)

        if (val_string == "nofitication air inflation completed\n"):
            break

