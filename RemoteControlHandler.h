#ifndef REMOTE_CONTROL_HANDLER_H
#define REMOTE_CONTROL_HANDLER_H

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <M5Unified.h> // For M5.Lcd

#include "BeepPlayer.h" // Include BeepPlayer header (これはBeepPlayerクラスを使用するため必要)

/**
 * @brief リモコンからのIR信号を受信し、処理するクラス
 */
class RemoteControlHandler {
public:
    /**
     * @brief コンストラクタ
     * @param recvPin IR受信に使用するピン番号
     * @param beepPlayer BeepPlayerオブジェクトへのポインタ
     */
    RemoteControlHandler(uint16_t recvPin, BeepPlayer* beepPlayer);

    /**
     * @brief デストラクタ
     */
    ~RemoteControlHandler(); // Destructor to clean up owned resources

    /**
     * @brief IR信号の受信と処理を行う (loop関数から呼び出す)
     */
    void loop();

private:
    uint16_t _recvPin;
    IRrecv* _irrecv; // このクラスが所有するオブジェクト
    decode_results _results;
    BeepPlayer* _player; // 外部から渡されるポインタ (このクラスは所有しない)

    // IRコードとWAVファイル名のマッピング構造体
    struct IrCodeMapping {
        uint32_t code;
        const char* filename;
    };

    // サポートするIRコードと対応するファイル名の配列
    // constを付けて変更されないようにする
    const IrCodeMapping _codeMappings[6] = { // Assuming 6 buttons
        {0x415f01fe, "/415f01fe.wav"}, // 切
        {0x415f817e, "/415f817e.wav"}, // 弱
        {0x415fa15e, "/415fa15e.wav"}, // タイマー
        {0x415f41be, "/415f41be.wav"}, // 中
        {0x415f21de, "/415f21de.wav"}, // ランプ
        {0x415fc13e, "/415fc13e.wav"}  // 強
    };
};

#endif