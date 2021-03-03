#pragma once

namespace prefab
{
	class CSpriteRender : public IGameObject
	{
	public:
		enum DrawScreen
		{
			Screen1,
			Screen2,
			AllScreen = 3
		};
	private:
		Sprite m_sprite;

		Vector3 m_position;
		Quaternion m_qRot;
		Vector3 m_scale;
		Vector2 m_pivot;
		DrawScreen m_drawScreen = AllScreen;

	public:
		CSpriteRender() :m_position(Vector3::Zero), m_qRot(Quaternion::Identity), m_scale(Vector3::One), m_pivot({ 0.5f,0.5f }) {}

		void Render(RenderContext& rc, Camera* camera) override;

		void Init(const char* spritePath, UINT width, UINT height);

		void SetPosition(Vector3 pos);
		void SetRotation(Quaternion qRot);
		void SetScale(Vector3 scale);
		void SetPivot(Vector2 pivot);
		void SetDrawScreen(DrawScreen screen) { m_drawScreen = screen; }

		Vector3 GetPosition() { return m_position; }
		Quaternion GetRotation() { return m_qRot; }
		Vector3 GetScale() { return m_scale; }
		Vector2 GetPivot() { return m_pivot; }
		DrawScreen GetDrawScreen() { return m_drawScreen; }
	};
}

