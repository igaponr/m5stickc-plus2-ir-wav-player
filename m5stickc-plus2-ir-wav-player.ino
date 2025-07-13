#include <Arduino.h>
#include <M5Unified.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <SPIFFS.h>

// 音声再生ライブラリ
#include "AudioFileSourceSPIFFS.h"
#include "AudioGeneratorWAV.h"
// I2S出力の代わりにPWM出力のヘッダーをインクルードする
#include "AudioOutputPWM.h"

// IRセンサーの入力ピン
const uint16_t kRecvPin = 36;
IRrecv irrecv(kRecvPin);
decode_results results;

// 音声出力オブジェクトをPWM用に変更
AudioOutputPWM *out = nullptr;
AudioGeneratorWAV *wav = nullptr;
AudioFileSourceSPIFFS *file = nullptr;

// 再生状態を管理するフラグ
bool isPlaying = false;

// 音声ファイルを再生する関数
void playWavFile(const char* filename) {
  if (isPlaying) {
    Serial.println("Already playing, skipping.");
    return; // 再生中は何もしない
  }
  isPlaying = true;

  // 既存のオブジェクトをクリーンアップ
  if (wav) {
    wav->stop();
    delete wav;
    wav = nullptr;
  }
  if (file) {
    delete file;
    file = nullptr;
  }

  file = new AudioFileSourceSPIFFS(filename);
  if (!file->isOpen()) {
    Serial.printf("Failed to open file: %s\n", filename);
    delete file;
    file = nullptr;
    isPlaying = false;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Zundamon Remote");
    return;
  }

  wav = new AudioGeneratorWAV();
  if (!wav->begin(file, out)) {
    Serial.printf("Failed to begin WAV generator for: %s\n", filename);
    delete wav;
    wav = nullptr;
    delete file;
    file = nullptr;
    isPlaying = false;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Zundamon Remote");
    return;
  }


  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("Playing: %s", filename);
  Serial.printf("Attempting to play: %s\n", filename);
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.println("Zundamon Remote");
  Serial.begin(115200);

  // SPIFFSの初期化
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Mount Failed");
    M5.Lcd.println("SPIFFS Failed!");
    while(1); // 無限ループで停止
  }
  Serial.println("SPIFFS Mount Successful");


  // I2Sの代わりにPWM出力を初期化する
  // M5StickC Plus2の内蔵ブザーはGPIO 2に接続されている
  out = new AudioOutputPWM(2);
  // 音量が小さい場合はゲインを上げてみる（最大4.0程度まで）
  out->SetGain(1.0); // デフォルトは1.0
  out->SetOutputModeMono(true); // モノラル出力に設定


  // IR受信の開始
  irrecv.enableIRIn();
  Serial.println("IR receiver enabled");
}

void loop() {
  // 音声再生の処理
  if (wav != nullptr && wav->isRunning()) {
    if (!wav->loop()) {
      Serial.println("Playback finished.");
      wav->stop();
      delete wav;
      wav = nullptr;
      delete file;
      file = nullptr;
      isPlaying = false;
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.println("Zundamon Remote");
    }
  }

  // IR受信の処理
  // 再生中は新しいIR信号を処理しない
  if (!isPlaying && irrecv.decode(&results)) {
    Serial.printf("Received IR code: 0x%08X\n", results.value);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Zundamon Remote");
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.printf("IR: 0x%08X", results.value);


    // 受信したIRコードに対応するWAVファイルを再生
    switch(results.value) {
      case 0x415f01fe: // 切 ボタンに対応するコードと仮定
        Serial.println("Detected code for '切'");
        playWavFile("/415f01fe.wav");
        break;

      case 0x415f817e: // 弱 ボタンに対応するコードと仮定
        Serial.println("Detected code for '弱'");
        playWavFile("/415f817e.wav");
        break;

      case 0x415fa15e: // タイマー ボタンに対応するコードと仮定
        Serial.println("Detected code for 'タイマー'");
        playWavFile("/415fa15e.wav");
        break;

      case 0x415f41be: // 中 ボタンに対応するコードと仮定
        Serial.println("Detected code for '中'");
        playWavFile("/415f41be.wav");
        break;

      case 0x415f21de: // ランプ ボタンに対応するコードと仮定
        Serial.println("Detected code for 'ランプ'");
        playWavFile("/415f21de.wav");
        break;

      case 0x415fc13e: // 強 ボタンに対応するコードと仮定
        Serial.println("Detected code for '強'");
        playWavFile("/415fc13e.wav");
        break;

      default:
        Serial.printf("Unknown IR code: 0x%08X\n", results.value);
        // 未知のコードを受信した場合に何か表示する
        M5.Lcd.setCursor(0, 80);
        M5.Lcd.println("Unknown Code!");
        break;
    }
    irrecv.resume(); // 次の信号受信を再開
  }

  // 短い遅延を入れてCPU使用率を抑える
  delay(10);
}