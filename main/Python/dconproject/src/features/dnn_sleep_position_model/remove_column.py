import pandas as pd

def remove_columns(input_file, output_file, columns_to_remove):
    # CSVファイルを読み込む
    df = pd.read_csv(input_file)
    
    # 指定された列を削除する
    df.drop(columns_to_remove, axis=1, inplace=True)
    
    # 新しいCSVファイルとして保存する
    df.to_csv(output_file, index=False)

# 使用例
input_file = "../../../data/raw/sensor_data.csv"  # 入力CSVファイルのパス
output_file = "../../../data/raw/dnn_model_6/sensor_data.csv"  # 出力CSVファイルのパス
columns_to_remove = ["AirPressure1", "AirPressure2", "AirPressure3", "AirPressure4", "AirPressure5", "AirPressure6", "HeadLocation"]  # 削除する列の名前

remove_columns(input_file, output_file, columns_to_remove)
