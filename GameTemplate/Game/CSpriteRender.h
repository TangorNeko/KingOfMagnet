#pragma once
#include "SpriteSupporter.h"

namespace prefab
{
	class CSpriteRender : public IGameObject
	{
	public:
		//描画先画面
		enum DrawScreen
		{
			Screen1,//左画面
			Screen2,//右画面
			AllScreen//画面全体
		};

		//スプライトのモード
		enum SpriteMode
		{
			Normal,		//通常
			Transition,	//トランジション用
		};
	private:
		void Update()override;
	public:

		/**
		 * @brief スプライトの描画
		 * @param rc レンダーコンテキスト
		 * @param camera 描画するカメラ
		*/
		void Render(RenderContext& rc, Camera* camera) override;

		/**
		 * @brief スプライトの描画　モデル等を描ききった後に呼ばれる
		 * @param rc レンダーコンテキスト
		 * @param camera 描画するカメラ
		*/
		void PostRender(RenderContext& rc, Camera* camera) override;

		/**
		 * @brief スプライトの初期化
		 * @param spritePath スプライトのパス
		 * @param width スプライトの横幅
		 * @param height スプライトの高さ
		*/
		void Init(const char* spritePath, UINT width, UINT height);

		/**
		 * @brief スプライトの座標を設定
		 * @param pos スプライトの座標
		*/
		void SetPosition(const Vector3& pos);

		/**
		 * @brief スプライトの回転を設定
		 * @param qRot スプライトの回転
		*/
		void SetRotation(const Quaternion& qRot);

		/**
		 * @brief スプライトの拡大率を設定
		 * @param 拡大率
		*/
		void SetScale(const Vector3& scale);

		/**
		 * @brief スプライトのピボットを設定
		 * @param ピボット
		*/
		void SetPivot(const Vector2& pivot);

		/**
		 * @brief スプライトを描画する画面を設定
		 * @param screen 描画画面
		*/
		void SetDrawScreen(DrawScreen screen) { m_drawScreen = screen; }
		
		/**
		 * @brief スプライトをモデルの描画後に描くか?
		 * @param モデルの描画後に描く(PostRender):true モデルと同じ描画タイミングで描く(Render):false
		*/
		void SetPostRenderMode(bool mode) { m_isPostRender = mode; }

		/**
		 * @brief スプライトのモードを設定
		 * @param sm スプライトのモード
		*/
		void SetSpriteMode(SpriteMode sm) { m_spriteMode = sm; }

		/**
		 * @brief スプライトのトランジションのボーダーをセット
		 * @param border ボーダー
		*/
		void SetTransitionBorder(const float& border) {m_sprite.SetAlpha(border); }

		/**
		 * @brief スプライトの乗算カラーを設定
		 * @param mulColor 乗算カラー
		*/
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}

		/**
		 * @brief スプライトの座標を取得
		 * @return 座標
		*/
		const Vector3& GetPosition() const { return m_position; }

		/**
		 * @brief スプライトの回転を取得
		 * @return 回転
		*/
		const Quaternion& GetRotation() const { return m_qRot; }

		/**
		 * @brief スプライトの拡大率を取得
		 * @return 拡大率
		*/
		const Vector3& GetScale() const { return m_scale; }

		/**
		 * @brief スプライトのピボットを取得
		 * @return ピボット
		*/
		const Vector2& GetPivot() const { return m_pivot; }

		/**
		 * @brief スプライトの描画先画面を取得
		 * @return 描画先画面
		*/
		DrawScreen GetDrawScreen() { return m_drawScreen; }

		/**
		 * @brief スプライトの乗算カラーを取得
		 * @return 乗算カラー
		*/
		const Vector4& GetMulColor() const { return m_sprite.GetMulColor(); }

		/**
		 * @brief スプライトサポーターを取得
		 * @return スプライトサポーター
		*/
		SpriteSupporter& GetSpriteSupporter() { return m_spriteSupporter; }

		/**
		 * @brief スプライトサポーターを使用するかのフラグをセット
		 * @param flag フラグ
		*/
		void SetUseSpriteSupporterFlag(bool flag)
		{
			m_useSpriteSupporterFlag = flag;
		}
	private:
		SpriteSupporter m_spriteSupporter;			//スプライトサポーター本体
		Sprite m_sprite;							//スプライト
		Vector3 m_position = Vector3::Zero;			//スプライトの座標
		Quaternion m_qRot = Quaternion::Identity;	//スプライトの回転
		Vector3 m_scale = Vector3::One;				//スプライトの拡大率
		Vector2 m_pivot = { 0.5f,0.5f };			//スプライトのピボット
		DrawScreen m_drawScreen = AllScreen;		//スプライトの描画先画面
		SpriteMode m_spriteMode = Normal;			//スプライトのモード
		bool m_isPostRender = true;					//スプライトをモデルの描画が終わってから描画するか?
		bool m_useSpriteSupporterFlag = true;		//スプライトサポーターを使用する?
	};
}

