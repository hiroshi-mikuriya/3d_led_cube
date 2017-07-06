#ifndef LED_H__
#define LED_H__

#include <string>

namespace makerfaire
{
	namespace fxat
	{
		/*!
		ウェイトする
		@param[in] ms 待機時間[ミリ秒]
		*/
		void Wait(int ms);

		class Led
		{
		public:
			enum
			{
				Width = 16,	///< 幅
				Height = 32,	///< 高さ
				Depth = 8,	///< 奥行
			};

			/*!
			3D LEDのURLを設定する
			本メソッドを呼び出すと、Showのたびに3D LEDへ描画情報が転送される
			@param[in] url LED制御装置（Raspberry Piなど）のURL
			*/
			void SetUrl(std::string url);

			/*!
			LED単体の色を設定する
			本関数は色の設定をキャッシュするだけで表示の更新はしない。
			@param[in] x X方向座標（0 <= x < Width）
			@param[in] y Y方向座標（0 <= y < Height）
			@param[in] z Z方向座標（0 <= z < Depth）
			@param[in] rgb 色（0x00000000 <= rgb <= 0x00FFFFFF）
			@note 範囲外のパラメータを指定した場合の動作は未定義（例外投げるかもよ）
			*/
			void SetLed(int x, int y, int z, int rgb);

			/*!
			LED設定のキャッシュをクリアする
			*/
			void Clear();

			/*!
			SetLedで設定したとおりにLED表示する
			*/
			void Show();

		private:
			int m[Width][Height][Depth];
			std::string m_url;
		};
	}
}

#endif // LED_H__