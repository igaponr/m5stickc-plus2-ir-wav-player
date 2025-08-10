#include "BeepPlayer.h"
#include <SPIFFS.h>

// ★★★ 全体的に修正 ★★★

BeepPlayer::BeepPlayer(AudioOutputI2S* audioOutput)
    : _out(audioOutput), _wav(nullptr), _mp3(nullptr), _file(nullptr), _currentGenerator(nullptr) {
    // コンストラクタで受け取ったポインタをメンバ変数に保存
}

BeepPlayer::~BeepPlayer() {
    stop(); // デストラクタで再生を停止し、リソースを解放
}

void BeepPlayer::playAudioFile(const char* filename) {
    if (isPlaying()) {
        stop();
    }

    Serial.printf("Playing %s\n", filename);

    _file = new AudioFileSourceSPIFFS(filename);
    if (!_file || !_file->isOpen()) {
        Serial.printf("Failed to open file: %s\n", filename);
        delete _file;
        _file = nullptr;
        return;
    }

    // ファイルの拡張子に応じて適切なジェネレータを選択
    if (strstr(filename, ".wav") != nullptr) {
        _wav = new AudioGeneratorWAV();
        _currentGenerator = _wav;
        _wav->begin(_file, _out);
    } else if (strstr(filename, ".mp3") != nullptr) {
        _mp3 = new AudioGeneratorMP3();
        _currentGenerator = _mp3;
        _mp3->begin(_file, _out);
    } else {
        Serial.printf("Error: Unsupported file type: %s\n", filename);
        delete _file; // 不要なファイルソースを閉じる
        _file = nullptr;
    }
}

void BeepPlayer::stop() {
    if (_currentGenerator && _currentGenerator->isRunning()) {
        _currentGenerator->stop();
    }
    // メモリを解放
    if (_wav) { delete _wav; _wav = nullptr; }
    if (_mp3) { delete _mp3; _mp3 = nullptr; }
    if (_file) { delete _file; _file = nullptr; }
    _currentGenerator = nullptr;
}

bool BeepPlayer::isPlaying() {
    if (_currentGenerator) {
        return _currentGenerator->isRunning();
    }
    return false;
}

void BeepPlayer::loop() {
    if (_currentGenerator && _currentGenerator->isRunning()) {
        if (!_currentGenerator->loop()) {
            // 再生が終了したら停止処理を呼ぶ
            stop();
            Serial.println("Playback finished.");
        }
    }
}