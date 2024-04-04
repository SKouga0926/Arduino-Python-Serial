import pandas as pd
import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import mean_squared_error
import matplotlib.pyplot as plt


import sys
sys.path.append('../pkg')

from dnn_model import DnnModel



if __name__ == "__main__":

    # Example

    # データの読み込み
    data = pd.read_csv('../../data/raw/sensor_data.csv')
    data_valid = pd.read_csv('../../data/raw/sensor_data_for_valid.csv')

    # 説明変数と目的変数の分離
    X = data[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
    y = data[["AirPressure1", "AirPressure2", "AirPressure3", "AirPressure4", "AirPressure5", "AirPressure6"]]

    # データの分割
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
    
    # 検証用データ読み込み
    X_valid = data_valid[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
    y_valid = data_valid[["AirPressure1", "AirPressure2", "AirPressure3", "AirPressure4", "AirPressure5", "AirPressure6"]]


    # Dnnインスタンス化とモデル構築
    input_shape = (4,)
    output_shape = 6
    dnn_model = DnnModel(input_shape, output_shape)

    # 学習
    dnn_model.train(X_train, y_train, X_valid, y_valid)

    # モデルの保存
    dnn_model.save_model('../../model/pressure_model_widgets.h5')

    # 予測の評価
    dnn_model.evaluate(X_test, y_test)

    # 新しいデータで予測
    new_data = np.array([[94.0, 112.0, 11, 54]])
    prediction = dnn_model.predict(new_data)

    # 予測結果を表示
    print("Prediction:", prediction)

    dnn_model.visualize_training('../../photo/acc_loss_plot.png')