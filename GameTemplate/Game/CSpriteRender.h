#pragma once
#include "SpriteSupporter.h"

namespace prefab
{
	class CSpriteRender : public IGameObject
	{
	public:
		//�`�����
		enum DrawScreen
		{
			Screen1,//�����
			Screen2,//�E���
			AllScreen//��ʑS��
		};

		//�X�v���C�g�̃��[�h
		enum SpriteMode
		{
			Normal,		//�ʏ�
			Transition,	//�g�����W�V�����p
		};
	private:
		void Update()override;
	public:

		/**
		 * @brief �X�v���C�g�̕`��
		 * @param rc �����_�[�R���e�L�X�g
		 * @param camera �`�悷��J����
		*/
		void Render(RenderContext& rc, Camera* camera) override;

		/**
		 * @brief �X�v���C�g�̕`��@���f������`����������ɌĂ΂��
		 * @param rc �����_�[�R���e�L�X�g
		 * @param camera �`�悷��J����
		*/
		void PostRender(RenderContext& rc, Camera* camera) override;

		/**
		 * @brief �X�v���C�g�̏�����
		 * @param spritePath �X�v���C�g�̃p�X
		 * @param width �X�v���C�g�̉���
		 * @param height �X�v���C�g�̍���
		*/
		void Init(const char* spritePath, UINT width, UINT height);

		/**
		 * @brief �X�v���C�g�̍��W��ݒ�
		 * @param pos �X�v���C�g�̍��W
		*/
		void SetPosition(const Vector3& pos);

		/**
		 * @brief �X�v���C�g�̉�]��ݒ�
		 * @param qRot �X�v���C�g�̉�]
		*/
		void SetRotation(const Quaternion& qRot);

		/**
		 * @brief �X�v���C�g�̊g�嗦��ݒ�
		 * @param �g�嗦
		*/
		void SetScale(const Vector3& scale);

		/**
		 * @brief �X�v���C�g�̃s�{�b�g��ݒ�
		 * @param �s�{�b�g
		*/
		void SetPivot(const Vector2& pivot);

		/**
		 * @brief �X�v���C�g��`�悷���ʂ�ݒ�
		 * @param screen �`����
		*/
		void SetDrawScreen(DrawScreen screen) { m_drawScreen = screen; }
		
		/**
		 * @brief �X�v���C�g�����f���̕`���ɕ`����?
		 * @param ���f���̕`���ɕ`��(PostRender):true ���f���Ɠ����`��^�C�~���O�ŕ`��(Render):false
		*/
		void SetPostRenderMode(bool mode) { m_isPostRender = mode; }

		/**
		 * @brief �X�v���C�g�̃��[�h��ݒ�
		 * @param sm �X�v���C�g�̃��[�h
		*/
		void SetSpriteMode(SpriteMode sm) { m_spriteMode = sm; }

		/**
		 * @brief �X�v���C�g�̃g�����W�V�����̃{�[�_�[���Z�b�g
		 * @param border �{�[�_�[
		*/
		void SetTransitionBorder(const float& border) {m_sprite.SetAlpha(border); }

		/**
		 * @brief �X�v���C�g�̏�Z�J���[��ݒ�
		 * @param mulColor ��Z�J���[
		*/
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}

		/**
		 * @brief �X�v���C�g�̍��W���擾
		 * @return ���W
		*/
		const Vector3& GetPosition() const { return m_position; }

		/**
		 * @brief �X�v���C�g�̉�]���擾
		 * @return ��]
		*/
		const Quaternion& GetRotation() const { return m_qRot; }

		/**
		 * @brief �X�v���C�g�̊g�嗦���擾
		 * @return �g�嗦
		*/
		const Vector3& GetScale() const { return m_scale; }

		/**
		 * @brief �X�v���C�g�̃s�{�b�g���擾
		 * @return �s�{�b�g
		*/
		const Vector2& GetPivot() const { return m_pivot; }

		/**
		 * @brief �X�v���C�g�̕`����ʂ��擾
		 * @return �`�����
		*/
		DrawScreen GetDrawScreen() { return m_drawScreen; }

		/**
		 * @brief �X�v���C�g�̏�Z�J���[���擾
		 * @return ��Z�J���[
		*/
		const Vector4& GetMulColor() const { return m_sprite.GetMulColor(); }

		/**
		 * @brief �X�v���C�g�T�|�[�^�[���擾
		 * @return �X�v���C�g�T�|�[�^�[
		*/
		SpriteSupporter& GetSpriteSupporter() { return m_spriteSupporter; }

		/**
		 * @brief �X�v���C�g�T�|�[�^�[���g�p���邩�̃t���O���Z�b�g
		 * @param flag �t���O
		*/
		void SetUseSpriteSupporterFlag(bool flag)
		{
			m_useSpriteSupporterFlag = flag;
		}
	private:
		SpriteSupporter m_spriteSupporter;			//�X�v���C�g�T�|�[�^�[�{��
		Sprite m_sprite;							//�X�v���C�g
		Vector3 m_position = Vector3::Zero;			//�X�v���C�g�̍��W
		Quaternion m_qRot = Quaternion::Identity;	//�X�v���C�g�̉�]
		Vector3 m_scale = Vector3::One;				//�X�v���C�g�̊g�嗦
		Vector2 m_pivot = { 0.5f,0.5f };			//�X�v���C�g�̃s�{�b�g
		DrawScreen m_drawScreen = AllScreen;		//�X�v���C�g�̕`�����
		SpriteMode m_spriteMode = Normal;			//�X�v���C�g�̃��[�h
		bool m_isPostRender = true;					//�X�v���C�g�����f���̕`�悪�I����Ă���`�悷�邩?
		bool m_useSpriteSupporterFlag = true;		//�X�v���C�g�T�|�[�^�[���g�p����?
	};
}

