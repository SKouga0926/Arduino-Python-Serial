import pandas as pd
import numpy as np

# CSVファイルのパス
original_csv_file_path = '../../../data/raw/dnn_model_4/sensor_data_for_before_model.csv'

# CSVファイルを読み込む
original_data = pd.read_csv(original_csv_file_path)

while True:
    # Pressure1, Pressure2, Pressure3, Pressure4の列に対して-3.0あるいは3.0のノイズを加える
    pressure_columns = ['Pressure1', 'Pressure2', 'Pressure3', 'Pressure4']
    selected_pressure_column = np.random.choice(pressure_columns)
    print(selected_pressure_column)
    noise = np.random.choice([-3.0, 3.0], size=len(original_data))
    original_data[selected_pressure_column] = np.round(original_data[selected_pressure_column] + noise, decimals=2)

    # Pressure1, Pressure2, Pressure3, Pressure4の列に対して-3.0あるいは3.0のノイズを加える
    pressure_columns = ['Pressure1', 'Pressure2', 'Pressure3', 'Pressure4']
    selected_pressure_column2 = np.random.choice(pressure_columns)
    if selected_pressure_column != selected_pressure_column2:
        print(selected_pressure_column2)
        noise = np.random.choice([-3.0, 3.0], size=len(original_data))
        original_data[selected_pressure_column2] = np.round(original_data[selected_pressure_column2] + noise, decimals=2)




    # AirPressure1, AirPressure2, AirPressure3, AirPressure4の列に対して-0.01か0.01のノイズを加える
    air_pressure_columns = ['AirPressure1', 'AirPressure2', 'AirPressure3', 'AirPressure4']
    selected_air_pressure_column = np.random.choice(air_pressure_columns)
    print(selected_air_pressure_column)
    noise = np.random.choice([-0.01, 0.01], size=len(original_data))
    original_data[selected_air_pressure_column] = np.round(original_data[selected_air_pressure_column] + noise, decimals=2)

    # AirPressure1, AirPressure2, AirPressure3, AirPressure4の列に対して-0.01か0.01のノイズを加える
    air_pressure_columns = ['AirPressure1', 'AirPressure2', 'AirPressure3', 'AirPressure4']
    selected_air_pressure_column2 = np.random.choice(air_pressure_columns)
    if selected_air_pressure_column != selected_air_pressure_column2:
        print(selected_air_pressure_column2)
        noise = np.random.choice([-0.01, 0.01], size=len(original_data))
        original_data[selected_air_pressure_column2] = np.round(original_data[selected_air_pressure_column2] + noise, decimals=2)

    # 処理をすべての行に対して行ったらループを抜ける
    if original_data.index.max() == len(original_data) - 1:
        break

# 加えたノイズを含むデータを新しいCSVファイルに保存する
original_data.to_csv('../../../data/interim/dnn_model_4/sensor_data_for_before_model.csv', index=False)
