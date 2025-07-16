#ifndef BEEP_PLAYER_H
#define BEEP_PLAYER_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutput.h>

class BeepPlayer {
public:
    BeepPlayer(AudioOutput* audioOut);
    ~BeepPlayer();
    void loop();
    void playWavFile(const char* filename);
    bool isPlaying();

private:
    AudioGeneratorWAV* _wav;
    AudioFileSourceSPIFFS* _file;
    AudioOutput* _out;
};

#endif