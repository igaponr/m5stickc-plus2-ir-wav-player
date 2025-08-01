#include "BeepPlayer.h"
#include <M5Unified.h>

BeepPlayer::BeepPlayer(AudioOutput* audioOut)
    : _out(audioOut), _generator(nullptr), _file(nullptr), _id3(nullptr), _currentType(NONE) {
}

BeepPlayer::~BeepPlayer() {
    cleanup();
}

void BeepPlayer::cleanup() {
    if (_generator && _generator->isRunning()) {
        _generator->stop();
    }
    delete _generator;
    _generator = nullptr;

    delete _id3;
    _id3 = nullptr;

    delete _file;
    _file = nullptr;

    _currentType = NONE;
}

void BeepPlayer::stop() {
    cleanup();
}

void BeepPlayer::playAudioFile(const char* filename) {
    // まずは現在の再生を停止し、リソースを解放
    cleanup();

    // ファイル拡張子を取得して判定 (簡易的な方法)
    const char* dot = strrchr(filename, '.');
    if (!dot) {
        Serial.println("Error: No file extension found.");
        return;
    }

    _file = new AudioFileSourceSPIFFS(filename);
    if (!_file || !_file->isOpen()) {
        Serial.printf("Error: Failed to open file %s\n", filename);
        cleanup();
        return;
    }

    Serial.printf("Playing %s\n", filename);

    // 拡張子に応じてジェネレータを生成
    if (strcasecmp(dot, ".wav") == 0) {
        _currentType = WAV;
        _generator = new AudioGeneratorWAV();
        _generator->begin(_file, _out);
    } else if (strcasecmp(dot, ".mp3") == 0) {
        _currentType = MP3;
        _id3 = new AudioFileSourceID3(_file); // MP3はID3を挟む
        _generator = new AudioGeneratorMP3();
        _generator->begin(_id3, _out);
    } else {
        Serial.printf("Error: Unsupported file type: %s\n", dot);
        cleanup();
        return;
    }

    if (!_generator->isRunning()) {
        Serial.println("Error: Failed to start playback.");
        cleanup();
    }
}

void BeepPlayer::loop() {
    if (_generator && _generator->isRunning()) {
        if (!_generator->loop()) {
            // 再生が完了したら全てのリソースを解放
            cleanup();
            Serial.println("Playback finished.");
        }
    }
}

bool BeepPlayer::isPlaying() {
    // _generatorが有効で、かつ再生中かを確認
    return (_generator && _generator->isRunning());
}