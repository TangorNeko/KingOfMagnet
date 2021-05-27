#pragma once

namespace prefab
{
	class CFontRender : public IGameObject
	{
	public:
		enum DrawScreen
		{
			Screen1,//�����
			Screen2,//�E���
			AllScreen//��ʑS��
		};
	private:
		Font m_font;

		std::wstring m_text = L"";//�\������e�L�X�g
		Vector2 m_position = { 0.0f,0.0f };//�\���ʒu
		Vector4 m_color = { 1.0f,1.0f,1.0f,1.0f };//�t�H���g�̐F
		float m_rotation = 0.0f;//�t�H���g�̉�]�p�x
		Vector2 m_scale = { 1.0f,1.0f };//�t�H���g�̊g�嗦
		Vector2 m_pivot = { 0.5f,0.5f };//�t�H���g�̃s�{�b�g(�����Ă��Ȃ�?)

		Vector2 m_aspect = { 1.0f,1.0f };//�t�H���g�̃A�X�y�N�g��(1���,2��ʂ̐؂�ւ��Ŏg��)

		bool m_drawShadowFlag = false;//�t�H���g�̃A�E�g���C����`�����̃t���O
		float m_shadowOffset = 1.0f;//�t�H���g�̃A�E�g���C���̑���
		Vector4 m_shadowColor = { 1.0f,1.0f,1.0f,1.0f };//�t�H���g�̃A�E�g���C���̐F

		DrawScreen m_drawScreen = AllScreen;//�`�����
	public:
		//�`�施��
		void PostRender(RenderContext& rc, Camera* camera) override;

		//�e�L�X�g���Z�b�g
		void SetText(const std::wstring& text) { m_text = text; }

		//���W���Z�b�g
		void SetPosition(const Vector2& position) { m_position = position; }

		//�t�H���g�̐F���Z�b�g
		void SetColor(const Vector4& color) { m_color = color; }

		//�t�H���g�̉�]���Z�b�g
		void SetRotation(const float& rotation) { m_rotation = rotation;; }

		//�t�H���g�̊g�嗦���Z�b�g
		void SetScale(const Vector2& scale) { m_scale = scale; }

		//�t�H���g�̃s�{�b�g���Z�b�g(�����Ă��Ȃ�...)
		void SetPivot(const Vector2& pivot) { m_pivot = pivot; }

		//�t�H���g�̕`����ʂ��Z�b�g
		void SetDrawScreen(DrawScreen screen) { m_drawScreen = screen; }

		//�t�H���g�̃A�E�g���C���̕`��t���O���Z�b�g
		void SetShadowFlag(bool flag)
		{
			m_drawShadowFlag = flag;
			m_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}

		//�t�H���g�̃A�E�g���C���̑������Z�b�g
		void SetShadowOffset(float offset)
		{
			m_shadowOffset = offset;
			m_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}

		//�t�H���g�̃A�E�g���C���̐F���Z�b�g
		void SetShadowColor(Vector4 color)
		{
			m_shadowColor = color;
			m_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}
	};
}

