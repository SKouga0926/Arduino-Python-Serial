import pandas as pd
import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import mean_squared_error
import matplotlib.pyplot as plt
from tensorflow.keras.optimizers import Adam
# import psutil
# from memory_profiler import profile

class DnnModel:

# public:

    def __init__(self, input_shape_, output_shape_):
        self.__model = self.__build_model(input_shape_, output_shape_)

    def train(self, X_train, y_train, X_valid, y_valid, epochs=300, batch_size=32):
        self.__log =  self.__model.fit(
            X_train, y_train,
            epochs = epochs, batch_size = batch_size,
            validation_data = (X_valid, y_valid),
            verbose=2
        )

    def save_model(self, file_path):
        self.__model.save(file_path)

    def load_model(self, file_path):
        self.__model = tf.keras.models.load_model(file_path)

    def evaluate(self, X_test, y_test):
        y_pred = self.__model.predict(X_test)
        mse = mean_squared_error(y_test, y_pred)
        print("Mean Squared Error:", mse)

    # @profile
    def predict(self, new_data):
        return self.__model.predict(new_data)  

    def visualize_training(self, save_path=None):
        if self.__log.history is None:
            print("Model has not been trained yet.")
            return

        epochs_range_loss_rmse = range(len(self.__log.history['loss']))

        valid_rmse = np.sqrt(self.__log.history['val_loss'])

        # 右側にLossのグラフをプロット
        plt.plot(epochs_range_loss_rmse, valid_rmse, label='Validation Rmse Loss')
        plt.legend()
        plt.xlabel('Epoch')
        plt.ylabel('Rmse')
        plt.title('Validation Rmse Loss')

        # 画像を保存
        if save_path:
            plt.savefig(save_path)

        # 表示
        plt.show()  


# private:
    
    def __build_model(self, input_shape_, output_shape_):
        model = tf.keras.Sequential()
        
        # 入力層
        model.add(tf.keras.layers.Dense(32, activation='relu', input_shape=input_shape_))
        model.add(tf.keras.layers.BatchNormalization())
        model.add(tf.keras.layers.Dropout(0.1))

        # 隠れ層1
        model.add(tf.keras.layers.Dense(64, activation='relu'))
        model.add(tf.keras.layers.BatchNormalization())
        model.add(tf.keras.layers.Dropout(0.1))
        
        # 出力層
        model.add(tf.keras.layers.Dense(output_shape_))

        adam_optimizer = Adam(learning_rate=0.001)  # 例：学習率0.001
        model.compile(optimizer=adam_optimizer, loss='mean_squared_error', metrics=['mse'])

        print(model.summary())
        
        return model
    


if __name__ == "__main__":

    # Example

    # データの読み込み
    data = pd.read_csv('../../data/raw/dnn_model_4/sensor_data.csv')

    # 1000以上の値を持つ列がある行を除外する
    data_filted = data[(data < 1000) & (data > 0)].dropna()

    # データの分割
    train_data, remaining_data = train_test_split(data_filted, test_size=0.4, random_state=42)
    validation_data, test_data = train_test_split(remaining_data, test_size=0.5, random_state=42)
    
    print("data size:", len(data_filted))
    print("Train data size:", len(train_data))
    print("Validation data size:", len(validation_data))
    print("Test data size:", len(test_data))

    # 説明変数と目的変数の分離
    X_train = train_data[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
    y_train = train_data[["AirPressure1", "AirPressure2", "AirPressure3", "AirPressure4", "AirPressure5", "AirPressure6"]]

    column1 = "Pressure1"  # 入れ替える列1
    column2 = "Pressure2"  # 入れ替える列2

    column3 = "Pressure3"  # 入れ替える列3
    column4 = "Pressure4"  # 入れ替える列4

    X_train_before_swap = X_train.copy()
    X_train[column1], X_train[column2] = X_train[column2], X_train[column1]
    X_train[column3], X_train[column4] = X_train[column4], X_train[column3]
    X_train_aug = pd.concat([X_train_before_swap, X_train], ignore_index=True)
    print("X_train_aug size:", len(X_train_aug))

    # X_train_combined.to_csv('../../data/raw/dnn_model_4/sensor_data_for_before_model.csv', index=False)

    column5 = "AirPressure1"  # 入れ替える列5
    column6 = "AirPressure3"  # 入れ替える列6

    column7 = "AirPressure4"  # 入れ替える列5
    column8 = "AirPressure6"  # 入れ替える

    y_train_before_swap = y_train.copy()
    y_train[column5], y_train[column6] = y_train[column6], y_train[column5]
    y_train[column7], y_train[column8] = y_train[column8], y_train[column7]
    y_train_aug = pd.concat([y_train_before_swap, y_train], ignore_index=True)
    print("y_train_aug size:", len(y_train_aug))

    # 元データ
    data_aug = pd.concat([X_train_aug, y_train_aug], axis=1)

    data_noised = data_aug.copy()
    temp_data_aug_for_noised = data_aug.copy()

    for i in range(12):
        for j, row in data_aug.iterrows():
            mean_val = row[['Pressure1', 'Pressure2', 'Pressure3', 'Pressure4']].mean()
            noise = np.random.normal(0, mean_val, size=4) * 0.10
            temp_data_aug_for_noised.loc[j, ['Pressure1', 'Pressure2', 'Pressure3', 'Pressure4']] += noise
            temp_data_aug_for_noised.loc[j, ['Pressure1', 'Pressure2', 'Pressure3', 'Pressure4']] = np.around(temp_data_aug_for_noised.loc[j, ['Pressure1', 'Pressure2', 'Pressure3', 'Pressure4']], decimals=1)
            
            temp_data_aug_for_noised = data_aug.copy()

        data_noised = pd.concat([data_noised, temp_data_aug_for_noised], ignore_index=True)

    print(data_noised)

    print("data_noised size:", len(data_noised))

    X_train_noised = data_noised[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
    y_train_noised = data_noised[["AirPressure1", "AirPressure2", "AirPressure3", "AirPressure4", "AirPressure5", "AirPressure6"]]


    # 説明変数と目的変数の分離
    X_valid = validation_data[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
    y_valid = validation_data[["AirPressure1", "AirPressure2", "AirPressure3", "AirPressure4", "AirPressure5", "AirPressure6"]]

    X_test = test_data[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
    y_test = test_data[["AirPressure1", "AirPressure2", "AirPressure3", "AirPressure4", "AirPressure5", "AirPressure6"]]

    # インスタンス化とモデル構築
    input_shape = (4,)
    output_shape = 6
    dnn_model = DnnModel(input_shape, output_shape)




    # # trainデータによる学習
    # dnn_model.train(X_train, y_train, X_valid, y_valid)

    # # 予測の評価
    # # mse 0.003877
    # # rmse 0.062266
    # dnn_model.evaluate(X_test, y_test)

    # # augデータによる学習
    # dnn_model.train(X_train_aug, y_train_aug, X_valid, y_valid)

    # # 予測の評価
    # # mse 0.001461
    # # rmse 0.038223
    # dnn_model.evaluate(X_test, y_test)

    # noisedデータによる学習
    dnn_model.train(X_train_noised, y_train_noised, X_valid, y_valid)

    # 予測の評価
    # mse 0.0009319
    # rmse 0.030527
    dnn_model.evaluate(X_test, y_test)

    dnn_model.predict(X_test[0:1])


    # モデルの保存
    # dnn_model.save_model('../../model/dnn_model_4/pressure_model_widgets.h5')

    # # 新しいデータで予測
    # new_data = np.array([[68.0,26.0,110.0,134.0]])
    # prediction = dnn_model.predict(new_data)

    # # 入力データを表示
    # print("Input data :", new_data)

    # # 入力データの正解値
    # Label_training_data = np.array([[0.9,0.81,0.86,0.88,1.13,0.86]])
    # print("Labeled training data: ", Label_training_data)

    # # 予測結果を表示
    # print("Prediction:", prediction)

    dnn_model.visualize_training('../../photo/acc_loss_plot.png')
