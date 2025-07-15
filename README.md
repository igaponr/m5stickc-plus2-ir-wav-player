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
  - ESP8266Audio
  - IRremoteESP8266 ver2.8.6
    - Arduino IDEのボードマネージャでM5Stackのバージョンを**2.1.0**にダウングレード
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

| ボタン | 動作内容      | 発言内容          |
| :--- |:----------|:--------------|
| 切 | 切れた発言をする  | you tubeきんしー  |
| 弱 | 弱そうな発言をする | とってもたのしいねー    |
| タイマー | 5秒後につぶやく  | おやつたべてもいいよ    |
| 中 | 普通な発言をする  |               |
| ランプ | パネルが光る    | せんり　ひの　なかよくして |
| 強 | 強そうな発言をする | ちゃんとはなしをききなさい |

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

## Arduino IDE 1.8.19 (ポータブル版) のインストール手順

### ステップ1: Arduino IDE 1.8.19 のZIPファイルをダウンロードする
1.  Arduinoの公式サイトのソフトウェアダウンロードページにアクセスします。
    [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
2.  ページを下にスクロールし、「Legacy releases」または「Previous Releases」のセクションを探します。
3.  「Arduino IDE 1.8.19」の項目を見つけ、その中から「**Windows ZIP file**」をクリックしてダウンロードします。
    > **注意:** インストーラー版（`.exe`）ではありません。

### ステップ2: 専用のフォルダを作成し、ZIPファイルを展開する
1.  PC内の分かりやすい場所（例: Cドライブ直下やドキュメントフォルダなど）に、Arduino IDE 1.8.19専用の新しいフォルダを作成します。フォルダ名は `Arduino-1.8.19` などが分かりやすいでしょう。
    *   例: `C:\Arduino-1.8.19`
2.  ダウンロードしたZIPファイル（`arduino-1.8.19-windows.zip`）の中身を、すべて先ほど作成したフォルダの中に展開（解凍）します。

### ステップ3: ポータブルモードを有効にする（強く推奨）
この設定を行うと、ライブラリやボード定義などの設定がすべてこのフォルダ内に保存されるようになり、既存の2.0系の環境と完全に分離できます。

1.  先ほどファイルを展開したフォルダ（例: `C:\Arduino-1.8.19`）の中に、手動で新しいフォルダを作成します。
2.  そのフォルダの名前を `portable` にします。
3.  これだけで、このArduino IDEは「ポータブルモード」で動作するようになります。

### ステップ4: Arduino IDE 1.8.19 を起動する
1.  展開したフォルダの中にある `arduino.exe` をダブルクリックして起動します。

これで、既存の2.0系とは独立した、まっさらなArduino IDE 1.8.19が起動します。

***

## 1.8.19 環境でのM5StickC Plus2開発環境セットアップ
新しくインストールした1.8.19には、まだM5StickC Plus2を扱うための設定が入っていません。以下の手順でセットアップが必要です。

### 1. ボード定義の追加
1.  起動したArduino IDE 1.8.19の **「ファイル」** > **「環境設定」** を開きます。
2.  「追加のボードマネージャのURL」に、以下のURLを貼り付けます。
    ```
    https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json
    ```
3.  **「ツール」** > **「ボード」** > **「ボードマネージャ」** を開き、「`M5Stack`」で検索してインストールします。

### 2. ライブラリのインストール
**「ツール」** > **「ライブラリを管理」** から、以下のライブラリを検索してインストールします。
*   `M5StickCPlus2`
*   `IRremoteESP8266`
*   `ESP8266Audio`

### 3. 「ESP32 Sketch Data Upload」ツールの導入
1.  GitHubからツールのZIPファイルをダウンロードします。
    *   [arduino-esp32fs-plugin Releases](https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/)
2.  Arduino IDE 1.8.19のスケッチブックの場所を確認します。（**「ファイル」** > **「環境設定」** で確認できます。ポータブルモードなら `C:\Arduino-1.8.19\portable\sketchbook` のような場所です）
3.  そのスケッチブックのフォルダ内に、手動で `tools` フォルダを作成します。
4.  ダウンロードしたツールのZIPファイルを解凍し、中にある `ESP32FS` フォルダを、今作成した `tools` フォルダの中に丸ごとコピーします。
    *   最終的なパス: `...\sketchbook\tools\ESP32FS\tool\esp32fs.jar`
5.  Arduino IDE 1.8.19を再起動します。
6.  再起動後、「ツール」メニューを開き、ボードを「`M5STICK-C-Plus2`」に設定すれば、「**ESP32 Sketch Data Upload**」が表示されるはずです。

これで、Arduino IDE 2.0系と1.8.19がPCに共存し、1.8.19側でデータアップロードツールを使える環境が整いました。
