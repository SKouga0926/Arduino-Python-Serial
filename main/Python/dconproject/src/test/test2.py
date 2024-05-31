import pandas as pd

# データフレームの作成
df1 = pd.DataFrame({'A': [1, 2, 3], 'B': [4, 5, 6]})
df2 = pd.DataFrame({'C': [7, 8, 9], 'D': [10, 11, 12]})

# 列方向に結合
result = pd.concat([df1, df2], axis=1)

print(result)
