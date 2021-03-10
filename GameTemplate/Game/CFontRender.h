#pragma once

namespace prefab
{
	class CFontRender : public IGameObject
	{
	public:
		enum DrawScreen
		{
			Screen1,
			Screen2,
			AllScreen = 3
		};
	private:
		Font m_font;

		std::wstring m_text = L"";
		Vector2 m_position = { 0.0f,0.0f };
		Vector4 m_color = { 1.0f,1.0f,1.0f,1.0f };
		float m_rotation = 0.0f;
		float m_scale = 1.0f;
		Vector2 m_pivot = { 0.5f,0.5f };

		DrawScreen m_drawScreen = AllScreen;
	public:
		void Render(RenderContext& rc, Camera* camera) override;

		void SetText(const std::wstring& text) { m_text = text; }

		void SetPosition(const Vector2& position) { m_position = position; }

		void SetColor(const Vector4& color) { m_color = color; }

		void SetRotation(const float& rotation) { m_rotation = rotation;; }

		void SetScale(const float& scale) { m_scale = scale; }

		void SetPivot(const Vector2& pivot) { m_pivot = pivot; }

		void SetDrawScreen(DrawScreen screen) { m_drawScreen = screen; }
	};
}

