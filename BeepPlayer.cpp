#include "BeepPlayer.h"
#include <M5Unified.h> // Serialデバッグのためにインクルード

BeepPlayer::BeepPlayer(AudioOutput* audioOut)
    : _out(audioOut), _wav(nullptr), _file(nullptr) {
    // オブジェクトの生成はここでは行わない
}

BeepPlayer::~BeepPlayer() {
    if (_wav) {
        _wav->stop();
        delete _wav;
        _wav = nullptr;
    }
    if (_file) {
        delete _file;
        _file = nullptr;
    }
}

void BeepPlayer::playWavFile(const char* filename) {
    // 既に再生中の場合は、一度停止してリソースを解放する
    if (isPlaying()) {
        _wav->stop();
        delete _wav;
        _wav = nullptr;
        delete _file;
        _file = nullptr;
    }

    // 再生の都度、新しいオブジェクトを生成する
    _file = new AudioFileSourceSPIFFS(filename);
    _wav = new AudioGeneratorWAV();

    // begin()で再生を開始する
    if (_wav->begin(_file, _out)) {
        Serial.printf("Playback started for: %s\n", filename);
    } else {
        // 失敗した場合は、生成したオブジェクトをすぐに破棄する
        Serial.printf("Error starting playback for: %s\n", filename);
        delete _wav;
        _wav = nullptr;
        delete _file;
        _file = nullptr;
    }
}

void BeepPlayer::loop() {
    if (_wav && _wav->isRunning()) {
        if (!_wav->loop()) {
            // 再生が完了した瞬間の処理
            _wav->stop();
            delete _wav;
            _wav = nullptr;
            delete _file;
            _file = nullptr;
            Serial.println("Playback finished. Resources freed.");
        }
    }
}

bool BeepPlayer::isPlaying() {
    return (_wav && _wav->isRunning());
}