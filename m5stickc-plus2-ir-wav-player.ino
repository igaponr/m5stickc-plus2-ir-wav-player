#include <Arduino.h>
#include <M5Unified.h>
#include <SPIFFS.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h> // I2S出力用のヘッダーをインクルード
// クラスヘッダーをインクルード
#include "BeepPlayer.h"
#include "RemoteControlHandler.h"

// IRセンサーの入力ピン
const uint16_t kRecvPin = 33;

// グローバルオブジェクト
AudioOutputI2S* audioOutput = nullptr; 
BeepPlayer* beepPlayer = nullptr;
RemoteControlHandler* remoteHandler = nullptr;

void setup() {
    M5.begin();
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

    audioOutput = new AudioOutputI2S();
    if (!audioOutput) {
         Serial.println("Failed to allocate AudioOutputI2S!");
         M5.Lcd.println("Audio Init Failed!");
         while(1);
    }

    // SetPinout(BCLK, LRC/WS, DOUT)
    audioOutput->SetPinout(12, 0, 2);
    audioOutput->SetGain(1.0); // 必要に応じてゲインを調整

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
    if (beepPlayer) {
        beepPlayer->loop();
    }
    delay(10);
}