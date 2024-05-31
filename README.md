# Arduino-Python-Serial

<div id="top"></div>

## 使用技術一覧

<!-- シールド一覧 -->
<!-- 該当するプロジェクトの中から任意のものを選ぶ-->
<p style="display: inline">  
<p style="display: inline">
    <img src="https://img.shields.io/badge/-C-000000.svg?logo=C++&style=for-the-badge">
    <img src="https://img.shields.io/badge/-C++-000000.svg?logo=C++&style=for-the-badge">
    <img src="https://img.shields.io/badge/-Python-000000.svg?logo=C++&style=for-the-badge">
    <img src="https://img.shields.io/badge/-Arduino-000000.svg?logo=C++&style=for-the-badge">


</p>

## 目次

1. [プロジェクトについて](#プロジェクトについて)
2. [環境](#環境)
3. [開発環境構築](#開発環境構築)

<!-- プロジェクト名を記載 -->

## DCONプロジェクト

AI枕を稼働させるためのプロジェクトになります。

<!-- プロジェクトについて -->

## プロジェクトについて
Arduinoで取得した圧力データを入力データとして活用し、AIが枕の空気袋内の空気圧量を推論し、小型ポンプから膨らませることで、最終的に高さ調整を実現しております。

システム設計については、下記の通りになります。

<img src="image/system.jpg">

<p align="right">(<a href="#top">トップへ</a>)</p>

## 環境

<!-- 言語、フレームワーク、ミドルウェア、インフラの一覧とバージョンを記載 -->

| 言語・フレームワーク  | バージョン |
| --------------------- | ---------- |
| Python                | 3.11.9     |
| tensorflow            | 2.16.1     |
| pandas                | 2.2.1      |
| keras                 | 3.1.1      |
| numpy                 | 1.26.4     |

その他Pythonパッケージについては、requirement.txtを参照ください。

<p align="right">(<a href="#top">トップへ</a>)</p>

## 開発環境構築

### venv作成と起動

以下のコマンドで仮想環境を作成できます。

python -m venv 仮想環境名

### ライブラリのインストール

以下のコマンドで仮想環境内にライブラリをインストールできます。

pip install -r for_PC/requirements.txt

<p align="right">(<a href="#top">トップへ</a>)</p>