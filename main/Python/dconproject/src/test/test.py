import pandas as pd
import numpy as np

# データの読み込み
data = pd.read_csv('../../data/raw/dnn_model_4/sensor_data.csv')

# 各行ごとに平均値を計算してノイズを加える
noisy_data = data.copy()
for i, row in data.iterrows():
    mean_val = row[['Pressure1', 'Pressure2', 'Pressure3', 'Pressure4']].mean()
    noise = np.random.normal(0, mean_val, size=4) * 0.10
    noisy_data.loc[i, ['Pressure1', 'Pressure2', 'Pressure3', 'Pressure4']] += noise
    noisy_data.loc[i, ['Pressure1', 'Pressure2', 'Pressure3', 'Pressure4']] = np.around(noisy_data.loc[i, ['Pressure1', 'Pressure2', 'Pressure3', 'Pressure4']], decimals=1)

    print(i)
    print(row)
    print(mean_val)
    print(noise)
    print(noisy_data)

# combined_data = pd.concat([data, noisy_data], ignore_index=True)
# print(combined_data)

# combined_data.to_csv('../../data/processed/dnn_model_4/sensor_data.csv', index=False, mode='a', header=False)
noisy_data.to_csv('../../data/processed/dnn_model_4/sensor_data.csv', index=False, mode='a', header=False)

# 結果を表示
# print(noisy_data)
# 578