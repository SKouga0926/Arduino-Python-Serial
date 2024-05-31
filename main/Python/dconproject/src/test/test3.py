import pandas as pd
import numpy as np

# 元のデータフレームを作成
df = pd.DataFrame({'a': [10], 'b': [20], 'c': [30], 'd': [40]})

# ノイズのパラメータ
noise_mean = 0
noise_std = 5

# 新しいデータを加える回数
num_new_data = 3

# 指定した回数だけノイズデータを生成し、元のデータフレームに追加する
for _ in range(num_new_data):
    noise_data = pd.DataFrame(np.random.normal(loc=noise_mean, scale=noise_std, size=(1, len(df.columns))), columns=df.columns)
    df = pd.concat([df, noise_data], ignore_index=True)

print(df)
