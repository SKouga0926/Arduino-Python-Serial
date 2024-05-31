# PC -> Arduino

# データ収集用のプログラム
# Arduinoのdatacollectと合わせて使う

import serial
import time
import threading
import re
import csv
import pandas as pd

ser = serial.Serial("COM6", 9600, timeout=0.1) 

id = 0
max_id = 100

def incrementId():
    global id 
    id = (id % max_id) + 1
    return id

def appendId(message_):
    message = f"{message_} id {id}"
    return message

def appendDelimiter(message_, delimiter="\n"):
    message = message_ + delimiter
    return message

def extractNumberAsString(message_, target_):
    pattern = rf'{target_}\s+((?:[-+]?\d*\.?\d+|\d+)(?:\s+(?:[-+]?\d*\.?\d+|\d+))*)'
    match = re.search(pattern, message_)

    if match:
        stringNumber = match.group(1).split()
        return stringNumber
    else:
        return "Not Found"
    
def convertListToFloat(stringList_):
    return [float(val) for val in stringList_]

def convertListToInt(stringList_):
    return [int(val) for val in stringList_]

# 100 0
def inputFromTerminal():
    while True:
        user_input = input()

        if user_input == "S":
            message_with_delimiter = appendDelimiter(user_input)
            ser.write(message_with_delimiter.encode("utf-8"))
            print("Python send :" + message_with_delimiter)

        else:
            user_input_splited = user_input.split()
            message = f"airPump {user_input_splited[0]} valve {user_input_splited[1]} place {user_input_splited[2]}"
            incrementId()
            message_with_id = appendId(message)
            message_with_delimiter = appendDelimiter(message_with_id) 
            ser.write(message_with_delimiter.encode("utf-8"))
            print("Python send :" + message_with_delimiter)


    
inputThread = threading.Thread(target=inputFromTerminal)
inputThread.start()
    



def main():

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


    

    # 項目
    # header = ["Pressure1", "Pressure2", "Pressure3", "Pressure4", "AirPressure1", "AirPressure2", "AirPressure3", "AirPressure4", "AirPressure5", "AirPressure6"]

    # # CSVファイルに書き込み（初回）
    # with open('../../data/raw/sensor_data.csv', 'w', newline='') as csv_file:
    #     csv_writer = csv.writer(csv_file)
        
    #     # 項目を書き込み
    #     csv_writer.writerow(header)

    
    while True:

        # Arduinoから受け取る
        val_byte = ser.readline()
        val_string = val_byte.decode("utf-8")
        print("Python receive :" + val_string)


        if val_string.startswith("pressure"):
            pressure_list = convertListToFloat(extractNumberAsString(val_string, "pressure"))

            airPressure_list = convertListToFloat(extractNumberAsString(val_string, "airPressure"))

            data_set = pressure_list + airPressure_list

            # CSVファイルに新しいデータを追加
            with open('../../data/raw/sensor_data.csv', 'a', newline='') as csv_file:
                csv_writer = csv.writer(csv_file)
                csv_writer.writerow(data_set)

            # 読み込んで表示
            data = pd.read_csv("../../data/raw/sensor_data.csv")
            print(data.head())

            X = data[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
            Y = data[["AirPressure1", "AirPressure2", "AirPressure3", "AirPressure4", "AirPressure5", "AirPressure6"]]


            
if __name__=="__main__":
    main()

# power 0 or 1 place



