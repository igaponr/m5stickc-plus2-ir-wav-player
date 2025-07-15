#include "BeepPlayer.h"

BeepPlayer::BeepPlayer(AudioOutput* audioOut) : _out(audioOut) {
    _wav = new AudioGeneratorWAV();
    _file = new AudioFileSourceSPIFFS();
}

BeepPlayer::~BeepPlayer() {
    if (_wav && _wav->isRunning()) {
        _wav->stop();
    }
    delete _wav;
    delete _file;
}

void BeepPlayer::playWavFile(const char* filename) {
    if (_wav->isRunning()) {
        _wav->stop();
    }
    _file->open(filename);
    // これで _wav->begin が正しい型の引数を受け取るようになります
    if (!_wav->begin(_file, _out)) {
        Serial.println("Failed to start WAV playback!");
    }
}

void BeepPlayer::loop() {
    if (_wav && _wav->isRunning()) {
        if (!_wav->loop()) {
            _wav->stop();
        }
    }
}

bool BeepPlayer::isPlaying() {
    if (_wav) {
        return _wav->isRunning();
    }
    return false;
}