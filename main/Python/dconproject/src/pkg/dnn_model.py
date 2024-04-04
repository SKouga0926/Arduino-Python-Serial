import pandas as pd
import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import mean_squared_error
import matplotlib.pyplot as plt

class DnnModel:

# public:

    def __init__(self, input_shape_, output_shape_):
        self.__model = self.__build_model(input_shape_, output_shape_)

    def train(self, X_train, y_train, X_valid, y_valid, epochs=60, batch_size=32):
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

    def predict(self, new_data):
        return self.__model.predict(new_data)  

    def visualize_training(self, save_path=None):
        if self.__log.history is None:
            print("Model has not been trained yet.")
            return

        epochs_range_acc = range(len(self.__log.history['accuracy']))
        epochs_range_loss = range(len(self.__log.history['loss']))

        # 左側にAccuracyのグラフをプロット
        plt.figure(figsize=(16, 8))
        plt.subplot(1, 2, 1)
        plt.plot(epochs_range_acc, self.__log.history['accuracy'], label='Training Accuracy')
        plt.plot(epochs_range_acc, self.__log.history['val_accuracy'], label='Validation Accuracy')
        plt.legend()
        plt.xlabel('Epoch')
        plt.ylabel('Accuracy')
        plt.title('Training and Validation Accuracy')

        # 右側にLossのグラフをプロット
        plt.subplot(1, 2, 2)
        plt.plot(epochs_range_loss, self.__log.history['loss'], label='Training Loss')
        plt.plot(epochs_range_loss, self.__log.history['val_loss'], label='Validation Loss')
        plt.legend()
        plt.xlabel('Epoch')
        plt.ylabel('Loss')
        plt.title('Training and Validation Loss')

        # 画像を保存
        if save_path:
            plt.savefig(save_path)

        # 表示
        plt.show()  


# private:
    
    def __build_model(self, input_shape_, output_shape_):
        model = tf.keras.Sequential()
        
        # 入力層
        model.add(tf.keras.layers.Dense(64, activation='relu', input_shape=input_shape_))
        model.add(tf.keras.layers.BatchNormalization())
        model.add(tf.keras.layers.Dropout(0.1))
        
        # 隠れ層1
        model.add(tf.keras.layers.Dense(32, activation='relu'))
        model.add(tf.keras.layers.BatchNormalization())
        model.add(tf.keras.layers.Dropout(0.1))
        
        # 隠れ層2
        model.add(tf.keras.layers.Dense(16, activation='relu'))
        model.add(tf.keras.layers.BatchNormalization())
        model.add(tf.keras.layers.Dropout(0.1))
        
        # 出力層
        model.add(tf.keras.layers.Dense(output_shape_))
        
        # モデルのコンパイル
        model.compile(optimizer='adam', loss='mean_squared_error', metrics=['accuracy'])
        
        return model
    


if __name__ == "__main__":

    # Example

    # データの読み込み
    data = pd.read_csv('../../data/processed/sensor_data.csv')
    data_valid = pd.read_csv('../../data/processed/sensor_data_for_valid.csv')

    # 説明変数と目的変数の分離
    X = data[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
    y = data[["AirPressure1", "AirPressure2", "AirPressure3", "AirPressure4", "AirPressure5", "AirPressure6"]]

    # データの分割
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

    # 説明変数と目的変数の分離
    X_valid = data_valid[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
    y_valid = data_valid[["AirPressure1", "AirPressure2", "AirPressure3", "AirPressure4", "AirPressure5", "AirPressure6"]]

    # インスタンス化とモデル構築
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
    new_data = np.array([[70.0,0.0,129.0,4.0]])
    prediction = dnn_model.predict(new_data)

    # 入力データを表示
    print("Input data :", new_data)

    # 入力データの正解値
    Label_training_data = np.array([[0.9,0.82,0.86,0.88,1.16,0.86]])
    print("Labeled training data: ", Label_training_data)

    # 予測結果を表示
    print("Prediction:", prediction)

    dnn_model.visualize_training('../../photo/acc_loss_plot.png')