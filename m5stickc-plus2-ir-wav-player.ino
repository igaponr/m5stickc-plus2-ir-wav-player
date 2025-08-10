// 変更箇所はloop関数のみ
#include <Arduino.h>
#include <M5Unified.h>
#include <SPIFFS.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioGeneratorMP3.h> // MP3再生のために追加
#include <AudioOutputI2S.h>

// クラスヘッダーをインクルード
#include "BeepPlayer.h"
#include "RemoteControlHandler.h"

// IRセンサーの入力ピン
const uint16_t kRecvPin = 36;  // M5GO

// グローバルオブジェクト
AudioOutputI2S* audioOutput = nullptr;
AudioGeneratorWAV* wav = nullptr;
AudioGeneratorMP3* mp3 = nullptr; // MP3再生用にグローバルで宣言
BeepPlayer* beepPlayer = nullptr;
RemoteControlHandler* remoteHandler = nullptr;

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);

    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(2);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.println("Zundamon Remote");
    Serial.begin(115200);

    if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS Mount Failed");
        M5.Lcd.println("SPIFFS Failed!");
        while(1);
    }
    Serial.println("SPIFFS Mount Successful");

    M5.Speaker.begin();
    auto spk_cfg = M5.Speaker.config();
    audioOutput = new AudioOutputI2S(spk_cfg.i2s_port, AudioOutputI2S::INTERNAL_DAC);
    audioOutput->SetPinout(spk_cfg.pin_bck, spk_cfg.pin_ws, spk_cfg.pin_data_out);
    audioOutput->SetGain(0.4);
    audioOutput->SetOutputModeMono(true);

    // 修正: 正しく設定された audioOutput オブジェクトを BeepPlayer のコンストラクタに渡します。
    beepPlayer = new BeepPlayer(audioOutput);
    if (!beepPlayer) {
         Serial.println("Failed to allocate BeepPlayer!");
         M5.Lcd.println("BeepPlayer Init Failed!");
         while(1);
    }

    remoteHandler = new RemoteControlHandler(kRecvPin, beepPlayer);
    if (!remoteHandler) {
         Serial.println("Failed to allocate RemoteControlHandler!");
         M5.Lcd.println("RemoteHandler Init Failed!");
         while(1);
    }

    Serial.println("Setup complete. Ready to receive IR signals.");
}

void loop() {
    if (remoteHandler) {
        remoteHandler->loop();
    }
    // ★★★ 修正点 ★★★
    // BeepPlayerのloopは再生処理に必要なので元に戻します
    if (beepPlayer) {
        beepPlayer->loop();
    }
    delay(10);
}
