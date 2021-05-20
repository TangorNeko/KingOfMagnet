#pragma once
#include "SpriteSupporter.h"

namespace prefab
{
	class CSpriteRender : public IGameObject
	{
	public:
		enum DrawScreen
		{
			Screen1,//�����
			Screen2,//�E���
			AllScreen//��ʑS��
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

		void PostRender(RenderContext& rc, Camera* camera) override;

		void Init(const char* spritePath, UINT width, UINT height);

		void Update()override;

		void SetPosition(Vector3 pos);
		void SetRotation(Quaternion qRot);
		void SetScale(Vector3 scale);
		void SetPivot(Vector2 pivot);
		void SetDrawScreen(DrawScreen screen) { m_drawScreen = screen; }
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}

		Vector3 GetPosition() { return m_position; }
		Quaternion GetRotation() { return m_qRot; }
		Vector3 GetScale() { return m_scale; }
		Vector2 GetPivot() { return m_pivot; }
		DrawScreen GetDrawScreen() { return m_drawScreen; }
		Vector4 GetMulColor() { return m_sprite.GetMulColor(); }

		/// <summary>
		/// �X�v���C�g���y�ɓ������N���X���Ăяo���邼
		/// </summary>
		/// <remarks>
		/// 
		/// ���T���v���R�[�h��
		/// render->m_spriteSupporter.SpriteMove(�ݒ�l);
		/// 
		/// </remarks>
		SpriteSupporter m_spriteSupporter;		//�X�v���C�g�T�|�[�^�[�{��

	};
}

