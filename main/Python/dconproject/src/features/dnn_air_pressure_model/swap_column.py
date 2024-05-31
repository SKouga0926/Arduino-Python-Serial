import pandas as pd

# 使用例
input_file = "../../../data/raw/dnn_model_4/sensor_data.csv"  # 入力CSVファイルのパス

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

df_before_swap = df.copy()

# 列の値を入れ替える
df[column1], df[column2] = df[column2], df[column1]

# 入れ替える前と入れ替えた後のデータフレームを結合
df_combined = pd.concat([df_before_swap, df], ignore_index=True)

print(df_before_swap)
print(df_combined)