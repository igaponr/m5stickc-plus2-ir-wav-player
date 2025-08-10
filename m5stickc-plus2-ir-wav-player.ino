#include <Arduino.h>
#include <M5Unified.h>
#include <SPIFFS.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>

#include "BeepPlayer.h"
#include "RemoteControlHandler.h"

const uint16_t kRecvPin = 36;  // M5GOのIRピン

AudioOutputI2S* audioOutput = nullptr;
BeepPlayer* beepPlayer = nullptr;
RemoteControlHandler* remoteHandler = nullptr;

void setup() {
    auto cfg = M5.config();

    // ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
    // ★ これが真の最終解決策です。
    // ★ RMTの競合元であるLEDバー（Neopixel）機能を、ピン設定を-1にすることで完全に無効化します。
    // ★ これによりRMTチャンネル0が解放され、IRライブラリが自由に使用できるようになります。
    cfg.pin_neopixel = -1;
    // ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

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
    M5.Speaker.end();

    auto spk_cfg = M5.Speaker.config();
    audioOutput = new AudioOutputI2S(spk_cfg.i2s_port, AudioOutputI2S::EXTERNAL_I2S);
    audioOutput->SetPinout(spk_cfg.pin_bck, spk_cfg.pin_ws, spk_cfg.pin_data_out);
    audioOutput->SetGain(0.4);
    audioOutput->SetOutputModeMono(true);

    beepPlayer = new BeepPlayer(audioOutput);
    remoteHandler = new RemoteControlHandler(kRecvPin, beepPlayer);

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
