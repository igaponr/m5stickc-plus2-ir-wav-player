#include <M5StickCPlus2.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// 音声再生ライブラリ
#include "AudioFileSourceSPIFFS.h"
#include "AudioGeneratorWAV.h"
// ★★★ 変更点 ★★★
// I2S出力の代わりにPWM出力のヘッダーをインクルードする
#include "AudioOutputPWM.h"

// IRセンサーの入力ピン
const uint16_t kRecvPin = 36;
IRrecv irrecv(kRecvPin);
decode_results results;

// ★★★ 変更点 ★★★
// 音声出力オブジェクトをPWM用に変更
AudioOutputPWM *out = nullptr;
AudioGeneratorWAV *wav = nullptr;
AudioFileSourceSPIFFS *file = nullptr;

// 再生状態を管理するフラグ
bool isPlaying = false;

// 音声ファイルを再生する関数
void playWavFile(const char* filename) {
  if (isPlaying) {
    return; // 再生中は何もしない
  }
  isPlaying = true;

  file = new AudioFileSourceSPIFFS(filename);
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("Playing: %s", filename);
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.println("Zundamon Remote");

  // SPIFFSの初期化
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Mount Failed");
    M5.Lcd.println("SPIFFS Failed!");
    return;
  }

  // ★★★ 変更点 ★★★
  // I2Sの代わりにPWM出力を初期化する
  // M5StickC Plus2の内蔵ブザーはGPIO 2に接続されている
  out = new AudioOutputPWM(2);
  // 音量が小さい場合はゲインを上げてみる（最大4.0程度まで）
  // out->SetGain(2.0);

  // IR受信の開始
  irrecv.enableIRIn();
}

void loop() {
  // 音声再生の処理
  if (wav != nullptr && wav->isRunning()) {
    if (!wav->loop()) {
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
  if (!isPlaying && irrecv.decode(&results)) {
    // ここに解析したリモコンのコードを設定
    switch(results.value) {
      case 0xBA45FF00: // 例: 決定ボタンのコード
        Serial.println("OK Button Pressed!");
        playWavFile("/zunda01.wav"); // ファイルパスは / から始める
        break;

      case 0xB946FF00: // 例: 上ボタンのコード
        Serial.println("Up Button Pressed!");
        playWavFile("/zunda02.wav");
        break;

      case 0xB847FF00: // 例: 下ボタンのコード
        Serial.println("Down Button Pressed!");
        playWavFile("/zunda03.wav");
        break;
    }
    irrecv.resume();
  }
}