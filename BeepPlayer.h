#ifndef BEEP_PLAYER_H
#define BEEP_PLAYER_H

#include <Arduino.h>
// ★★★ 修正点 ★★★
// AudioFileSourceSPIFFS.h が依存しているため、先に SPIFFS.h をインクルードする
#include <SPIFFS.h>
#include <AudioOutputI2S.h>
#include <AudioGeneratorWAV.h>
#include <AudioGeneratorMP3.h>
#include <AudioFileSourceSPIFFS.h> // このファイルの処理より前にSPIFFSが定義されている必要がある

class BeepPlayer {
public:
    // コンストラクタでAudioOutputI2Sのポインタを受け取る
    BeepPlayer(AudioOutputI2S* audioOutput);
    ~BeepPlayer();

    // 再生処理のためにloop関数を復活
    void loop();

    void playAudioFile(const char* filename);
    bool isPlaying();
    void stop();

private:
    AudioOutputI2S* _out;
    AudioGeneratorWAV* _wav;
    AudioGeneratorMP3* _mp3;
    AudioFileSourceSPIFFS* _file;

    // 現在再生中のジェネレータを追跡するためのポインタ
    AudioGenerator* _currentGenerator;
};

#endif