#include "RemoteControlHandler.h"

RemoteControlHandler::RemoteControlHandler(uint16_t recvPin, BeepPlayer* beepPlayer)
    : _recvPin(recvPin), _player(beepPlayer) {

    // ★★★ LEDバーを無効化して解放された「RMTチャンネル0」を明示的に指定します ★★★
    _irrecv = new IRrecv(_recvPin, 0);

    _irrecv->enableIRIn();
    Serial.println("IR receiver enabled using RMT Channel 0.");
}

RemoteControlHandler::~RemoteControlHandler() {
    // _irrecvがクラスのメンバとして正しく宣言されたので、このコードは有効になります
    if (_irrecv) {
        delete _irrecv;
        _irrecv = nullptr;
    }
}

void RemoteControlHandler::loop() {
    // _irrecvがクラスのメンバなので、ここでも正しく動作します
    if (_player && !_player->isPlaying()) {
        _irrecv->enableIRIn();
        if (_irrecv->decode(&_results)) {
            Serial.printf("Received IR code: 0x%08X\n", _results.value);
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.setCursor(0, 0);
            M5.Lcd.println("Zundamon Remote");
            M5.Lcd.setCursor(0, 50);
            M5.Lcd.printf("IR: 0x%08X", _results.value);

            const char* fileToPlay = nullptr;
            for (int i = 0; i < sizeof(_codeMappings) / sizeof(_codeMappings[0]); ++i) {
                if (_results.value == _codeMappings[i].code) {
                    fileToPlay = _codeMappings[i].filename;
                    break;
                }
            }

            if (_player && fileToPlay) {
                _player->playAudioFile(fileToPlay);
                _irrecv->disableIRIn();
            } else {
                Serial.printf("Unknown IR code: 0x%08X\n", _results.value);
                M5.Lcd.setCursor(0, 80);
                M5.Lcd.println("Unknown Code!");
            }
        }
    }
}
