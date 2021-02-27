#pragma once

namespace prefab
{
	class CSpriteRender : public IGameObject
	{
	private:
		Sprite m_sprite;

		Vector3 m_position;
		Quaternion m_qRot;
		Vector3 m_scale;
		Vector2 m_pivot;

		void UpdateModel();
	public:
		CSpriteRender() :m_position(Vector3::Zero), m_qRot(Quaternion::Identity), m_scale(Vector3::One), m_pivot({ 0.5f,0.5f }) {}

		void Render(RenderContext& rc, Camera* camera) override;

		void Init(const char* spritePath, UINT width, UINT height);

		void SetPosition(Vector3 pos);
		void SetRotation(Quaternion qRot);
		void SetScale(Vector3 scale);
		void SetPivot(Vector2 pivot);

		Vector3 GetPosition() { return m_position; }
		Quaternion GetRotation() { return m_qRot; }
		Vector3 GetScale() { return m_scale; }
		Vector2 GetPivot() { return m_pivot; }
	};
}

