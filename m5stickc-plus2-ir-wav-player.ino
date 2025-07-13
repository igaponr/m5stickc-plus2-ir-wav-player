#include <Arduino.h>
#include <M5Unified.h>
#include <SPIFFS.h>

// Audio関連ヘッダー
// ESP8266Audio関連を先に、かつ AudioOutput.h を AudioOutputPWM.h より先にインクルード
#include <AudioOutput.h>       // ESP8266Audio版のAudioOutput.hをインクルード
#include <AudioOutputPWM.h>    // AudioOutputPWMを使用するためインクルード
#include <AudioFileSourceSPIFFS.h> // BeepPlayerで使用するが、メインファイルでも参照するためインクルード
#include <AudioGeneratorWAV.h> // BeepPlayerで使用するが、メインファイルでも参照するためインクルード
#include <AudioTools.h>        // AudioToolsの他の機能も使用する場合にインクルード


// クラスヘッダーをインクルード
#include "BeepPlayer.h"
#include "RemoteControlHandler.h"

// IRセンサーの入力ピン
const uint16_t kRecvPin = 33;

// グローバルオブジェクト (setupで初期化し、loopで使用)
// クラスインスタンスはヒープに確保し、ポインタで管理します
AudioOutputPWM* audioOutput = nullptr; // AudioOutputPWMはsetupで初期化し、BeepPlayerに渡す
BeepPlayer* beepPlayer = nullptr;
RemoteControlHandler* remoteHandler = nullptr;


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
    audioOutput = new AudioOutputPWM(2); // ヒープに確保
    // audioOutputがnewで失敗していないかチェック (メモリ不足など)
    if (!audioOutput) {
         Serial.println("Failed to allocate AudioOutputPWM!");
         M5.Lcd.println("Audio Init Failed!");
         while(1); // 初期化失敗で停止
    }

    audioOutput->SetGain(1.0); // デフォルトは1.0
    audioOutput->SetOutputModeMono(true); // モノラル出力に設定

    // BeepPlayerとRemoteControlHandlerのインスタンスを生成
    // BeepPlayerには初期化したAudioOutputPWMのポインタを渡す
    beepPlayer = new BeepPlayer(audioOutput); // ヒープに確保
    if (!beepPlayer) {
         Serial.println("Failed to allocate BeepPlayer!");
         // BeepPlayerがないとリモコンも機能しないのでここで停止しても良い
         M5.Lcd.println("BeepPlayer Init Failed!");
         while(1);
    }


    // RemoteControlHandlerにはIR受信ピンとBeepPlayerのポインタを渡す
    remoteHandler = new RemoteControlHandler(kRecvPin, beepPlayer); // ヒープに確保
     if (!remoteHandler) {
         Serial.println("Failed to allocate RemoteControlHandler!");
         M5.Lcd.println("RemoteHandler Init Failed!");
         while(1); // リモコンハンドラがないとIR受信できないので停止
    }


    Serial.println("Setup complete. Ready to receive IR signals.");
}

void loop() {
    // 各クラスのloopメソッドを呼び出す
    // nullptrチェックを追加
    if (beepPlayer) {
        beepPlayer->loop();
    }
    if (remoteHandler) {
        remoteHandler->loop();
    }

    // 短い遅延を入れてCPU使用率を抑える
    delay(10);
}

// 注意: デストラクタを呼び出すには main.cpp を修正する必要がありますが、
// 通常Arduino環境ではloopから抜け出さないため、ここでは省略します。
// 長時間実行する場合は、適切なタイミングでのdeleteを検討してください。
// 特にM5Stack/M5StickCでは、リセット時にすべて解放されるため通常は不要です。