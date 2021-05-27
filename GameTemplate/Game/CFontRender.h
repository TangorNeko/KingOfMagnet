#pragma once

namespace prefab
{
	class CFontRender : public IGameObject
	{
	public:
		enum DrawScreen
		{
			Screen1,//左画面
			Screen2,//右画面
			AllScreen//画面全体
		};
	private:
		Font m_font;

		std::wstring m_text = L"";//表示するテキスト
		Vector2 m_position = { 0.0f,0.0f };//表示位置
		Vector4 m_color = { 1.0f,1.0f,1.0f,1.0f };//フォントの色
		float m_rotation = 0.0f;//フォントの回転角度
		Vector2 m_scale = { 1.0f,1.0f };//フォントの拡大率
		Vector2 m_pivot = { 0.5f,0.5f };//フォントのピボット(動いていない?)

		Vector2 m_aspect = { 1.0f,1.0f };//フォントのアスペクト比(1画面,2画面の切り替えで使う)

		bool m_drawShadowFlag = false;//フォントのアウトラインを描くかのフラグ
		float m_shadowOffset = 1.0f;//フォントのアウトラインの太さ
		Vector4 m_shadowColor = { 1.0f,1.0f,1.0f,1.0f };//フォントのアウトラインの色

		DrawScreen m_drawScreen = AllScreen;//描画先画面
	public:
		//描画命令
		void PostRender(RenderContext& rc, Camera* camera) override;

		//テキストをセット
		void SetText(const std::wstring& text) { m_text = text; }

		//座標をセット
		void SetPosition(const Vector2& position) { m_position = position; }

		//フォントの色をセット
		void SetColor(const Vector4& color) { m_color = color; }

		//フォントの回転をセット
		void SetRotation(const float& rotation) { m_rotation = rotation;; }

		//フォントの拡大率をセット
		void SetScale(const Vector2& scale) { m_scale = scale; }

		//フォントのピボットをセット(動いていない...)
		void SetPivot(const Vector2& pivot) { m_pivot = pivot; }

		//フォントの描画先画面をセット
		void SetDrawScreen(DrawScreen screen) { m_drawScreen = screen; }

		//フォントのアウトラインの描画フラグをセット
		void SetShadowFlag(bool flag)
		{
			m_drawShadowFlag = flag;
			m_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}

		//フォントのアウトラインの太さをセット
		void SetShadowOffset(float offset)
		{
			m_shadowOffset = offset;
			m_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}

		//フォントのアウトラインの色をセット
		void SetShadowColor(Vector4 color)
		{
			m_shadowColor = color;
			m_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}
	};
}

