#include "BeepPlayer.h" // このヘッダーがAudio関連のインクルードを適切に行っているため、ここでは追加のAudio関連ヘッダーは不要

BeepPlayer::BeepPlayer(AudioOutputPWM* audioOut) : _out(audioOut) {
    // AudioOutputPWM object is created and managed outside this class (in setup)
    // ここでは初期化済みポインタを受け取るだけ
}

BeepPlayer::~BeepPlayer() {
    // Clean up objects owned by this class
    if (_wav) {
        _wav->stop();
        delete _wav;
        _wav = nullptr;
    }
    if (_file) {
        // Closing the file source is usually handled by the generator's stop/destructor,
        // but explicitly deleting the source object is needed here.
        delete _file;
        _file = nullptr;
    }
    // Do NOT delete _out, it is owned by the caller (e.g., the .ino setup function)
    Serial.println("BeepPlayer destroyed."); // For debugging
}

void BeepPlayer::playWavFile(const char* filename) {
    if (_isPlaying) {
        Serial.println("Already playing, skipping.");
        return; // 再生中は何もしない
    }
    _isPlaying = true;

    // Clean up existing objects before starting new playback
    // (This also handles the case where playback finished but loop() wasn't called yet to cleanup)
    // これはデストラクタと同じクリーンアップ処理
    if (_wav) {
        _wav->stop();
        delete _wav;
        _wav = nullptr; // Ensure pointer is null after deletion
    }
    if (_file) {
        delete _file;
        _file = nullptr; // Ensure pointer is null after deletion
    }

    _file = new AudioFileSourceSPIFFS(filename);
    // newが成功したか、かつファイルが開けたかチェック
    if (!_file || !_file->isOpen()) {
        Serial.printf("Failed to open file: %s\n", filename);
        if (_file) { // ファイルオブジェクトが生成されていれば削除
            delete _file;
            _file = nullptr;
        }
        _isPlaying = false;
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.println("Zundamon Remote");
        return;
    }

    _wav = new AudioGeneratorWAV();
     // newが成功したかチェック
    if (!_wav) {
         Serial.println("Failed to allocate AudioGeneratorWAV!");
         delete _file; _file = nullptr; // file sourceもクリーンアップ
         _isPlaying = false;
         M5.Lcd.fillScreen(BLACK);
         M5.Lcd.setCursor(0, 0);
         M5.Lcd.println("Zundamon Remote");
         return;
    }

    // Check if _out is valid before beginning (should be valid if setup was successful)
    // AudioGeneratorWAV::beginはAudioOutput*を期待します。
    // BeepPlayer.hでESP8266Audio関連ヘッダーを適切にインクルードすることで、
    // AudioGeneratorWAVが期待するAudioOutputが適切に解決されるはずです。
    if (_out == nullptr || !_wav->begin(_file, _out)) {
        Serial.printf("Failed to begin WAV generator for: %s\n", filename);
        if (_wav) { // _wavが生成されていれば削除
            delete _wav;
            _wav = nullptr; // Ensure pointer is null after deletion
        }
        if (_file) { // _fileが生成されていれば削除
            delete _file;
            _file = nullptr; // Ensure pointer is null after deletion
        }
        _isPlaying = false;
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.println("Zundamon Remote");
        return;
    }

    // Display playback info on LCD
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf("Playing: %s", filename);
    Serial.printf("Attempting to play: %s\n", filename);
}

void BeepPlayer::loop() {
    // Continue playback if WAV generator is running
    if (_wav != nullptr && _wav->isRunning()) {
        if (!(_wav->loop())) { // loop()がfalseを返したら再生終了
            Serial.println("Playback finished.");
            // クリーンアップ処理
            _wav->stop(); // 停止
            delete _wav;  // メモリ解放
            _wav = nullptr; // ポインタを無効にする
            delete _file; // ファイルソースも解放
            _file = nullptr; // ポインタを無効にする
            _isPlaying = false;

            // Restore default display
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.setCursor(0, 0);
            M5.Lcd.println("Zundamon Remote");
        }
    }
    // else if (!_isPlaying) { // Playback is not active, no need to call loop()
    //     // Optional: Add some idle processing if needed
    // }
}