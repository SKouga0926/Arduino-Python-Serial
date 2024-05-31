import pandas as pd
import numpy as np

# CSVファイルのパス
original_csv_file_path = '../../../data/raw/dnn_model_6/sensor_data_valid.csv'

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

    # 処理をすべての行に対して行ったらループを抜ける
    if original_data.index.max() == len(original_data) - 1:
        break

# 加えたノイズを含むデータを新しいCSVファイルに保存する
original_data.to_csv('../../../data/processed/dnn_model_6/sensor_data_valid.csv', index=False)
