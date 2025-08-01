#include <M5Unified.h>
#include "FS.h"
#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutput.h" // ★★★ これがM5StickC Plus2用の正しいライブラリです ★★★
#include <M5Unified.h>

// --- 再生設定 ---
#define FILENAME "/415f01fe.wav"    // 再生するファイル名

// --- オブジェクトの宣言 ---
AudioFileSourceSPIFFS *file = nullptr;
AudioGeneratorWAV *wav = nullptr;
AudioOutput *out = nullptr; // ★★★ I2SではなくPWM用のオブジェクトに変更 ★★★

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("WAV Playback Test");
    M5.Lcd.println("for M5StickC Plus2");

    // 1. SPIFFSの初期化
    if (!SPIFFS.begin(true)) {
        M5.Lcd.println("SPIFFS Mount Failed!");
        while (1);
    }
    M5.Lcd.println("SPIFFS OK");

    // 2. テスト用WAVファイルの存在確認
    File testFile = SPIFFS.open(FILENAME, "r");
    if (!testFile) {
        M5.Lcd.printf("File %s not found!\n", FILENAME);
        while (1);
    }
    M5.Lcd.printf("File %s found.\n", FILENAME);
    testFile.close();

    out = new AudioOutput();
    if (out == nullptr) {
        Serial.println("AudioOutputの初期化に失敗しました");
        return;
    }
    out->begin(); // これでM5StickC Plus2のスピーカー出力が準備されます

    // 4. 再生開始
    file = new AudioFileSourceSPIFFS(FILENAME);
    wav = new AudioGeneratorWAV();
    
    // begin()は成功するはずです
    if (wav->begin(file, out)) {
        M5.Lcd.println("Playback started!");
    } else {
        M5.Lcd.println("Playback failed to start.");
    }
}

void loop() {
    if (wav && wav->isRunning()) {
        if (!wav->loop()) {
            wav->stop();
            M5.Lcd.println("Playback finished.");
        }
    }
}