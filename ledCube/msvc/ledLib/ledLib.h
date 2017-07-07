#pragma once

#define LED_WIDTH	16
#define LED_HEIGHT 32
#define LED_DEPTH	8

/*!
3D LEDを制御しているRaspberry PiのURL（IPアドレス）を指定する
本メソッドをコールすることで、ShowのたびにRaspberry Piへデータが転送される。
@param[in] url Raspberry PiのURL（IPアドレス）
*/
typedef void * SetUrl_t(char const * url);

/*!
LED単体の色を設定する
本関数は色の設定をキャッシュするだけで表示の更新はしない。
@param[in] x X方向座標（0 <= x < LED_WIDTH）
@param[in] y Y方向座標（0 <= y < LED_HEIGHT）
@param[in] z Z方向座標（0 <= z < LED_DEPTH）
@param[in] rgb 色（0x00000000 <= rgb <= 0x00FFFFFF）
@note 範囲外のパラメータを指定した場合の動作は未定義
*/
typedef void * SetLed_t(int x, int y, int z, int rgb);

/*!
LED設定のキャッシュをクリアする
*/
typedef void * Clear_t();

/*!
SetLedで設定したとおりにGUIに3D LEDを表示する
SetUrl呼び出し後は、RaspberryPiへ描画情報を転送する
*/
typedef void * Show_t();


/*!
ウェイトする
GUIにLEDを表示するには、本関数のコールが必要
Show関数がRaspberry Piへの転送する時間を考慮したウェイトをする
@param[in] ms 待機時間[ミリ秒]
*/
typedef void * Wait_t(int ms);
