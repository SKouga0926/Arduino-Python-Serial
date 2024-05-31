import pandas as pd
import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler, LabelEncoder, OneHotEncoder
from sklearn.metrics import accuracy_score, mean_squared_error
import matplotlib.pyplot as plt

class DnnModel_6:

# public:

    def __init__(self, input_shape_, output_shape_):
        self.__model = self.__build_model(input_shape_, output_shape_)

    def train(self, X_train, y_train, X_valid, y_valid, epochs=1000, batch_size=32):
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
        y_pred = np.argmax(self.__model.predict(new_data), axis=-1)

        if y_pred == 0:
            print("---------------------------")
            print("Predicted sleep position: aomuke")
            print("---------------------------")
        elif y_pred == 1:
            print("---------------------------")
            print("Predicted sleep position: yokomuki")
            print("---------------------------")

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
        model.add(tf.keras.layers.Dense(32, activation='relu', input_shape=input_shape_))
        model.add(tf.keras.layers.BatchNormalization())
        model.add(tf.keras.layers.Dropout(0.1))
        
        # 隠れ層1
        model.add(tf.keras.layers.Dense(64, activation='relu'))
        model.add(tf.keras.layers.BatchNormalization())
        model.add(tf.keras.layers.Dropout(0.1))
        
        # 出力層
        model.add(tf.keras.layers.Dense(output_shape_, activation='softmax'))

        # モデルのコンパイル
        model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
        
        return model

if __name__ == "__main__":

    # Example

     # データの読み込み
    data = pd.read_csv('../../data/raw/dnn_model_6/sensor_data.csv')

    # SleepPosition列の値を変換する
    data.loc[data['SleepPosition'] == 'right_yokomuki', 'SleepPosition'] = 'yokomuki'
    data.loc[data['SleepPosition'] == 'left_yokomuki', 'SleepPosition'] = 'yokomuki'

    # 1000以上の値を持つ列がある行を除外する
    data_filted = data[(data < 1000) & (data > 0)].dropna()

    print("data", data)

    # データの分割
    train_data, remaining_data = train_test_split(data_filted, test_size=0.4, random_state=42)
    validation_data, test_data = train_test_split(remaining_data, test_size=0.5, random_state=42)
    
    print("data size:", len(data))
    print("Train data size:", len(train_data))
    print("Validation data size:", len(validation_data))
    print("Test data size:", len(test_data))

    # 説明変数と目的変数の分離
    X_train = train_data[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
    y_train = train_data[["SleepPosition"]]
    y_train_one_hoted = pd.get_dummies(y_train)

    print(y_train_one_hoted)

    column1 = "Pressure1"  # 入れ替える列1
    column2 = "Pressure2"  # 入れ替える列2

    column3 = "Pressure3"  # 入れ替える列3
    column4 = "Pressure4"  # 入れ替える列4

    X_train_before_swap = X_train.copy()
    X_train[column1], X_train[column2] = X_train[column2], X_train[column1]
    X_train[column3], X_train[column4] = X_train[column4], X_train[column3]
    X_train_aug = pd.concat([X_train_before_swap, X_train], ignore_index=True)
    print("X_train_aug size:", len(X_train_aug))

    y_train_aug = pd.concat([y_train_one_hoted, y_train_one_hoted], ignore_index=True)

    # augデータ
    data_aug = pd.concat([X_train_aug, y_train_aug], axis=1)

    print("data_aug", data_aug)

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
    y_train_noised = data_noised[["SleepPosition_aomuke", "SleepPosition_yokomuki"]]



    # 説明変数と目的変数の分離
    X_valid = validation_data[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
    y_valid = validation_data[["SleepPosition"]]
    y_valid_one_hoted = pd.get_dummies(y_valid)

    X_test = test_data[["Pressure1", "Pressure2", "Pressure3", "Pressure4"]]
    y_test = test_data[["SleepPosition"]]
    y_test_one_hoted = pd.get_dummies(y_test)

    # インスタンス化とモデル構築
    input_shape = (4,)
    output_shape = 2
    dnn_model = DnnModel_6(input_shape, output_shape)

    # 学習
    dnn_model.train(X_train_noised, y_train_noised, X_valid, y_valid_one_hoted)

    # モデルの保存
    # dnn_model.save_model('../../model/dnn_model_6/pressure_model_widgets.h5')

    # 予測の評価
    dnn_model.evaluate(X_test, y_test_one_hoted)

    # 新しいデータで予測
    new_data = np.array([[147.0,15.0,209.0,413.0]])
    prediction = dnn_model.predict(new_data)

    dnn_model.visualize_training('../../photo/acc_loss_plot.png')

