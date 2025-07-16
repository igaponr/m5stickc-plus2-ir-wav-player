#ifndef BEEP_PLAYER_H
#define BEEP_PLAYER_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioFileSourceID3.h>   // MP3用にインクルード追加
#include <AudioGeneratorWAV.h>
#include <AudioGeneratorMP3.h>   // MP3用にインクルード追加
#include <AudioOutput.h>

class BeepPlayer {
public:
    BeepPlayer(AudioOutput* audioOut);
    ~BeepPlayer();

    void loop();
    // メソッド名をより汎用的に変更
    void playAudioFile(const char* filename);
    bool isPlaying();
    void stop(); // 明示的に停止するメソッドを追加

private:
    // 【変更点】ジェネレータを親クラスのポインタで保持
    AudioGenerator* _generator;
    AudioFileSourceSPIFFS* _file;
    // 【追加】MP3再生で必要になるID3ソース
    AudioFileSourceID3* _id3;
    AudioOutput* _out;

    // 現在のファイルタイプを保持する変数 (WAV or MP3)
    enum FileType { NONE, WAV, MP3 };
    FileType _currentType;

    void cleanup(); // リソース解放をまとめるヘルパー関数
};

#endif