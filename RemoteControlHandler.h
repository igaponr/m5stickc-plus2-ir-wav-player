#ifndef REMOTE_CONTROL_HANDLER_H
#define REMOTE_CONTROL_HANDLER_H

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <M5Unified.h>

#include "BeepPlayer.h"

class RemoteControlHandler {
public:
    // ★★★ コンストラクタを正しい宣言に戻します ★★★
    RemoteControlHandler(uint16_t recvPin, BeepPlayer* beepPlayer);

    ~RemoteControlHandler();

    void loop();

private:
    uint16_t _recvPin;
    // ★★★ IRrecvオブジェクトのポインタをクラスのメンバとして正しく宣言します ★★★
    IRrecv* _irrecv;
    decode_results _results;
    BeepPlayer* _player;

    struct IrCodeMapping {
        uint32_t code;
        const char* filename;
    };

    const IrCodeMapping _codeMappings[6] = {
        {0x415f01fe, "/415f01fe.wav"},
        {0x415f817e, "/415f817e.wav"},
        {0x415fa15e, "/415fa15e.wav"},
        {0x415f41be, "/415f41be.wav"},
        {0x415f21de, "/415f21de.wav"},
        {0x415fc13e, "/415fc13e.wav"}
    };
};

#endif