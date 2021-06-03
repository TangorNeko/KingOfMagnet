#pragma once

namespace prefab
{

	/// <summary>
	/// スプライトの移動や演出をワンコマンドでやってくれる
	/// </summary>
	class SpriteSupporter
	{
	public:

		/// <summary>
		/// SpriteRenderのアドレスを代入する
		/// 最初に実行してね！
		/// </summary>
		/// <param name="sp">ポインタ</param>
		void SpriteRenderSetting(class CSpriteRender* sp) {
			m_spriteRender = sp;
		}

		//更新（SpriteRenderが呼ぶ）
		void SpriteSupporter_Update();

		/// <summary>
		/// スプライトの演出を全てリセットする
		/// </summary>
		void SpriteDelayReset();

		//実行するコマンド
		void SpriteMove(const Vector2& move, const int& moveTime,
			const int& moveDelay, const bool& relative = false);
		void SpriteMove(const Vector3& move, const int& moveTime,
			const int& moveDelay, const bool& relative = false);
		void SpriteRotation(const float& rot, const int& moveTime,
			const int& moveDelay, const bool& loopflag = false);
		void SpriteScale(const Vector3& scale, const int& moveTime, const int& moveDelay);
		void SpriteScale(const float& scale, const int& moveTime, const int& moveDelay);
		void SpriteColor(const Vector4& color, const int& moveTime, const int& moveDelay);
		void SpriteShake(const Vector2& move, const int& moveTime, const int& moveCount);

		/// <summary>
		/// このフラグがtrue かつ このスプライトが全く動いていない場合
		/// スプライトは自動でDeleteGOされる
		/// </summary>
		/// <param name="flag">フラグ</param>
		void SetAutoDeathFlag(const bool& flag) {
			m_autoDeathFlag = flag;
		}

		//状態取得用
		//スプライトの移動リスト要素数を取得
		//移動中のスプライトかどうかを判別する など
		int GetSpriteMoveListLen() {
			return static_cast<int>(m_spriteMoveList.size());
		}
		int GetSpriteScaleListLen() {
			return static_cast<int>(m_spriteScaleList.size());
		}

	private:
		//準備と片付け
		void SpriteDataUpdate();
		void SpriteDataReturn();
		//アップデート用
		void SpriteMoveUpdate();
		void SpriteRotationUpdate();
		void SpriteScaleUpdate();
		void SpriteColorUpdate();
		void SpriteShakeUpdate();

		//メンバ変数
		class CSpriteRender* m_spriteRender;
		Vector3 m_position = Vector3().Zero;					//座標
		Quaternion m_rotation = Quaternion().Identity;			//回転
		Vector3 m_scale = Vector3().One;						//拡大率
		Vector4 m_mulColor = { 1.0f,1.0f,1.0f,1.0f };			//乗算カラー

		//Move
		struct SpriteMoveSet {
			Vector2 m_spriteMovePos = { 0.0f,0.0f };			//スプライトの移動先
			Vector2 m_spriteMoveSpeed = { 0.0f,0.0f };			//スプライトの移動量
			int m_spriteMoveLimit = -1;							//スプライトの移動時間（-1は移動中ではない）
			int m_spriteMoveDelay = -1;							//スプライトの移動ディレイ
			int m_spriteMoveTimer = -1;							//スプライトの移動タイマー
			bool m_spriteMoveRelative = false;					//スプライトの相対移動フラグ（trueなら相対移動）
			bool m_spriteMoveDeleteFlag = false;				//削除フラグ（いじらないでね）
		};
		std::list<SpriteMoveSet> m_spriteMoveList;				//移動のリスト
		//Rotation
		Quaternion m_spriteRotation = Quaternion().Identity;	//1フレームの回転量
		int m_spriteRotationLimit = -1;							//スプライトの回転時間（-1は移動中ではない）
		int m_spriteRotationDelay = -1;							//スプライトの回転ディレイ
		int m_spriteRotationTimer = -1;							//スプライトの回転タイマー
		bool m_spriteLoopRotationFlag = false;					//trueなら永遠に延々にフォーエバー回る回る回る
		//Scale
		struct SpriteScaleSet {
			Vector3 m_spriteScale = Vector3().Zero;				//目標の大きさ
			Vector3 m_spriteScaleMove = Vector3().Zero;			//1フレームの変化量
			int m_spriteScaleLimit = -1;						//スプライトの拡大時間（-1は移動中ではない）
			int m_spriteScaleDelay = -1;						//スプライトの拡大ディレイ
			int m_spriteScaleTimer = -1;						//スプライトの拡大タイマー
			bool m_spriteScaleDeleteFlag = false;				//削除フラグ（いじらないでね）
		};
		std::list<SpriteScaleSet> m_spriteScaleList;			//拡大縮小のリスト
		//MulColor
		Vector4 m_spriteColor = Vector4().White;				//目標の色
		Vector4 m_spriteColorMove = Vector4().White;			//1フレームの変化量
		int m_spriteColorLimit = -1;							//スプライトの色変更時間（-1は変化中ではない）
		int m_spriteColorDelay = -1;							//スプライトの変化ディレイ
		int m_spriteColorTimer = -1;							//スプライトの変化タイマー
		//Shake
		Vector2 m_spriteShakeMove = { 0.0f,0.0f };				//シェイクでの移動距離
		Vector2 m_spriteShakeMove_OneFlame = { 0.0f,0.0f };		//シェイクの1フレーム移動距離
		int m_spriteShakeLimit = -1;							//スプライトのシェイク間隔（-1は変化中ではない）
		int m_spriteShakeCount = -1;							//スプライトのシェイク回数（0の場合、止めるまでループする）
		int m_spriteShakeCounter = -1;							//スプライトのシェイク回数カウンター
		int m_spriteShakeTimer = -1;							//スプライトのシェイクタイマー
		//AutoDeath
		bool m_autoDeathFlag = false;							//何もしていない場合、親のスプライトレンダーを削除する

	};

}