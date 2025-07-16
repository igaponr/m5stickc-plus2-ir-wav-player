#include "RemoteControlHandler.h" // このヘッダーがRemoteControlHandlerクラスの定義をインクルード

RemoteControlHandler::RemoteControlHandler(uint16_t recvPin, BeepPlayer* beepPlayer)
    : _recvPin(recvPin), _player(beepPlayer) {
    // Create and initialize the IRrecv object
    _irrecv = new IRrecv(_recvPin); // ヒープに確保
    _irrecv->enableIRIn();
    Serial.println("IR receiver enabled");
}

RemoteControlHandler::~RemoteControlHandler() {
    // Clean up objects owned by this class
    if (_irrecv) {
        // IRrecv doesn't have an explicit stop method needed before deletion,
        // simply deleting the object is usually sufficient.
        delete _irrecv;
        _irrecv = nullptr; // Ensure pointer is null after deletion
    }
    // Do NOT delete _player, it is owned by the caller
    Serial.println("RemoteControlHandler destroyed."); // For debugging
}

void RemoteControlHandler::loop() {
    // 再生中は新しいIR信号を処理しない
    // _playerが有効なことと、再生中でないことを確認
    if (_player != nullptr && !_player->isPlaying()) {
        if (_irrecv != nullptr && _irrecv->decode(&_results)) {
            Serial.printf("Received IR code: 0x%08X\n", _results.value);
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.setCursor(0, 0);
            M5.Lcd.println("Zundamon Remote");
            M5.Lcd.setCursor(0, 50);
            M5.Lcd.printf("IR: 0x%08X", _results.value);

            const char* fileToPlay = nullptr;
            // 受信したIRコードに対応するファイル名を探す
            for (int i = 0; i < sizeof(_codeMappings) / sizeof(_codeMappings[0]); ++i) {
                if (_results.value == _codeMappings[i].code) {
                    fileToPlay = _codeMappings[i].filename;
                    Serial.printf("Detected code for file: %s\n", fileToPlay);
                    break;
                }
            }

            if (_player && fileToPlay) {
                // 対応するファイルが見つかったらBeepPlayerに再生を依頼
                _player->playAudioFile(fileToPlay);
            } else {
                // 未知のコードの場合
                Serial.printf("Unknown IR code: 0x%08X\n", _results.value);
                M5.Lcd.setCursor(0, 80);
                M5.Lcd.println("Unknown Code!");
            }

            _irrecv->resume(); // 次の信号受信を再開
        }
    }
    // 短い遅延はメインループで行われる
}