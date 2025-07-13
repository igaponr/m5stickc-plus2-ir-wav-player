#ifndef BEEP_PLAYER_H
#define BEEP_PLAYER_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <M5Unified.h> // For M5.Lcd

// Audio関連ヘッダー
// ESP8266Audio関連を先に、かつ AudioOutput.h を AudioOutputPWM.h より先にインクルード
#include <AudioOutput.h>        // ESP8266Audio版のAudioOutput.hをインクルード
#include <AudioOutputPWM.h>     // BeepPlayerクラス内でAudioOutputPWM*を使用するため早期にインクルード
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioTools.h>         // AudioToolsの他の機能も使用する場合にインクルード


/**
 * @brief WAVファイルを再生するクラス
 */
class BeepPlayer {
public:
    /**
     * @brief コンストラクタ
     * @param audioOut PWM出力オブジェクトへのポインタ
     */
    BeepPlayer(AudioOutputPWM* audioOut);

    /**
     * @brief デストラクタ
     */
    ~BeepPlayer(); // Destructor to clean up owned resources

    /**
     * @brief 指定されたWAVファイルを再生開始する
     * @param filename SPIFFS上のWAVファイルパス
     */
    void playWavFile(const char* filename);

    /**
     * @brief 再生中の処理を行う (loop関数から呼び出す)
     */
    void loop();

    /**
     * @brief 現在音声が再生中かどうかを取得する
     * @return 再生中の場合はtrue、そうでない場合はfalse
     */
    bool isPlaying() const { return _isPlaying; }

private:
    AudioOutputPWM* _out; // 外部から渡されるポインタ (このクラスは所有しない)
    AudioGeneratorWAV* _wav = nullptr; // このクラスが所有するオブジェクト
    AudioFileSourceSPIFFS* _file = nullptr; // このクラスが所有するオブジェクト
    bool _isPlaying = false;
};

#endif