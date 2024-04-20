import pandas as pd

def swap_columns(input_file, output_file, column1, column2):
    # CSVファイルを読み込む
    df = pd.read_csv(input_file)
    
    # 列の値を入れ替える
    df[column1], df[column2] = df[column2], df[column1]
    
    # 新しいCSVファイルとして保存する
    df.to_csv(output_file, index=False)

# 使用例
input_file = "../../data/raw/sensor_data_for_before_model_valid.csv"  # 入力CSVファイルのパス
output_file = "../../data/processed/sensor_data_for_before_model_valid.csv"  # 出力CSVファイルのパス

column1 = "Pressure1"  # 入れ替える列1
column2 = "Pressure2"  # 入れ替える列2

column3 = "Pressure3"  # 入れ替える列3
column4 = "Pressure4"  # 入れ替える列4

column5 = "AirPressure1"  # 入れ替える列5
column6 = "AirPressure3"  # 入れ替える列6

column7 = "AirPressure4"  # 入れ替える列5
column8 = "AirPressure6"  # 入れ替える列6

# CSVファイルを読み込む
df = pd.read_csv(input_file)

# 列の値を入れ替える
df[column1], df[column2] = df[column2], df[column1]

# 列の値を入れ替える
df[column3], df[column4] = df[column4], df[column3]

# 列の値を入れ替える
df[column5], df[column6] = df[column6], df[column5]

# 列の値を入れ替える
df[column7], df[column8] = df[column8], df[column7]

# 新しいCSVファイルとして保存する
df.to_csv(output_file, index=False)
