import pandas as pd

input_file = '../../../data/interim/dnn_model_4/sensor_data_for_before_model.csv'

# CSVファイルを読み込む
df = pd.read_csv(input_file)

# 外れ値を除く処理とその行数をカウント
removed_rows_count = 0
for column in df.columns:
    if df[column].dtype != 'object':  # カテゴリカルデータは無視する
        outliers = df[df[column] >= 1000]
        removed_rows_count += len(outliers)
        df = df[df[column] < 1000]

# データを新しいCSVファイルとして保存
df.to_csv('../../../data/interim2/dnn_model_4/sensor_data_for_before_model.csv', index=False)

print("外れ値を除いた行数:", removed_rows_count)
print("新しいファイル '{}' が作成されました。".format('../../../data/interim2/dnn_model_4/sensor_data_for_before.csv'))
