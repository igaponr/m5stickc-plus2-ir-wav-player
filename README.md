# m5stickc-plus2-ir-wav-player

## 目的
- リモコンを活用したマイコン製作物を作る

## 背景
- 会社のハッカソンイベントで作った

## 説明
- 家にありそうなリモコンで、マイコンを制御して、音声の再生などを行う

## 環境
- Windows11 24H2(OSビルド 26100.4652)
- ArduinoIDE バージョン：1.8.19(=SPIFFSアップロード用)
- ArduinoIDE バージョン：2.3.6(=コンパイル用)
  - 日付：2025-04-09T11:26:55.498Z
  - CLIバージョン：1.2.0
- ライブラリ
  - IRremoteESP8266
  - Arduino IDEのボードマネージャでM5Stackのバージョンを**2.1.0**に設定
  - [arduino-audio-tools](https://github.com/pschatzmann/arduino-audio-tools)はzipをダウンロードして、メニュー[スケッチ]-[ライブラリをインクルード]-[.zip形式のライブラリをインストール]する
- VOICEVOX Ver. 0.24.2
- Audacity 3.7.4
- マイコン
  - [M5StickC Plus2](https://docs.m5stack.com/ja/core/M5StickC%20PLUS2)
- IRセンサー
  - [M5GO](https://docs.m5stack.com/ja/core/m5go)についていたIRセンサー
  - M5StickC Plus2 の Groveポートに接続
  - 33PIN
- リモコン
  - タカラスタンダードのレンジフード用リモコンの信号

IRrecvDemo.inoでリモコンボタンコードを調べたところ
![リモコンボタンコード](./image/リモコンボタンコード.png)
![リモコン](./image/リモコン.JPG)

| ボタン | 受信信号 |
| :--- | :--- |
| 切 | `415F01FE` |
| 弱 | `415F817E` |
| タイマー | `415FA15E` |
| 中 | `415F41BE` |
| ランプ | `415F21DE` |
| 強 | `415FC13E` |

## 設定
- M5StickC Plus2に音声データをSPIFFSにアップロードする
- M5StickC Plus2にスケッチを書き込む
- Grove線でIRセンサーを接続する
- リセットして動かす

## 使い方
- IRセンサーに向かって、リモコンのボタンを押すと以下の通り動く

| ボタン | 動作内容      |
| :--- |:----------|
| 切 | 切れた発言をする  |
| 弱 | 弱そうな発言をする |
| タイマー | 5秒後につぶやく  |
| 中 | 普通な発言をする  |
| ランプ | パネルが光る    |
| 強 | 強そうな発言をする |

## 音声データ作成方法
- [VOICEVOX](https://voicevox.hiroshiba.jp/)でボタンごとの音声を作成した
![音声作成](./image/音声作成.png)
- AIさんのアドバイス「WAVファイルの形式を以下のように変更すると、比較的きれいに（ブザーの限界の範囲で）再生されやすくな」とのこと
  - サンプリングレート: 8000 Hz （または16000Hz。8000Hzの方が成功しやすいです）
  - チャンネル: モノラル
  - フォーマット: 8-bit Unsigned PCM （または16-bit PCM）
- 無料の音声編集ソフト「[Audacity](https://forest.watch.impress.co.jp/library/software/audacity/)」を使って、WAVファイルをM5StickC Plus2の内蔵ブザー再生に適した形式に変換
  - メニューの[トラック]-[再サンプリング]で8000Hzを選択
  - メニューの[ファイル]-[オーディオをエクスポート]でdataフォルダ以下に保存

## 音声データのアップロード方法
- ArduinoIDE 1.8.19を使用する
- [arduino-esp32fs-pluginプラグイン](https://github.com/me-no-dev/arduino-esp32fs-plugin)をセットアップする
- スケッチがあるフォルダ内の「data」フォルダにアップロードするファイルを保存する
- メニュー[ツール]-[ESP32 Sketch Data Upload]を選ぶ
- ダイアログが表示されたら、SPIFFSを選びOKをクリックする
  - 参考記事[SPIFFS/LittleFS Filesystem へアップロードする方法](https://www.farmsoft.jp/2065/)